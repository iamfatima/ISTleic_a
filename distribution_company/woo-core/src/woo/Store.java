package woo;
import woo.exceptions.*;
import java.io.*;
import java.util.*;
import java.util.regex.Pattern;

/**
 * Class Store implements a store.
 */
public class Store implements Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 202009192006L;

  private int _balance = 0;
  private int _date = 0;
  private int _currentTransactionId = 0;
  private Map<String, Client> _clients = new TreeMap<String, Client>(String.CASE_INSENSITIVE_ORDER);
  private Map<String, Supplier> _suppliers = new TreeMap<String, Supplier>(String.CASE_INSENSITIVE_ORDER);
  private Map<String, Product> _products = new TreeMap<String, Product>(String.CASE_INSENSITIVE_ORDER);
  private Map<Integer, Transaction> _transactions = new TreeMap<Integer, Transaction>();

  /* Aux for register order */
  private Order _tempOrder;

  /**
   * @param txtfile filename to be loaded.
   * @throws IOException
   * @throws BadEntryException
   * @throws IOException
   */
  void importFile(String txtfile) throws IOException, BadEntryException {
    try{
      BufferedReader reader = new BufferedReader(new FileReader(txtfile));
      String line;

      while((line = reader.readLine()) != null)
      {
        String[] fields = line.split("\\|");
        registerFromFields(fields);

      }

      reader.close();
    }
    catch(IOException ioe)
    {
      ioe.printStackTrace();
    }
  }

  //Client methods --------------------------------------------------------------------------

  /**
   * Registers a new client in the system
   * @param id client key
   * @param name client name
   * @param address client address
   */
  void registerClient(String id, String name, String address)
  {
    Client client = new Client(id, name, address);
    newClientNotifications(client);
    _clients.put(id, client);
  }

  /**
   * Checks if the client id/key is in use in the system
   * @param id client key
   * @return true if already in use
   */
  boolean containsClientKey(String id)
  {
    return _clients.containsKey(id);
  }

  /**
   * Gets the corresponding client's toString, notifications and cleans their notifications 
   * @param id client key
   * @return client's information + all of their notifications
   */
  String displayClient(String id)
  {
    String string = "";
    Client client = _clients.get(id);

    string += client.toString() + "\n";
    for(String notif : client.getNotifications())
      string += notif + "\n";

    return string;
  }

  /**
   * Creates a string containing all clients's information
   * @return merged clients' toStrings
   */
  String displayClients()
  {
    String string = "";
    for(Map.Entry<String, Client> entry : _clients.entrySet())
    {
      string += entry.getValue().toString();
      string += "\n";
    }

    return string;
  }

  /**
   * Gets all sales the store has made to the specified client
   * @param id client key
   * @return merged string of all client's transactions
   */
  String displayClientTransactions(String id)
  {
    String string = "";
    LinkedList<Integer> purchases = _clients.get(id).getPurchases();

    for (int p : purchases)
    {
      Sale sale = (Sale) _transactions.get(p);
      if(!sale.isPaid())
        sale.setPriceToPay(calculatePrice(sale.getId()));

      string += sale.toString();
      string += "\n";
    }

    return string;
  }

    /**
   * Registers or removes an observer on the product, depending if it's present or not
   * @param clientId client/observer id
   * @param productId product's id
   * @return true if client is now observing product (has notifications)
   */
  boolean toggleNotifications(String clientId, String productId)
  {
    return _products.get(productId).toggleNotifications(_clients.get(clientId));
  }



  //Product methods --------------------------------------------------------------------------

  /**
   * Registers a new box in the system
   * @param id product key
   * @param supplierId product's supplier key
   * @param price price per unit sold
   * @param minStock minimum threshhold for stock
   * @param service service type
   */
  void registerProductBox(String id, String supplierId, int price, int minStock, String service)
  {
    Product product = new Box(id, supplierId, price, minStock, service);
    newProductNotifications(product);
    _products.put(id, product);
  }

  /**
   * Registers a new container in the system
   * @param id product key
   * @param supplierId product's supplier key
   * @param price price per unit sold
   * @param minStock minimum threshhold for stock
   * @param service service type
   * @param quality service quality
   */
  void registerProductContainer(String id, String supplierId, int price, int minStock, String service, String quality)
  {
    Product product = new Container(id, supplierId, price, minStock, service, quality);
    newProductNotifications(product);
    _products.put(id, product);
  }

  /**
   * Registers a new book in the system
   * @param id product key
   * @param supplierId product's supplier key
   * @param price price per unit sold
   * @param minStock minimum threshhold for stock
   * @param title book's title
   * @param author book's author
   * @param ISBN book's ISBN (string)
   */
  void registerProductBook(String id, String supplierId, int price, int minStock, String title, String author, String ISBN)
  {
    Product product = new Book(id, supplierId, price, minStock, title, author, ISBN);
    newProductNotifications(product);
    _products.put(id, product);
  }

  /**
   * Checks if the product id/key is in use in the system
   * @param id product key
   * @return true if already in use
   */
  boolean containsProductKey(String id)
  {
    return _products.containsKey(id);
  }

  /**
   * Creates a string containing all products' information
   * @return merged products' toStrings
   */
  String displayProducts() 
  {
    String string = "";

    for(Map.Entry<String, Product> entry : _products.entrySet())
    {
      string += entry.getValue().toString();
      string += "\n";
    }

    return string;
  }

  String lessProduct()
  {
    int stock = -1;
    String id = "";

    for(Map.Entry<String, Product> entry : _products.entrySet())
    {
      if(entry.getValue().getStock() < stock)
        stock = entry.getValue().getStock();
        id = entry.getValue().getId();

    }

    return id;
  }

  /**
   * Verifies if the store has enough product in stock
   * @param productId product's id
   * @param amount amount to compare it to
   * @return true if there's enough stock (stock >= amount asked)
   */
  boolean hasEnoughProduct(String productId, int amount)
  {
    return _products.get(productId).getStock() >= amount;
  }

  /**
   * Gets the current stock for asked product
   * @param productId product's id
   * @return amount currently in store
   */
  int getExistingStock(String productId)
  {
    return _products.get(productId).getStock();
  }


  /**
   * Changes a product's price and notifies observers if it's lowered
   * @param productId product's id
   * @param price new price
   */
  void changePrice(String productId, int price)
  {
    Product product = _products.get(productId);
    int oldPrice = product.getPrice();

    if(price < 0)
      return;
    product.setPrice(price);

    if(oldPrice > price)
      product.notifyBargain();
  }

  //Supplier methods --------------------------------------------------------------------------

  /**
   * Registers a new supplier in the system
   * @param id supplier key
   * @param name supplier name
   * @param address supplier address
   */
  void registerSupplier(String id, String name, String address)
  {
    Supplier supplier = new Supplier(id, name, address);
    _suppliers.put(id, supplier);
  }

  /**
   * Checks if the supplier id/key is in use in the system
   * @param id supplier key
   * @return true if already in use
   */
  boolean containsSupplierKey(String id)
  {
    return _suppliers.containsKey(id);
  }

  /**
   * Creates a string containing all suppliers' information
   * @return merged suppliers' toStrings
   */
  String displaySuppliers()
  {
    String string = "";
    for(Map.Entry<String, Supplier> entry : _suppliers.entrySet())
    {
      string += entry.getValue().toString();
      string += "\n";
    }

    return string;
  }

  /**
   * Switch the availability of a supplier
   * @param id supplier key
   * @return true if available
   */

  boolean switchAvailability(String id)
  {
    return _suppliers.get(id).switchAvailability();
  }

  /**
   * Evaluates the availability of a supplier
   * @param id supplier key
   * @return true if available
   */
  public boolean isSupplierAvailable(String id)
  {
    
    return _suppliers.get(id).isAvailable();
  }

  /**
   * Displays all transactions for a given supplier
   * @param id supplier key
   * @return string containing all of the supplier's transactions
   */
  String showSupplierTransactions(String id)
  {
    String string = "";
    LinkedList<Integer> transactions = _suppliers.get(id).getTransactions();

    for (int t : transactions)
    {
      Transaction tr =  _transactions.get(t);
      string += tr.toString();
      string += "\n";
    }

    return string;
  }

  String bestSupplier()
  {
    int highest = -1;
    String name  = "";
    for(Map.Entry<String, Supplier> entry : _suppliers.entrySet())
    {
      
      if (entry.getValue().getTransactions().size() > highest)
        highest = entry.getValue().getTransactions().size();
        name = entry.getValue().getName();

    }

    return name;
    
  }

  //Transaction methods --------------------------------------------------------------------------

  /**
   * Gets a transactions information given the id
   * @param id transaction's id
   * @return transaction's toString
   */
  String displayTransaction(int id)
  {
    if(_transactions.get(id) instanceof Sale)
    {
      Sale sale = (Sale) _transactions.get(id);

      if(!sale.isPaid()) //Updates to-pay prices for unpaid Sales
        sale.setPriceToPay(calculatePrice(id));

      return sale.toString();
    }
    else
      return _transactions.get(id).toString();
    
  }

   /**
   * Checks if the transaction id/key is in use in the system
   * @param id transaction key
   * @return true if already in use
   */
  boolean containsTransactionKey(int id)
  {
    return _transactions.containsKey(id);
  }

  //Order methods --------------------------------------------------------------------------

  /**
   * Creates a temporary order that could either be registered or deleted
   * @param supplierId id of the order's supplier
   */
  void newOrder(String supplierId)
  {
    _tempOrder = new Order(_currentTransactionId, supplierId, _date);
  }

  /**
   * Adds product to the temporary order
   * @param productId id of the product to add
   * @param amount amount of product to add
   */
  void addToOrder(String productId, int amount)
  {
    _tempOrder.addProduct(productId, amount, _products.get(productId).getPrice());
  }

  /**
   * Registers temporary order into the transaction structure and it's supplier's history, notifiying clients if it's restocked
   */
  void registerOrder()
  {
    TreeMap<String, Integer> orderedProducts = _tempOrder.getProducts();

    for(Map.Entry<String, Integer> entry : orderedProducts.entrySet())
    {
      if(_products.get(entry.getKey()).getStock() == 0)
        _products.get(entry.getKey()).notifyNew();

      _products.get(entry.getKey()).addStock(entry.getValue());
    }
    
    _transactions.put(_currentTransactionId, _tempOrder);
    _balance -= _tempOrder.getPrice();
    _suppliers.get(_tempOrder.getSupplierId()).addTransaction(_currentTransactionId++);
  }

  //Sale methods --------------------------------------------------------------------------

  /**
   * Registers a new sale
   * @param clientId client's key
   * @param deadline client's payment deadline
   * @param productId product key
   * @param amount amount bought
   */
  void registerSale(String clientId, int deadline, String productId, int amount)
  {
    Sale sale = new Sale(_currentTransactionId, clientId, deadline, productId, amount, _products.get(productId).getPrice(), _date);
    _transactions.put(_currentTransactionId, sale);

    _products.get(productId).removeStock(amount);
    _clients.get(clientId).makePurchase(_currentTransactionId++, sale.getPrice());
  }

  /**
   * Pays a transaction in the system. Does nothing if an order or already paid.
   * @param transactionId transaction's key
   */
  void paySale(int transactionId)
  {
    if(_transactions.get(transactionId) instanceof Sale)
    {
      Sale sale = (Sale) _transactions.get(transactionId);
      if(!sale.isPaid())
      {
        int paid = executePayment(transactionId);
        sale.Pay(_date, paid);
        _balance += paid;
      }
    }
  }

  //Date methods --------------------------------------------------------------------------

  /**
   * Advances internal date counter
   * @param days number of days to advance
   */
  void advanceDate(int days)
  {
    this._date += days;
  }

  /**
   * Gets date's value
   * @return returns integer corresponding to the current date
   */
  int getDate()
  {
    return this._date;
  }


  //Miscellaneous methods --------------------------------------------------------------------------

  /**
   * Calculates the price of a sale, taking discounts and fines
   * @param transactionId transaction's id
   * @return the price the client would pay on today's date
   */
  int calculatePrice(int transactionId)
  {
    Sale sale = (Sale) _transactions.get(transactionId);
    Client client = _clients.get(sale.getClientId());
    int deadline = sale.getDeadline();
    int offset = _products.get(sale.getProductId()).getDeadlineOffset();
    int price = sale.getPrice();

    if(deadline - _date >= offset)
      return client.calculatePrice(PaymentPeriod.P1, _date, deadline, price);
    else if (deadline - _date < offset && deadline - _date >= 0)
      return client.calculatePrice(PaymentPeriod.P2, _date, deadline, price);
    else if (_date - deadline <= offset && _date - deadline > 0)
      return client.calculatePrice(PaymentPeriod.P3, _date, deadline, price);
    else
      return client.calculatePrice(PaymentPeriod.P4, _date, deadline, price);
  }


  /**
   * Executes a payment on the given date, updating client points
   * @param transactionId transaction's id
   * @return the price the client paid on today's date
   */
  int executePayment(int transactionId)
  {
    Sale sale = (Sale) _transactions.get(transactionId);
    Client client = _clients.get(sale.getClientId());
    int deadline = sale.getDeadline();
    int offset = _products.get(sale.getProductId()).getDeadlineOffset();
    int price = sale.getPrice();

    if(deadline - _date >= offset)
      return client.Pay(PaymentPeriod.P1, _date, deadline, price);
    else if (deadline - _date < offset && deadline - _date >= 0)
      return client.Pay(PaymentPeriod.P2, _date, deadline, price);
    else if (_date - deadline <= offset && _date - deadline > 0)
      return client.Pay(PaymentPeriod.P3, _date, deadline, price);
    else
      return client.Pay(PaymentPeriod.P4, _date, deadline, price);
  }

  /**
   * Gets all products whose prices are under a given limit
   * @param topPrice the upper limit for the price search
   * @return string containing all products toString
   */
  String lookupProductsByPrice(int topPrice)
  {
    String string = "";

    for(Map.Entry<String, Product> entry : _products.entrySet())
    {
      if(entry.getValue().getPrice() < topPrice)
      {
        string += entry.getValue().toString();
        string += "\n";
      }
    }

    return string;
  }


  /**
   * Gets all purchases the client has paid
   * @param clientId client's id
   * @return string containing all of client's paid purchases
   */
  String getClientPayments(String clientId)
  {
    String string = "";
    LinkedList<Integer> purchases = _clients.get(clientId).getPurchases();

    for (int id : purchases)
    {
      Sale sale = (Sale) _transactions.get(id);
      if(sale.isPaid())
      {
        string += sale.toString();
        string += "\n";
      }
    }

    return string;
  }

  /**
   * Checks if service string is a valid type of service
   * @param service name to test
   * @return true if it exists
   * @throws IllegalArgumentException
   */
  boolean validService(String service) throws IllegalArgumentException
  {
    try {
      Service.valueOf(service);
    }
    catch(IllegalArgumentException e)
    {
      return false;
    }

    return true;
  }


  /**
   * Checks if quality string is a valid type of quality
   * @param quality name to test
   * @return true if it exists
   * @throws IllegalArgumentException
   */
  boolean validQuality(String quality) throws IllegalArgumentException
  {
    try {
      Quality.valueOf(quality);
    }
    catch(IllegalArgumentException e)
    {
      return false;
    }

    return true;
  }

  /**
   * Gets the store's current balance
   *@return store's balance 
   */
  int getBalance()
  {
    return _balance;
  }

  /**
   * Gets the store's theoretical accounting balance for today's date
   *@return store's accounting balance 
   */
  int getAccountingBalance()
  {
    int clientDebt = 0;
    for(Map.Entry<Integer, Transaction> entry : _transactions.entrySet())
    {
      if(entry.getValue() instanceof Sale)
      {
        Sale sale = (Sale) entry.getValue();
        if(!sale.isPaid())
        {
          clientDebt += calculatePrice(sale.getId());
        }
      }
    }

    return _balance + clientDebt;
  }

  /**
   * Checks if product belongs to given supplier
   * @param productId product whose supplier we want to verify
   * @param supplierId supplier that might offer the product
   * @return true if product has same supplier id as supplierId
   */
  boolean productSupplierVerify(String productId, String supplierId)
  {
    return _products.get(productId).getSupplierId().equalsIgnoreCase(supplierId);
  }

   /**
   * Puts all existing clients as observers for the new product
   * @param p Product to register observers
   */
  void newProductNotifications(Product p)
  {
    for(Map.Entry<String, Client> entry : _clients.entrySet())
    {
      p.registerObserver(entry.getValue());
    }
  }

  /**
   * Puts new client as an observer for all existing products
   * @param c observer to add
   */
  void newClientNotifications(Client c)
  {
    for(Map.Entry<String, Product> entry : _products.entrySet())
    {
      entry.getValue().registerObserver(c);
    }
  }

  /**
   * Matches a pattern and creates and registers it if it's recognized
   * @param fields parsed fields from imported file
   * @throws BadEntryException
   */
  void registerFromFields(String[] fields) throws BadEntryException
  {
    Pattern clientPattern = Pattern.compile("^(CLIENT)");
    Pattern supplierPattern = Pattern.compile("^(SUPPLIER)");
    Pattern boxPattern = Pattern.compile("^(BOX)");
    Pattern containerPattern = Pattern.compile("^(CONTAINER)");
    Pattern bookPattern = Pattern.compile("^(BOOK)");

    if(clientPattern.matcher(fields[0]).matches())
    {
      String id = fields[1];
      String name = fields[2];
      String address = fields[3];
      registerClient(id, name, address);
    }
    else if(supplierPattern.matcher(fields[0]).matches())
    {
      String id = fields[1];
      String name = fields[2];
      String address = fields[3];
      registerSupplier(id, name, address);
    }
    else if(boxPattern.matcher(fields[0]).matches())
    {
      String id = fields[1];
      String service = fields[2];
      String supplierId = fields[3];
      int price = Integer.parseInt(fields[4]);
      int minStock = Integer.parseInt(fields[5]);
      int stock = Integer.parseInt(fields[6]);

      //Requires a different constructor from usual register
      Product product = new Box(id, supplierId, price, minStock, service, stock);
      newProductNotifications(product);
      _products.put(id, product);
      
    }
    else if(containerPattern.matcher(fields[0]).matches())
    {
      String id = fields[1];
      String service = fields[2];
      String quality = fields[3];
      String supplierId = fields[4];
      int price = Integer.parseInt(fields[5]);
      int minStock = Integer.parseInt(fields[6]);
      int stock = Integer.parseInt(fields[7]);

      Product product = new Container(id, supplierId, price, minStock, service, quality, stock);
      newProductNotifications(product);
      _products.put(id, product);
      
    }
    else if(bookPattern.matcher(fields[0]).matches())
    {
      String id = fields[1];
      String title = fields[2];
      String author = fields[3];
      String isbn = fields[4];
      String supplierId = fields[5];
      int price = Integer.parseInt(fields[6]);
      int minStock = Integer.parseInt(fields[7]);
      int stock = Integer.parseInt(fields[8]);

      Product product = new Book(id, supplierId, price, minStock, title, author, isbn, stock);
      newProductNotifications(product);
      _products.put(id, product);
    }
    else
      throw new BadEntryException("Unknown entry:" + fields[0]);
  }
}

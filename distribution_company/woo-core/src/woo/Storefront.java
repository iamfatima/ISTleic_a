package woo;
import woo.exceptions.*;
import java.io.*;

/**
 * Storefront: façade for the core classes.
 */
public class Storefront {

  /** Current filename. */
  private String _filename = "";

  /** The actual store. */
  private Store _store = new Store();

  //FIXME define other attributes
  //FIXME define constructor(s)
  //FIXME define other methods

  /**
   * @throws IOException
   * @throws FileNotFoundException
   * @throws MissingFileAssociationException
   */
  public void save() throws IOException, FileNotFoundException, MissingFileAssociationException {
    ObjectOutputStream out = new ObjectOutputStream(new BufferedOutputStream(new FileOutputStream(_filename)));
    out.writeObject(_store);
    out.close();
  }

  /**
   * @param filename
   * @throws MissingFileAssociationException
   * @throws IOException
   * @throws FileNotFoundException
   */
  public void saveAs(String filename) throws MissingFileAssociationException, FileNotFoundException, IOException {
    _filename = filename;
    save();
  }

  /**
   * @param filename
   * @throws UnavailableFileException
   */
  public void load(String filename) throws UnavailableFileException, FileNotFoundException, IOException, ClassNotFoundException {
      ObjectInputStream in = new ObjectInputStream(new BufferedInputStream(new FileInputStream(filename)));
      _store = (Store) in.readObject();
      _filename = filename;
      in.close();
  }

  /**
   * @param textfile
   * @throws ImportFileException
   */
  public void importFile(String textfile) throws ImportFileException {
    try {
      _store.importFile(textfile);
    } catch (IOException | BadEntryException /* FIXME maybe other exceptions */ e) {
      throw new ImportFileException(textfile);
    }
  }

  //Client methods --------------------------------------------------------------------------

  public void registerClient(String id, String name, String address)
  {
    _store.registerClient(id, name, address);
  }

  public boolean containsClientKey(String id)
  {
    return _store.containsClientKey(id);
  }

  public String displayClient(String id)
  {
    return _store.displayClient(id);
  }

  public String displayClients()
  {
    return _store.displayClients();
  }

  public String displayClientTransactions(String id)
  {
    return _store.displayClientTransactions(id);
  }

  public boolean toggleNotifications(String clientId, String productId)
  {
    return _store.toggleNotifications(clientId, productId);
  }

  
  //Product methods --------------------------------------------------------------------------

  public void registerProductBox(String id, String supplierId, int price, int minStock, String service)
  {
    _store.registerProductBox(id, supplierId, price, minStock, service);
  }

  public void registerProductContainer(String id, String supplierId, int price, int minStock, String service, String quality)
  {
    _store.registerProductContainer(id, supplierId, price, minStock, service, quality);
  }

  public void registerProductBook(String id, String supplierId, int price, int minStock, String title, String author, String ISBN)
  {
    _store.registerProductBook(id, supplierId, price, minStock, title, author, ISBN);
  }

  public boolean containsProductKey(String id)
  {
    return _store.containsProductKey(id);
  }

  public String displayProducts()
  {
    return _store.displayProducts();
  }

  public boolean hasEnoughProduct(String productId, int amount)
  {
    return _store.hasEnoughProduct(productId, amount);
  }

  public int getExistingStock(String productId)
  {
    return _store.getExistingStock(productId);
  }

  public void changePrice(String productId, int price)
  {
    _store.changePrice(productId, price);
  }

  public String lessProduct()
  {
    return _store.lessProduct();
  }


  //Supplier methods --------------------------------------------------------------------------

  public void registerSupplier(String id, String name, String address)
  {
    _store.registerSupplier(id, name, address);
  }

  public boolean containsSupplierKey(String id)
  {
    return _store.containsSupplierKey(id);
  }

  public String displaySuppliers()
  {
    return _store.displaySuppliers();
  }

  public boolean isSupplierAvailable(String id)
  {
    return _store.isSupplierAvailable(id);
  }

  public boolean switchAvailability(String id)
  {
    return _store.switchAvailability(id);
  }

  public String showSupplierTransactions(String id)
  {
    return _store.showSupplierTransactions(id);
  }

  public String bestSupplier()
  {
    return _store.bestSupplier();
  }



  
  //Transaction methods --------------------------------------------------------------------------

  public String displayTransaction(int id)
  {
    return _store.displayTransaction(id);
  }

  public boolean containsTransactionKey(int id)
  {
    return _store.containsTransactionKey(id);
  }

  //Order methods --------------------------------------------------------------------------

  public void newOrder(String supplierId)
  {
    _store.newOrder(supplierId);
  }

  public void addToOrder(String productId, int amount)
  {
    _store.addToOrder(productId, amount);
  }

  public void registerOrder()
  {
    _store.registerOrder();
  }

  //Sale methods --------------------------------------------------------------------------

  public void registerSale(String clientId, int deadline, String productId, int amount)
  {
    _store.registerSale(clientId, deadline, productId, amount);
  }

  public void paySale(int transactionId)
  {
    _store.paySale(transactionId);
  }

  //Date methods --------------------------------------------------------------------------

  public void advanceDate(int days)
  {
    _store.advanceDate(days);
  }

  public int getDate()
  {
    return _store.getDate();
  }


  //Miscellaneous methods --------------------------------------------------------------------------

  public String getFilename()
  {
    return _filename;
  }

  public String lookupProductsByPrice(int topPrice)
  {
    return _store.lookupProductsByPrice(topPrice);
  }

  public String getClientPayments(String clientId)
  {
    return _store.getClientPayments(clientId);
  }

  public boolean validService(String service)
  {
    return _store.validService(service);
  }

  public boolean validQuality(String quality)
  {
    return _store.validQuality(quality);
  }

  public boolean productSupplierVerify(String productId, String supplierId)
  {
    return _store.productSupplierVerify(productId, supplierId);
  }

  public int getBalance()
  {
    return _store.getBalance();
  }

  public int getAccountingBalance()
  {
    return _store.getAccountingBalance();
  }
}

package woo;
import java.util.Map;
import java.util.TreeMap;

class Order extends Transaction {
    private String _supplierId;
    private TreeMap<String, Integer> _products = new TreeMap<String, Integer>();

    Order(int id, String supplierId, int date)
    {
        super(id, date, 0);
        this._supplierId = supplierId;
    }

    public void addProduct(String productId, int amount, int price)
    {
        _products.put(productId, amount);
        setPrice(getPrice() + price * amount);
    }

    public void removeProduct(String productId) throws NullPointerException
    {
        try {
            _products.remove(productId);
        }
        catch(NullPointerException e)
        {
            e.printStackTrace();
        }
    }

    public String getSupplierId()
    {
        return this._supplierId;
    }

    public TreeMap<String, Integer> getProducts()
    {
        return this._products;
    }

    @Override
    public String toString()
    {
        String string = "" + getId() + "|" + this._supplierId + "|" + getPrice() + "|" + getPaymentDate();
        
        for(Map.Entry<String, Integer> entry : _products.entrySet())
            string += "\n" + entry.getKey() + "|" + entry.getValue();
        
        return string;
    }
}

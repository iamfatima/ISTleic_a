package woo;

import java.io.Serializable;
import java.util.LinkedList;

class Supplier implements Serializable {
    private String _id;
    private String _name;
    private String _address;
    private LinkedList<Integer> _orders = new LinkedList<Integer>();
    private boolean _available;

    Supplier(String id, String name, String address)
    {
        this._id = id;
        this._name = name;
        this._address = address;
        this._available = true;
    }

    public void addTransaction(int id)
    {
        _orders.add(id);
    }

    //Getters
    public String getId()
    {
        return this._id;
    }

    public String getName()
    {
        return this._name;
    }

    public String getAddress()
    {
        return this._address;
    }

    public LinkedList<Integer> getTransactions()
    {
        return this._orders;
    }

    public boolean isAvailable()
    {
        return this._available;
    }

    public boolean switchAvailability()
    {
        this._available = !_available;
       return this._available;   
    }


    @Override
    public String toString()
    {
        return "" + _id + "|" + _name  + "|" + _address + "|" + _available;
    }
}

package woo;

import java.io.Serializable;
import java.util.ArrayList;

class Product implements Serializable, NotificationSubject {
    
    private ArrayList<NotificationObserver> _observers = new ArrayList<NotificationObserver>();
    private String _id;
    private int _stock;
    private int _minimumStock;
    private int _price;
    private String _supplierId;
    private int _deadlineOffset;
    private NotificationDelivery _delivery = new NormalDelivery();

    Product(String id, String supplierId, int price, int minimumStock, int offset)
    {
        this._id = id;
        this._minimumStock = minimumStock;
        this._stock = 0;
        this._price = price;
        this._supplierId = supplierId;
        this._deadlineOffset = offset;
    }

    Product(String id, String supplierId, int price, int minimumStock, int offset, int stock)
    {
        this._id = id;
        this._minimumStock = minimumStock;
        this._stock = stock;
        this._price = price;
        this._supplierId = supplierId;
        this._deadlineOffset = offset;
    }

    public void addStock(int n)
    {
        this._stock += n;
    }

    public void removeStock(int n)
    {
        this._stock -= n;
    }

    public String getId()
    {
        return this._id;
    }

    public int getPrice()
    {
        return this._price;
    }

    public String getSupplierId()
    {
        return this._supplierId;
    }

    public int getStock()
    {
        return this._stock;
    }

    public int getMinimumStock()
    {
        return this._minimumStock;
    }

    public int getDeadlineOffset()
    {
        return this._deadlineOffset;
    }

    public void setPrice(int price)
    {
        this._price = price;
    }

    public void setNotifDelivery(NotificationDelivery nd)
    {
        this._delivery = nd;
    }

    public boolean toggleNotifications(NotificationObserver ob)
    {
        if(_observers.contains(ob))
        {
            removeObserver(ob);
            return false;
        }
        else
        {
            registerObserver(ob);
            return true;
        }
    }

    @Override
    public void registerObserver(NotificationObserver ob)
    {
        _observers.add(ob);
    }

    @Override
    public void removeObserver(NotificationObserver ob)
    {
        _observers.remove(ob);
    }

    @Override
    public void notifyBargain()
    {
        for (NotificationObserver observer: _observers) {
            observer.updateBargain(this, _delivery);
        }
    }

    @Override
    public void notifyNew()
    {
        for (NotificationObserver observer: _observers) {
            observer.updateNew(this, _delivery);
        }
    }
}

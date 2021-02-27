package woo;

import java.io.Serializable;
import java.util.LinkedList;
import java.util.ArrayList;

class Client implements Serializable, NotificationObserver {
    private String _id;
    private String _name;
    private String _address;
    private int _points;
    private int _totalToPay;
    private int _totalPaid;
    private int _numTrans;
    private RankState _rank = new NormalRank();
    private LinkedList<Integer> _purchases = new LinkedList<Integer>();
    private ArrayList<String> _notifications = new ArrayList<String>();

    public Client(String id, String name, String address)
    {
        this._id = id;
        this._name = name;
        this._address = address;
        this._totalToPay = 0;
        this._totalPaid = 0;
        this._points = 0;
        this._numTrans = 0;
    }

    public int calculatePrice(PaymentPeriod p, int date, int deadline, int price)
    {
        return _rank.calculatePrice(p, date, deadline, price);
    }

    public int Pay(PaymentPeriod p, int date, int deadline, int price)
    {
        int paid = _rank.Pay(this, p, date, deadline, price);
        _totalPaid += paid;
        return paid;
    }

    public void updateStatus()
    {
        if(_points > 25000)
        {
            this.setRank(new EliteRank());
        }
        else if (_points > 2000)
        {
            this.setRank(new SelectionRank());
        }
    }

    public void setRank(RankState state)
    {
        this._rank = state;
    }

    public int getPoints()
    {
        return this._points;
    }

    public void setPoints(int n)
    {
        this._points = n;
    }

    public LinkedList<Integer> getPurchases()
    {
        return this._purchases;
    }

    public void makePurchase(int saleId, int price)
    {
        _purchases.add(saleId);
        this._totalToPay += price;
        this._numTrans += 1;
    }

    @Override
    public ArrayList<String> getNotifications()
    {
        ArrayList<String> notifs = this._notifications;
        wipeNotifications();
        return notifs;
    }

    @Override
    public void wipeNotifications()
    {
        this._notifications = new ArrayList<String>();
    }

    @Override
    public void updateBargain(Product p, NotificationDelivery delivery)
    {
        String notif = delivery.deliver(p);
        _notifications.add("BARGAIN|" + notif);
    }

    @Override
    public void updateNew(Product p, NotificationDelivery delivery)
    {
        String notif = delivery.deliver(p);
        _notifications.add("NEW|" + notif);
    }

    public String toString()
    {
        return "" + _id + "|" + _name + "|" + _address + "|" + _rank.getRank() + "|" + _totalToPay + "|" + _totalPaid + "|" +_numTrans;
    }
}

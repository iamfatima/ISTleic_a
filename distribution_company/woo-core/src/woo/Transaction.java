package woo;

import java.io.Serializable;

abstract class Transaction implements Serializable {
    private int _id;
    private int _paymentDate;
    private int _basePrice;

    Transaction(int id, int paymentDate, int basePrice)
    {
        this._id = id;
        this._paymentDate = paymentDate;
        this._basePrice = basePrice;
    }

    public int getId()
    {
        return this._id;
    }

    public int getPaymentDate()
    {
        return this._paymentDate;
    }

    public int getPrice()
    {
        return this._basePrice;
    }

    public void setPrice(int n)
    {
        this._basePrice = n;
    }
}

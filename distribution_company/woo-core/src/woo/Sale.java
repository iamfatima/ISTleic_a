package woo;

class Sale extends Transaction {
    private String _clientId;
    private int _deadline;
    private String _productId;
    private int _amount;
    private int _priceToPay;
    private boolean _paid = false;
    private int _paymentDate;
    
    Sale(int id, String clientId, int deadline, String productId, int amount, int price, int date)
    {
        super(id, date, price*amount);
        this._clientId = clientId;
        this._deadline = deadline;
        this._productId = productId;
        this._amount = amount;
        this._priceToPay = price*amount;
    }

    //Getters and Setters ---------------------------------------------------------------
    public int getDeadline()
    {
        return this._deadline;
    }

    public int getAmount()
    {
        return this._amount;
    }

    public String getClientId()
    {
        return this._clientId;
    }

    public String getProductId()
    {
        return this._productId;
    }

    public void setPriceToPay(int n)
    {
        this._priceToPay = n;
    }

    public boolean isPaid()
    {
        return this._paid;
    }

    public void Pay(int date, int paid)
    {
        this._paid = true;
        this._paymentDate = date;
        this._priceToPay = paid;
    }

    @Override
    public String toString()
    {
        if(_paid)
            return "" + getId() + "|" + _clientId + "|" + _productId + "|" + _amount + "|" + getPrice() + "|" + _priceToPay + "|" + _deadline + "|" + _paymentDate;
        else
            return "" + getId() + "|" + _clientId + "|" + _productId + "|" + _amount + "|" + getPrice() + "|" + _priceToPay + "|" + _deadline;
    }
}

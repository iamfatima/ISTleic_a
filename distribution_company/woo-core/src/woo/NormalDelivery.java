package woo;

class NormalDelivery extends NotificationDelivery {
    public String deliver(Product p)
    {
        return "" + p.getId() + "|" + p.getPrice();
    }
}

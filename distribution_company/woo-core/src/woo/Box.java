package woo;

class Box extends Product {
    private Service _service;

    Box(String id, String supplierId, int price, int minimumStock, String service)
    {
        super(id, supplierId, price, minimumStock, 5);
        _service = Service.valueOf(service);
    }

    Box(String id,  String supplierId, int price, int minimumStock, String service, int stock)
    {
        super(id, supplierId, price, minimumStock, 5, stock);
        _service = Service.valueOf(service);
    }

    public String toString()
    {
        return "BOX|" + getId() + "|" + getSupplierId() + "|" + getPrice() + "|" + getMinimumStock() + "|" + getStock() + "|" + _service.toString();
    }
}

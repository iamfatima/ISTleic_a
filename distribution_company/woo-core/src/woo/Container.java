package woo;

class Container extends Product {
    private Service _service;
    private Quality _quality;

    Container(String id, String supplierId, int price, int minimumStock, String service, String quality)
    {
        super(id, supplierId, price, minimumStock, 8);
        _service = Service.valueOf(service);
        _quality = Quality.valueOf(quality);
    }

    Container(String id, String supplierId, int price, int minimumStock, String service, String quality, int stock)
    {
        super(id, supplierId, price, minimumStock, 8, stock);
        _service = Service.valueOf(service);
        _quality = Quality.valueOf(quality);
    }

    public String toString()
    {
        return "CONTAINER|" + getId() + "|" + getSupplierId() + "|" + getPrice() + "|" + getMinimumStock()  + "|" + getStock() + "|" + _service.toString() + "|" + _quality.toString();
    }
}

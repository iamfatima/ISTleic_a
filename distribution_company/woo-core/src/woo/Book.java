package woo;

class Book extends Product {
    private String _title;
    private String _author;
    private String _ISBN;

    Book(String id, String supplierId, int price, int minimumStock, String title, String author, String ISBN)
    {
        super(id, supplierId, price, minimumStock, 3);
        this._title = title;
        this._author = author;
        this._ISBN = ISBN;
    }

    Book(String id, String supplierId, int price, int minimumStock, String title, String author, String ISBN, int stock)
    {
        super(id, supplierId, price, minimumStock, 3, stock);
        this._title = title;
        this._author = author;
        this._ISBN = ISBN;
    }

    public String toString()
    {
        return "BOOK|" + getId() + "|" + getSupplierId() + "|" + getPrice() + "|" + getMinimumStock() + "|" + getStock() + "|" + _title + "|" + _author + "|" + _ISBN ;
    }
}

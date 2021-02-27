package woo;
import java.io.Serializable;

abstract class NotificationDelivery implements Serializable {
    public abstract String deliver(Product p);
}

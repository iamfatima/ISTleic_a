package woo;
import java.io.Serializable;
import java.util.ArrayList;

interface NotificationObserver extends Serializable {
    public void updateBargain(Product p, NotificationDelivery delivery);
    public void updateNew(Product p, NotificationDelivery delivery);
    public void wipeNotifications();
    public ArrayList<String> getNotifications();
}

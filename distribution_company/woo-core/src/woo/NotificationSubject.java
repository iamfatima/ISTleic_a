package woo;
import java.io.Serializable;

interface NotificationSubject extends Serializable {
    public void registerObserver(NotificationObserver ob);
    public void removeObserver(NotificationObserver ob);
    public void notifyBargain();
    public void notifyNew();
}

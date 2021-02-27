package woo;
import woo.exceptions.MaxRankException;
import woo.exceptions.RootRankException;
import java.io.Serializable;

interface RankState extends Serializable {
    void previousRank(Client client) throws RootRankException;
    void nextRank(Client client) throws MaxRankException;
    int calculatePrice(PaymentPeriod p, int date, int deadline, int price);
    int Pay(Client client, PaymentPeriod p, int date, int deadline, int price);
    String getRank();
}

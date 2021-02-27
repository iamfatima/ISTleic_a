package woo;
import woo.exceptions.RootRankException;

class NormalRank implements RankState {

    @Override
    public void previousRank(Client client) throws RootRankException
    {
        throw new RootRankException("Client currently in bottom rank.");
    }
    
    @Override
    public void nextRank(Client client)
    {
        client.setRank(new SelectionRank());
    }

    @Override
    public int calculatePrice(PaymentPeriod p, int date, int deadline, int price)
    {
        switch(p)
        {
            case P1:
                return (int) (price * 0.9);
                
            case P2:
                return price;

            case P3:
                return (int) (price * (1 + (0.05 * (date-deadline) )));

            case P4:
                return (int) (price * (1 + (0.1 * (date-deadline) )));

            default:
                return 0;
        }
    }

    @Override
    public int Pay(Client client, PaymentPeriod p, int date, int deadline, int price)
    {
        switch(p)
        {
            case P1:
            case P2:
                int pricePaid = calculatePrice(p, date, deadline, price);
                client.setPoints(client.getPoints() + (pricePaid * 10));
                client.updateStatus();
                return pricePaid;
            case P3:
            case P4:
                return calculatePrice(p, date, deadline, price);

            default:
                return 0;
        }
    }

    @Override
    public String getRank()
    {
        return "NORMAL";
    }
}
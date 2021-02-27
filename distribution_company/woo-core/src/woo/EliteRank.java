package woo;
import woo.exceptions.MaxRankException;

class EliteRank implements RankState {

    @Override
    public void previousRank(Client client)
    {
        client.setRank(new SelectionRank());
    }
    
    @Override
    public void nextRank(Client client) throws MaxRankException
    {
        throw new MaxRankException("Client is already at max rank.");
    }

    @Override
    public int calculatePrice(PaymentPeriod p, int date, int deadline, int price)
    {
        switch(p)
        {
            case P1:
                return (int) (price * 0.9);
                
            case P2:
                return (int) (price * 0.9);

            case P3:
                return (int) (price * 0.95);

            case P4:
                return price;

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
                return pricePaid;
            case P3:
            case P4:
                if(date - deadline > 15)
                {
                    client.setPoints((int) (client.getPoints() * 0.25));
                    previousRank(client);
                    return calculatePrice(p, date, deadline, price);
                }
                else
                    return calculatePrice(p, date, deadline, price);
            default:
                return 0;
        }
    }

    @Override
    public String getRank()
    {
        return "ELITE";
    }
}
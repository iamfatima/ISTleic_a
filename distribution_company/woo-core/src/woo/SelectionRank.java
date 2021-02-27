package woo;

class SelectionRank implements RankState {

    @Override
    public void previousRank(Client client)
    {
        client.setRank(new NormalRank());
    }
    
    @Override
    public void nextRank(Client client)
    {
        client.setRank(new EliteRank());
    }

    @Override
    public int calculatePrice(PaymentPeriod p, int date, int deadline, int price)
    {
        switch(p)
        {
            case P1:
                return (int) (price * 0.9);
                
            case P2:
                if(deadline - date >= 2)
                    return (int) (price * 0.95);
                else
                    return price;

            case P3:
                if(date - deadline > 1)
                    return (int) (price * (1 + (0.02 * (date - deadline))));
                else
                    return price;

            case P4:
                return (int) (price * (1 + (0.05 * (date-deadline))));

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
                if(date - deadline > 2)
                {
                    client.setPoints((int) (client.getPoints() * 0.1));
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
        return "SELECTION";
    }
}

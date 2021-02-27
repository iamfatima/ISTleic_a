package woo;

enum Service {
    NORMAL, AIR, EXPRESS, PERSONAL;

    public boolean contains(String string)
    {
        for(Service service : Service.values())
        {
            if(service.name().equals(string))
                return true;
        }
        
        return false;
    }
}

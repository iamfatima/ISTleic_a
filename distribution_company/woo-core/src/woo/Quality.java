package woo;

enum Quality {
    B4, C4, C5, DL;
    
    public boolean contains(String string)
    {
        for(Quality quality : Quality.values())
        {
            if(quality.toString().equals(string))
                return true;
        }

        return false;
    }
}
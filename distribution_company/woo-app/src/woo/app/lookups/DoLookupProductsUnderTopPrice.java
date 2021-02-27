package woo.app.lookups;

import pt.tecnico.po.ui.Command;     
import pt.tecnico.po.ui.DialogException; 
import pt.tecnico.po.ui.Input;
import woo.Storefront;               
//FIXME import other classes

/**
 * Lookup products cheaper than a given price.
 */
public class DoLookupProductsUnderTopPrice extends Command<Storefront> {

  private Input<Integer> _priceLimit;

  public DoLookupProductsUnderTopPrice(Storefront storefront) {
    super(Label.PRODUCTS_UNDER_PRICE, storefront);
    _priceLimit = _form.addIntegerInput(Message.requestPriceLimit());
  }

  @Override
  public void execute() throws DialogException 
  {
    _form.parse();

    _display.add(_receiver.lookupProductsByPrice(_priceLimit.value()));
    _display.display();
  }
}

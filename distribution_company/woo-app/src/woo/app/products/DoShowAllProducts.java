package woo.app.products;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;           
import pt.tecnico.po.ui.Input;                    
import woo.Storefront;                    
//FIXME import other classes

/**
 * Show all products.
 */
public class DoShowAllProducts extends Command<Storefront> {

  private String allProducts;

  public DoShowAllProducts(Storefront receiver) {
    super(Label.SHOW_ALL_PRODUCTS, receiver);
    allProducts = "";
  }

  @Override
  public final void execute() throws DialogException {
    allProducts = _receiver.displayProducts();
    _display.add(allProducts);
    _display.display();
  }

}

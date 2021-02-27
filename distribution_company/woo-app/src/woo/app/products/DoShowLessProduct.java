package woo.app.products;

import pt.tecnico.po.ui.Command;                
import woo.Storefront;                    

public class DoShowLessProduct extends Command<Storefront> {

  private String lessProduct;

  public DoShowLessProduct(Storefront receiver) 
  {
    super(Label.LESS_PRODUCT, receiver);
    lessProduct = "";
  }

  @Override
  public final void execute() {
    _form.parse();
    lessProduct = _receiver.lessProduct();
    _display.add(lessProduct);
    _display.display();
  }

}
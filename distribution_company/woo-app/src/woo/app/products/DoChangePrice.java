package woo.app.products;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;
import woo.app.products.Message;
import woo.app.exceptions.UnknownProductKeyException;                                                                                                                        

/**
 * Change product price.
 */
public class DoChangePrice extends Command<Storefront> {

  private Input<String> _productId;
  private Input<Integer> _price;
  
  public DoChangePrice(Storefront receiver) {
    super(Label.CHANGE_PRICE, receiver);
    _productId = _form.addStringInput(Message.requestProductKey());
    _price = _form.addIntegerInput(Message.requestPrice());
  }

  @Override
  public final void execute() throws DialogException, UnknownProductKeyException {
    _form.parse();
    if(!_receiver.containsProductKey(_productId.value()))
      throw new UnknownProductKeyException(_productId.value());
    
    _receiver.changePrice(_productId.value(), _price.value());
  }
}

package woo.app.products;

import pt.tecnico.po.ui.Command;  
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        
import woo.app.exceptions.DuplicateProductKeyException;
import woo.app.exceptions.UnknownSupplierKeyException;
import woo.app.exceptions.UnknownServiceTypeException;

/**
 * Register box.
 */
public class DoRegisterProductBox extends Command<Storefront> {

  private Input<String> _id;
  private Input<String> _supplierId;
  private Input<Integer> _price;
  private Input<Integer> _minStock;
  private Input<String> _service;


  public DoRegisterProductBox(Storefront receiver) {
    super(Label.REGISTER_BOX, receiver);
    _id = _form.addStringInput(Message.requestProductKey());
    _price = _form.addIntegerInput(Message.requestPrice());
    _minStock = _form.addIntegerInput(Message.requestStockCriticalValue());
    _supplierId = _form.addStringInput(Message.requestSupplierKey());
    _service = _form.addStringInput(Message.requestServiceType());
  }

  @Override
  public final void execute() throws DialogException, UnknownServiceTypeException, DuplicateProductKeyException, UnknownSupplierKeyException {
    _form.parse();
    
    if(_receiver.containsProductKey(_id.value()))
      throw new DuplicateProductKeyException(_id.value());
    else if(!_receiver.containsSupplierKey(_supplierId.value()))
      throw new UnknownSupplierKeyException(_supplierId.value());
    else if(_receiver.validService(_service.value()))
    {
      _receiver.registerProductBox(_id.value(), _supplierId.value(), _price.value(), 
      _minStock.value(),_service.value());
    }
    else
      throw new UnknownServiceTypeException(_service.value());
  }
}

package woo.app.products;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;
import woo.app.exceptions.DuplicateProductKeyException;
import woo.app.exceptions.UnknownSupplierKeyException;
import woo.app.exceptions.UnknownServiceTypeException;
import woo.app.exceptions.UnknownServiceLevelException;
import woo.Storefront;                                                                                                                        

/**
 * Register container.
 */
public class DoRegisterProductContainer extends Command<Storefront> {

  private Input<String> _id;
  private Input<String> _supplierId;
  private Input<Integer> _price;
  private Input<Integer> _minStock;
  private Input<String> _service;
  private Input<String> _quality;

  public DoRegisterProductContainer(Storefront receiver) {
    super(Label.REGISTER_CONTAINER, receiver);
    _id = _form.addStringInput(Message.requestProductKey());
    _price = _form.addIntegerInput(Message.requestPrice());
    _minStock = _form.addIntegerInput(Message.requestStockCriticalValue());
    _supplierId = _form.addStringInput(Message.requestSupplierKey());
    _service = _form.addStringInput(Message.requestServiceType());
    _quality = _form.addStringInput(Message.requestServiceLevel());
  }

  @Override
  public final void execute() throws DialogException, UnknownServiceTypeException, UnknownServiceLevelException, DuplicateProductKeyException, UnknownSupplierKeyException {
    _form.parse();
    
    if(_receiver.containsProductKey(_id.value()))
      throw new DuplicateProductKeyException(_id.value());
    else if(!_receiver.containsSupplierKey(_supplierId.value()))
      throw new UnknownSupplierKeyException(_supplierId.value());
    else if(!_receiver.validService(_service.value()))
      throw new UnknownServiceTypeException(_service.value());
    else if(!_receiver.validQuality(_quality.value()))
      throw new UnknownServiceLevelException(_quality.value());
    else
    {
      _receiver.registerProductContainer(_id.value(), _supplierId.value(), _price.value(), _minStock.value(), _service.value(), _quality.value());
    }
  }
}

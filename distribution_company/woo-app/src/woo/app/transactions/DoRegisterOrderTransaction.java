package woo.app.transactions;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;
import pt.tecnico.po.ui.Form; 
import woo.app.exceptions.UnknownSupplierKeyException;
import woo.app.exceptions.UnknownProductKeyException;         
import woo.app.exceptions.UnauthorizedSupplierException;
import woo.app.exceptions.WrongSupplierException;                                                                                                       

/**
 * Register order.
 */
public class DoRegisterOrderTransaction extends Command<Storefront> {

  private Input<String> _supplierId;
  private Input<String> _productId;
  private Input<Integer> _amount;
  private Input<Boolean> _more;
  private boolean alreadyCreated = false;

  public DoRegisterOrderTransaction(Storefront receiver) {
    super(Label.REGISTER_ORDER_TRANSACTION, receiver);
    _supplierId = _form.addStringInput(Message.requestSupplierKey());
  }

  @Override
  public final void execute() throws DialogException, UnknownSupplierKeyException, UnknownProductKeyException, UnauthorizedSupplierException, WrongSupplierException {
    _form.parse();

    _receiver.newOrder(_supplierId.value());

    do
    {
      Form _moreForm = new Form();
      _productId = _moreForm.addStringInput(Message.requestProductKey());
      _amount = _moreForm.addIntegerInput(Message.requestAmount());
      _more = _moreForm.addBooleanInput(Message.requestMore());
      _moreForm.parse();
      
      if(!_receiver.containsSupplierKey(_supplierId.value()))
        throw new UnknownSupplierKeyException(_supplierId.value());
      else if(!_receiver.isSupplierAvailable(_supplierId.value()))
        throw new UnauthorizedSupplierException(_supplierId.value());
      else if(!_receiver.containsProductKey(_productId.value()))
        throw new UnknownProductKeyException(_productId.value());
      else if(!_receiver.productSupplierVerify(_productId.value(), _supplierId.value()))
        throw new WrongSupplierException(_supplierId.value(), _productId.value());
      else
      {
        if(!alreadyCreated)
        {
          _receiver.newOrder(_supplierId.value());
          alreadyCreated = true;
        }
        _receiver.addToOrder(_productId.value(), _amount.value());
      }

    }while(_more.value());

    _receiver.registerOrder();
  }
}

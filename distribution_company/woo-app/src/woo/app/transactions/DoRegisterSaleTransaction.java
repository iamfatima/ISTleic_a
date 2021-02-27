package woo.app.transactions;

import pt.tecnico.po.ui.Command; 
import pt.tecnico.po.ui.DialogException;  
import pt.tecnico.po.ui.Input;        
import woo.Storefront; 
import woo.app.exceptions.UnknownClientKeyException;
import woo.app.exceptions.UnknownProductKeyException;
import woo.app.exceptions.UnavailableProductException;


/**
 * Register sale.
 */
public class DoRegisterSaleTransaction extends Command<Storefront> {

  private Input<String> _clientId;
  private Input<Integer> _deadline;
  private Input<String> _productId;
  private Input<Integer> _amount;
  private int _storeAmount;

  public DoRegisterSaleTransaction(Storefront receiver) {
    super(Label.REGISTER_SALE_TRANSACTION, receiver);
    _clientId = _form.addStringInput(Message.requestClientKey());
    _deadline = _form.addIntegerInput(Message.requestPaymentDeadline());
    _productId = _form.addStringInput(Message.requestProductKey());
    _amount = _form.addIntegerInput(Message.requestAmount());
    
  }

  @Override
  public final void execute() throws DialogException, UnknownClientKeyException, UnknownProductKeyException, UnavailableProductException {
    _form.parse();

    if (!_receiver.containsClientKey(_clientId.value())) 
      throw new UnknownClientKeyException(_clientId.value());
    else if(!_receiver.containsProductKey(_productId.value()))
      throw new UnknownProductKeyException(_productId.value());
    else if(!_receiver.hasEnoughProduct(_productId.value(),_amount.value()))
    {
      _storeAmount = _receiver.getExistingStock(_productId.value());
      throw new UnavailableProductException(_productId.value(),_amount.value(), _storeAmount);
    }

    _receiver.registerSale(_clientId.value(),_deadline.value(),_productId.value(),_amount.value());

    
  }

}

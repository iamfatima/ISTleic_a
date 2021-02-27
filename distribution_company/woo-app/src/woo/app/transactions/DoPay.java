package woo.app.transactions;

import pt.tecnico.po.ui.Command; 
import pt.tecnico.po.ui.DialogException;       
import pt.tecnico.po.ui.Input;       
import woo.Storefront;       
import woo.app.exceptions.UnknownTransactionKeyException; 
//FIXME import other classes

/**
 * Pay transaction (sale).
 */
public class DoPay extends Command<Storefront> {

  private Input<Integer> _saleId;

  
  public DoPay(Storefront storefront) {
    super(Label.PAY, storefront);
    _saleId = _form.addIntegerInput(Message.requestTransactionKey());
  }

  @Override
  public final void execute() throws DialogException, UnknownTransactionKeyException {
    _form.parse();

    if(!_receiver.containsTransactionKey(_saleId.value()))
      throw new UnknownTransactionKeyException(_saleId.value());

    _receiver.paySale(_saleId.value());
    
  }

}

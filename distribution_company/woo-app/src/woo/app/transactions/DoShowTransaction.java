package woo.app.transactions;

import pt.tecnico.po.ui.Command;   
import pt.tecnico.po.ui.DialogException; 
import pt.tecnico.po.ui.Input; 
import woo.Storefront;  
import woo.app.exceptions.UnknownTransactionKeyException;

/**
 * Show specific transaction.
 */
public class DoShowTransaction extends Command<Storefront> {

  private Input<Integer> _transactionId;

  public DoShowTransaction(Storefront receiver) {
    super(Label.SHOW_TRANSACTION, receiver);
    _transactionId = _form.addIntegerInput(Message.requestTransactionKey());
  }

  @Override
  public final void execute() throws DialogException, UnknownTransactionKeyException {
    _form.parse();

    if(!_receiver.containsTransactionKey(_transactionId.value()))
      throw new UnknownTransactionKeyException(_transactionId.value());

    _display.addLine(_receiver.displayTransaction(_transactionId.value()));
    _display.display();
  }

}

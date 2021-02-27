package woo.app.suppliers;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;
import woo.app.exceptions.UnknownSupplierKeyException;                                                                                                                        

/**
 * Enable/disable supplier transactions.
 */
public class DoToggleTransactions extends Command<Storefront> {

  private Input<String> _supplierId;

  public DoToggleTransactions(Storefront receiver) {
    super(Label.TOGGLE_TRANSACTIONS, receiver);
    _supplierId = _form.addStringInput(Message.requestSupplierKey());
  }

  @Override
  public void execute() throws DialogException, UnknownSupplierKeyException {
    _form.parse();
    if(!_receiver.containsSupplierKey(_supplierId.value()))
      throw new UnknownSupplierKeyException(_supplierId.value());

    if(_receiver.switchAvailability(_supplierId.value()))
      _display.addLine(Message.transactionsOn(_supplierId.value()));
    else
      _display.addLine(Message.transactionsOff(_supplierId.value()));

    _display.display();
  } 

}

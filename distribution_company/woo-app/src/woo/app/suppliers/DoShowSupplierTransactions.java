package woo.app.suppliers;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;  
import woo.app.exceptions.UnknownSupplierKeyException;                                                                                                                      


/**
 * Show all transactions for specific supplier.
 */
public class DoShowSupplierTransactions extends Command<Storefront> {

  private Input<String> _supplierId;

  public DoShowSupplierTransactions(Storefront receiver) {
    super(Label.SHOW_SUPPLIER_TRANSACTIONS, receiver);
    _supplierId = _form.addStringInput(Message.requestSupplierKey());
  }

  @Override
  public void execute() throws DialogException, UnknownSupplierKeyException {
    _form.parse();

    if(!_receiver.containsSupplierKey(_supplierId.value()))
      throw new UnknownSupplierKeyException(_supplierId.value());

    _display.add(_receiver.showSupplierTransactions(_supplierId.value()));
    _display.display();
  }

}

package woo.app.lookups;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException; 
import pt.tecnico.po.ui.Input;
import woo.app.exceptions.UnknownClientKeyException;                                                                                                                import woo.Storefront;                                                                                                                        //FIXME import other classes

/**
 * Lookup payments by given client.
 */
public class DoLookupPaymentsByClient extends Command<Storefront> {

  private Input<String> _clientId;

  public DoLookupPaymentsByClient(Storefront storefront) {
    super(Label.PAID_BY_CLIENT, storefront);
    _clientId = _form.addStringInput(Message.requestClientKey());

  }

  @Override
  public void execute() throws DialogException, UnknownClientKeyException {
    _form.parse();

    if(!_receiver.containsClientKey(_clientId.value()))
      throw new UnknownClientKeyException(_clientId.value());
    
    _display.add(_receiver.getClientPayments(_clientId.value()));
    _display.display();
  }

}

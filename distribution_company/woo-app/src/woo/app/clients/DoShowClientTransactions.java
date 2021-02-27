package woo.app.clients;

import pt.tecnico.po.ui.Command;   
import pt.tecnico.po.ui.DialogException;   
import pt.tecnico.po.ui.Input;      
import woo.Storefront; 
import woo.app.exceptions.UnknownClientKeyException;
//FIXME import other classes

/**
 * Show all transactions for a specific client.
 */
public class DoShowClientTransactions extends Command<Storefront> 
{

  private Input<String> _clientId;

  public DoShowClientTransactions(Storefront storefront) {
    super(Label.SHOW_CLIENT_TRANSACTIONS, storefront);
    _clientId = _form.addStringInput(Message.requestClientKey());
  }

  @Override
  public void execute() throws DialogException, UnknownClientKeyException  {
    _form.parse();
    if(!_receiver.containsClientKey(_clientId.value()))
      throw new UnknownClientKeyException(_clientId.value());
    else
    {
      _display.add(_receiver.displayClientTransactions(_clientId.value()));
      _display.display();
    }
  }
}

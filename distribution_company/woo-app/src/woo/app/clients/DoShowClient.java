package woo.app.clients;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        
import woo.app.exceptions.UnknownClientKeyException;

/**
 * Show client.
 */
public class DoShowClient extends Command<Storefront> {

  private Input<String> id;
  private String clientInfo;

  public DoShowClient(Storefront storefront) {
    super(Label.SHOW_CLIENT, storefront);
    id = _form.addStringInput(Message.requestClientKey());
  }

  @Override
  public void execute() throws DialogException, UnknownClientKeyException {
    _form.parse();
    if(!_receiver.containsClientKey(id.value()))
      throw new UnknownClientKeyException(id.value());
    else
      clientInfo = _receiver.displayClient(id.value());

    _display.add(clientInfo);
    _display.display();
  }

}

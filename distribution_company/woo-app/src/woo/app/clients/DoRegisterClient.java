package woo.app.clients;

import pt.tecnico.po.ui.Command;           
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        
import woo.app.exceptions.DuplicateClientKeyException;

/**
 * Register new client.
 */
public class DoRegisterClient extends Command<Storefront> {

  //FIXME add input fields
  private Input<String> id;
  private Input<String> name;
  private Input<String> address;

  public DoRegisterClient(Storefront storefront) {
    super(Label.REGISTER_CLIENT, storefront);
    id = _form.addStringInput(Message.requestClientKey());
    name = _form.addStringInput(Message.requestClientName());
    address = _form.addStringInput(Message.requestClientAddress());
  }

  @Override
  public void execute() throws DialogException, DuplicateClientKeyException {
  
    _form.parse();
    if(_receiver.containsClientKey(id.value()))
      throw new DuplicateClientKeyException(id.value());
    else
      _receiver.registerClient(id.value(), name.value(), address.value());
  }

}

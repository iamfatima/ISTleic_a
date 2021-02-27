package woo.app.clients;

import pt.tecnico.po.ui.Command;                   
import pt.tecnico.po.ui.DialogException;   
import pt.tecnico.po.ui.Input;                 
import woo.Storefront;     
import woo.app.exceptions.UnknownClientKeyException;
import woo.app.exceptions.UnknownProductKeyException;
import woo.app.clients.Message;

/**
 * Toggle product-related notifications.
 */
public class DoToggleProductNotifications extends Command<Storefront> {

  private Input<String> _clientId;
  private Input<String> _productId;

  public DoToggleProductNotifications(Storefront storefront) {
    super(Label.TOGGLE_PRODUCT_NOTIFICATIONS, storefront);
    _clientId = _form.addStringInput(Message.requestClientKey());
    _productId = _form.addStringInput(Message.requestProductKey());
  }

  @Override
  public void execute() throws DialogException,UnknownClientKeyException, UnknownProductKeyException {
    _form.parse();

    if(!_receiver.containsClientKey(_clientId.value()))
      throw new UnknownClientKeyException(_clientId.value());
    else if(!_receiver.containsProductKey(_productId.value()))
      throw new UnknownProductKeyException(_productId.value());
    
    if(_receiver.toggleNotifications(_clientId.value(), _productId.value()))
      _display.addLine(Message.notificationsOn(_clientId.value(), _productId.value()));
    else
      _display.addLine(Message.notificationsOff(_clientId.value(), _productId.value()));

    _display.display();
  }

}

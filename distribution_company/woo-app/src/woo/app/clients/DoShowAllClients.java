package woo.app.clients;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        

/**
 * Show all clients.
 */
public class DoShowAllClients extends Command<Storefront> {

  private String allClients;

  public DoShowAllClients(Storefront storefront) {
    super(Label.SHOW_ALL_CLIENTS, storefront);
    allClients = "";
  }

  @Override
  public void execute() throws DialogException {
    allClients = _receiver.displayClients();
    _display.add(allClients);
    _display.display();
  }
}

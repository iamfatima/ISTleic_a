package woo.app.suppliers;

import pt.tecnico.po.ui.Command;                                                                                                              import pt.tecnico.po.ui.DialogException;                                                                                                      import pt.tecnico.po.ui.Input;                                                                                                                import woo.Storefront;                                                                                                                        //FIXME import other classes

/**
 * Show all suppliers.
 */
public class DoShowSuppliers extends Command<Storefront> {

  private String allSuppliers;

  public DoShowSuppliers(Storefront receiver) {
    super(Label.SHOW_ALL_SUPPLIERS, receiver);
    allSuppliers = "";
  }

  @Override
  public void execute() throws DialogException {
    allSuppliers = _receiver.displaySuppliers();
    allSuppliers = allSuppliers.replace("true", Message.yes());
    allSuppliers = allSuppliers.replace("false", Message.no());
    _display.add(allSuppliers);
    _display.display();
  }
}

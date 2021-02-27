package woo.app.suppliers;
import woo.Storefront; 
import pt.tecnico.po.ui.Command;                                                                                                              import pt.tecnico.po.ui.DialogException;                                                                                                      import pt.tecnico.po.ui.Input;                                                                                                                import woo.Storefront;                                                                                                                        //FIXME import other classes

/**
 * Show all suppliers.
 */
public class DoShowBestSupplier extends Command<Storefront> {

  private String bestSupplier;

  public DoShowBestSupplier(Storefront receiver) {
    super(Label.SHOW_BEST_SUPPLIER, receiver);
    bestSupplier = "";
  }

  @Override
  public void execute() {
    bestSupplier = _receiver.bestSupplier();
    _display.add(bestSupplier);
    _display.display();
  }
}
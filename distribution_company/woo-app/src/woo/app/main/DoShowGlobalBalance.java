package woo.app.main;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;    
import pt.tecnico.po.ui.Input;     
import woo.Storefront;        
//FIXME import other classes

/**
 * Show global balance.
 */
public class DoShowGlobalBalance extends Command<Storefront> {

  private int balance;
  private int accounting;

  public DoShowGlobalBalance(Storefront receiver) {
    super(Label.SHOW_BALANCE, receiver);
  }

  @Override
  public final void execute() {

    balance = _receiver.getBalance();
    accounting = _receiver.getAccountingBalance();
    _display.addLine(Message.currentBalance(balance, accounting));
    _display.display();
  }
}

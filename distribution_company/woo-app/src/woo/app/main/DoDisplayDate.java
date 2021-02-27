package woo.app.main;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        

/**
 * Show current date.
 */
public class DoDisplayDate extends Command<Storefront> {

  private int date;

  public DoDisplayDate(Storefront receiver) {
    super(Label.SHOW_DATE, receiver);
    this.date = 0;
  }

  @Override
  public final void execute() throws DialogException {
    date = _receiver.getDate();
    _display.addLine(Message.currentDate(date));
    _display.display();
  }
}

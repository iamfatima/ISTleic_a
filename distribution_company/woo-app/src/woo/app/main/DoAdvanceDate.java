package woo.app.main;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;                
import woo.app.exceptions.InvalidDateException;                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        

/**
 * Advance current date.
 */
public class DoAdvanceDate extends Command<Storefront> {
  
  private Input<Integer> days;

  public DoAdvanceDate(Storefront receiver) {
    super(Label.ADVANCE_DATE, receiver);
    days = _form.addIntegerInput(Message.requestDaysToAdvance());
  }

  @Override
  public final void execute() throws DialogException {
      _form.parse();
      if(days.value() <= 0)
        throw new InvalidDateException(days.value());
      else
        _receiver.advanceDate(days.value());
  }
}

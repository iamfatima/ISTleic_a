package woo.app.main;

import java.io.*;
import woo.exceptions.MissingFileAssociationException;
import pt.tecnico.po.ui.Command;                                                                                                              import pt.tecnico.po.ui.DialogException;                                                                                                      import pt.tecnico.po.ui.Input;                                                                                                                import woo.Storefront;                                                                                                                        //FIXME import other classes

/**
 * Save current state to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<Storefront> {

  private Input<String> file;

  /** @param receiver */
  public DoSave(Storefront receiver) {
    super(Label.SAVE, receiver);
    if(_receiver.getFilename() == "")
      file = _form.addStringInput(Message.newSaveAs());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    try
    {
      if(_receiver.getFilename() == "")
      {
        _form.parse();
        _receiver.saveAs(file.value());
      }
      else
        _receiver.save();
    }
    catch(IOException ioe)
    {
      ioe.printStackTrace();
    }
    catch(MissingFileAssociationException mfae)
    {
      _display.addLine(mfae.getMessage());
      _display.display();
      mfae.printStackTrace();
    }
  }
}

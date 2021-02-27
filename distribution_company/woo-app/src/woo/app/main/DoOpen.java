package woo.app.main;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import woo.Storefront;
import woo.exceptions.UnavailableFileException;
import woo.app.exceptions.FileOpenFailedException;
import java.io.*;


/**
 * Open existing saved state.
 */
public class DoOpen extends Command<Storefront> {

  private Input<String> file;

  /** @param receiver */
  public DoOpen(Storefront receiver) {
    super(Label.OPEN, receiver);
    file = _form.addStringInput(Message.openFile());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    try {
      _form.parse();
      _receiver.load(file.value());
    } catch (UnavailableFileException ufe) {
      throw new FileOpenFailedException(ufe.getFilename());
    }
    catch (FileNotFoundException fnfe) 
    {
      throw new FileOpenFailedException(file.value());
    }
    catch(IOException ioe)
    {
      ioe.printStackTrace();
    }
    catch(ClassNotFoundException cnfe)
    {
      cnfe.printStackTrace();
    }
  }

}

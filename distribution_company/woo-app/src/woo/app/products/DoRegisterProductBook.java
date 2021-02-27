package woo.app.products;

import javax.lang.model.util.ElementScanner6;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import woo.Storefront;
import woo.app.exceptions.DuplicateProductKeyException;
import woo.app.exceptions.UnknownSupplierKeyException;


/**
 * Register book.
 */
public class DoRegisterProductBook extends Command<Storefront> {

  private Input<String> _id;
  private Input<String> _supplierId;
  private Input<Integer> _price;
  private Input<String> _title;
  private Input<String> _author;
  private Input<String> _ISBN;
  private Input<Integer> _minStock;

  public DoRegisterProductBook(Storefront receiver) {
    super(Label.REGISTER_BOOK, receiver);
    _id = _form.addStringInput(Message.requestProductKey());
    _title = _form.addStringInput(Message.requestBookTitle());
    _author = _form.addStringInput(Message.requestBookAuthor());
    _ISBN = _form.addStringInput(Message.requestISBN());
    _price = _form.addIntegerInput(Message.requestPrice());
    _minStock = _form.addIntegerInput(Message.requestStockCriticalValue());
    _supplierId = _form.addStringInput(Message.requestSupplierKey());
  }

  @Override
  public final void execute() throws DialogException, DuplicateProductKeyException, UnknownSupplierKeyException{
    
    _form.parse();

    if(_receiver.containsProductKey(_id.value()))
      throw new DuplicateProductKeyException(_id.value());
    else if(!_receiver.containsSupplierKey(_supplierId.value()))
      throw new UnknownSupplierKeyException(_supplierId.value());
    else
    _receiver.registerProductBook(_id.value(), _supplierId.value(), _price.value(), 
    _minStock.value(),_title.value(),_author.value(), _ISBN.value());

  }
}

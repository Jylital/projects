import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;

public class JTextFieldLimit extends PlainDocument
{
	// Ei tarvitse konstruktoria, sillä tässä luokassa ei ole mitään attribuutteja
	
	public void insertString(int offset, String str, AttributeSet a) throws BadLocationException
	{
		if(str == null)
			return;
		if((getLength() + str.length()) <= 1) // Annettu string lisätään jos ja vain jos alkuperäisen sisällön pituus on 0
			super.insertString(offset, str, a);
	}
}
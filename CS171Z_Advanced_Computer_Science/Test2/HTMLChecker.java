/*************************************************************************
 *  Compilation:  javac -cp :../stdlib.jar HTMLChecker.java
 *  Execution:    java -cp :../stdlib.jar HTMLChecker < small.html
 *  
 *  Checks to ensure that HTML tags are balanced and well-formed.
 *
 *************************************************************************/

public class HTMLChecker {

    public static String getTag() {
        final int OUTSIDE  = 0;   // parsing normal code
        final int INTAG    = 1;   // found a leading '<'

        int state = OUTSIDE;      // initial state

	String tag = "";
	boolean found = false;

        while(!StdIn.isEmpty() && ! found) {
            char c = StdIn.readChar();
            switch(state) {
                case OUTSIDE:  
		    if      (c == '<')  state = INTAG;
		    break;

                case INTAG: 
		    if      (c == '>')  found = true;
		    else if (c == '<')  tag = ""; // Oops! Nevermind...
		    else if (c == '\n' || c == '\t' || c == ' ') 
			{ state = OUTSIDE; tag = ""; }
		    else                tag += c;  
		    break;
             }
        }
	if (! found && ! tag.equals("")) { // bad tag
	    System.out.println("Illegal tag: " + tag);
	    System.exit(0);
	}
	return tag;
    }


    public static void main(String[] args) {
        DoublingStack<String> stack;

	if (args.length > 0 && args[0].equals("-t"))
	    stack = new DoublingStack<String>(true);
	else
	    stack = new DoublingStack<String>();

	String token;

	while (!StdIn.isEmpty()) {
	    token = getTag();
	    if (token.equals("")) break;
	    else if (token.charAt(0) == '/') { // a closing tag
		if (stack.isEmpty()) {
		    System.out.println("unmatched closing tag: <" + token + ">");
		    System.exit(0);
		}

		String check = stack.pop();

		if (token.substring(1).equals(check)) ; // match
		else {
		    System.out.println("tag mismatch: <" + check + "> <" + token + ">");
		    System.exit(0);
		}
	    }
	    else 
		stack.push(token);
	}

	if (! stack.isEmpty()) {
	    String extra = stack.pop();
	    System.out.println("unmatched opening tag: <" + extra + ">");
	    System.exit(0);
	}

	System.out.println("All tags matched correctly.");
    }
}
	    

	
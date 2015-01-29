/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *
 *  DATE SUBMIITED: 11/25/2013
 *  Compilation:  javac Pycalc.java 
 *  Execution:    java  > commandline
 *  Dependencies: StdIn.java
 *
 *************************************************************************/

import java.util.*;
import java.util.regex.*;
import java.io.*;
import java.math.*;

class Tokenizer {
  private String theLine; // This references the input line.
    private int lineIndex;  // This points to the next character to
                                   // be inspected when getToken() is called.

    private boolean done;   // This is false initially, and set to true
                                   // when lineIndex >= theLine.length().
    public Tokenizer(String input){ // This initializes the instance variables of the class.
      theLine = input;
    }
           
    public String getToken() throws SyntaxErrorException { // This implements the finite state 
                                                          // machine to collect and returnthe next token 
        final int NUMBERS = 0;                            // in theLine starting from position lineIndex
        final int DOUBLES = 1;
        final int IDENTIFIERS = 2;
        final int START = 3;
        int STATE = START;

        String currentStr = "";

        tokenloop:
            while (!done()) {

            char c = theLine.charAt(lineIndex);
            switch(STATE) {
                case START: 
                            if (c == '.') 
                                { STATE = DOUBLES; currentStr += c; lineIndex++;                      }
                            else if (c == '+' || c == '-' || c == '*' 
                                              || c == '/' || c == '%' 
                                              || c == '(' || c == ')'
                                              || c == '=') 
                                { STATE = START; currentStr += c; lineIndex++; break tokenloop;       }
                            else if ((Character.toString(c).matches("[A-Z?]")) 
                                    ||(Character.toString(c).matches("[a-z?]"))) 
                                { STATE = IDENTIFIERS; lineIndex++; currentStr += c;                  }
                            else if ((Character.toString(c).matches("[0-9?]"))) 
                                { STATE = NUMBERS; lineIndex++; currentStr += c;                      }
                            else if (c == ' ') 
                                { STATE = START; lineIndex++;                                         }   
                            else 
                                { throw new SyntaxErrorException("Error: invalid syntax");            }
                            break;

                case NUMBERS: 
                            if ((Character.toString(c).matches("[0-9?]"))) 
                                { STATE = NUMBERS; lineIndex++;  currentStr += c;                     }
                            else if (c == '.') 
                                { STATE = DOUBLES; lineIndex++; currentStr += c;                      }
                            else if (c == ' ') 
                                { STATE = START; lineIndex++; break tokenloop;                        }
                            else if (c == ')') 
                                { STATE = START; break tokenloop;                                     }
                            else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' 
                                              || c == '=' || c == '(')
                                { STATE = START;   break tokenloop; }
                            else 
                                { throw new SyntaxErrorException("Error: invalid syntax");            }

                            break;

                case DOUBLES:
                            if ((Character.toString(c).matches("[0-9?]"))) 
                                { STATE = DOUBLES; lineIndex++; currentStr += c;                      }
                            else if (c == ' ')  
                                { STATE = START;  break tokenloop;                                    }
                            else if (c == ')') 
                                { STATE = START; break tokenloop;                                     }
                            else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' 
                                              || c == '=' || c == '(')          
                                { STATE = START;   break tokenloop;                                   }
                            else 
                                { throw new SyntaxErrorException("Error: invalid syntax");            }

                            break;

                case IDENTIFIERS:
                            if (c == ' ') 
                                { STATE = START; lineIndex++; break tokenloop;                        }                                   
                            else if (((Character.toString(c).matches("[A-Z?]")) 
                                 || (Character.toString(c).matches("[a-z?]")))
                                 || ((Character.toString(c).matches("[0-9?]")))) 
                                { STATE = IDENTIFIERS; lineIndex++; currentStr += c;   }
                            else if (c == ')') 
                                { STATE = START;  break tokenloop;                                    }
                            else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' 
                                              || c == '=' || c == '(')  
                                { STATE = START;   break tokenloop;   }
                            else 
                                { throw new SyntaxErrorException("Error: invalid syntax");            }

                            break;                     

                default:        { throw new SyntaxErrorException("Error: invalid syntax");            }
            }
        }
              return currentStr;
    }

    public boolean done() { 
        if(lineIndex >= theLine.length()) return true;
        else return false; 
    }
}


// One of the main class
class InToPost {
  private Queue<String> theQ;   // This is the queue of the
                                       // translated postfix expression.
    private Tokenizer theT;      // This is the tokenizer where
                                       // the infix expression resides.
    public InToPost(Tokenizer in) { // This initializes the instance variables (theT = in).
      theT = in;
    }

    public boolean isOperator(String c) { // Tell whether c is an operator.
        return c.equals("+")  ||  c.equals("-")  || c.equals("*") || c.equals("%")  
                              ||  c.equals("/")  || c.equals("(") || c.equals(")");
    }

    public static boolean isIdentifier(String c) { // Tell whether c is an proper operator
      boolean bool = true;
        if(!(((Character.toString((c.charAt(0)))).matches("[A-Z?]")) 
            || (Character.toString(c.charAt(0)).matches("[a-z?]"))))
          return false;
        else {
            for(int i = 1; i < c.length(); i++){
                if(!((Character.toString(c.charAt(i)).matches("[A-Z?]")) 
                  || (Character.toString(c.charAt(i)).matches("[a-z?]"))
                  || (Character.toString(c.charAt(i)).matches("[0-9?]")))) {

                      bool = false;
                }
            }
        }
        return bool;
    }

    public static boolean isInteger(String c) {
        Pattern pattern = Pattern.compile("^[-\\+]?[\\d]*[\\.]*[0]$"); 
        return pattern.matcher(c).matches(); 
    }

    /* Returns the operator with higher precedence among 'op1' & 'op2', if they have equal
     * precedence, the first operator in the argument list (op1) is returned.*/

    private boolean lowerPrecedence(String op1, String op2) {
      // Tell whether op1 has lower precedence than op2, where op1 is an
      // operator on the left and op2 is an operator on the right.
      // op1 and op2 are assumed to be operator characters (+,-,*,/,%).
      
      switch (op1) {

         case "+":
         case "-":
            return !(op2.equals("+") || op2.equals("-")) ;

         case "*":
         case "/":
         case "%":
            return op2.equals("(");

         case "(": return true;

         default:  // (shouldn't happen)
            return false;
      }
    }

    public Queue translate() throws SyntaxErrorException { 

        Stack<String> stack = new Stack<String>();

        theQ = new Queue<String>();

        while(!theT.done()) {

            String token = theT.getToken();

            if(isOperator(token)) {

             while(!stack.isEmpty() && !lowerPrecedence(stack.peek(), token))
                theQ.enqueue(stack.pop());   

                if(token.equals(")")) {
                    String op = stack.pop();
                        while(!op.equals("(")) {
                                theQ.enqueue(op);
                                op = stack.pop(); 
                            }
                } else 
                    stack.push(token);

            } else
                theQ.enqueue(token);          
        }

        while(!stack.isEmpty()) {
            String x = stack.pop();
            if(x.equals("(")) throw new SyntaxErrorException("");
            theQ.enqueue(x);        
        }
        return theQ;
    }
}


// Main class- evaluate
class Evaluate {
    private Queue<String> inputQ;  // This is the queue that holds
                                        // the postfix expression to be
                                        // evaluated.
    public Evaluate(Queue<String> in) { 
        inputQ = in; 
    }
    
    public boolean isNumber(String x) {
        try {
           Double m = Double.parseDouble(x);
           return true;
        } catch(NumberFormatException e){
           return false;
        }
    }

    public boolean isOperator(String c) { // Tell whether c is an operator.
        return c.equals("+")  ||  c.equals("-")  || c.equals("*") || c.equals("%")  
                              ||  c.equals("/") ;
    }

    public Double evaluate (HashTable<String, Double> ST) throws SyntaxErrorException, 
                                                            VariableNotFoundException {
        Stack<Double> stack = new Stack<Double>();
        Double result;
        Double answer;

     try {
        while(!inputQ.isEmpty()) {
          String nextString = inputQ.dequeue();
          if(isOperator(nextString)){
              Double val2 = stack.pop();
              Double val1 = stack.pop();
              // use BigDecimal to reduce the loss of precision
              BigDecimal bd1 = new BigDecimal(Double.toString(val1));
              BigDecimal bd2 = new BigDecimal(Double.toString(val2));

              switch(nextString) {
                  case "+":  result = (bd1.add(bd2)).doubleValue();
                             stack.push(result);
                             break; 
                  case "-":  result = (bd1.subtract(bd2)).doubleValue();
                             stack.push(result);
                             break;
                  case "/":  result = val1 / val2;
                             stack.push(result);
                             break;
                  case "%":  result = val1 % val2;
                             stack.push(result);
                             break;
                  case "*":  result = (bd1.multiply(bd2)).doubleValue();
                             stack.push(result);
                             break;
              } 
            } else if(isNumber(nextString)) {
                  stack.push(Double.parseDouble(nextString));
            } else {
                    if(ST.contains(nextString)) {
                        stack.push(ST.get(nextString));
                    } else if(!ST.contains(nextString) && InToPost.isIdentifier(nextString)) {
                        throw new VariableNotFoundException("variable '" + 
                        nextString + "' is not defined");
                    } else
                        throw new SyntaxErrorException("");
                    }
            }

            answer = stack.pop();
            if(stack.isEmpty())
                return answer;
            else {
              throw new SyntaxErrorException("Error: invalid syntax");
            }
    } catch(EmptyStackException e) {
              throw new SyntaxErrorException("Error: invalid syntax");
    }
  }
}

// an ErrorException class
class SyntaxErrorException extends Exception {
    SyntaxErrorException(String message) {
      super(message);
    }
}

class VariableNotFoundException extends Exception {
    VariableNotFoundException(String message) {
        super(message);
    }
}


// this is the helper class-stack for class IntoPost
class Stack<Item> {
    private int N;          // size of the stack
    private Node first;     // top of stack

    // helper linked list class
    private class Node {
        private Item item;
        private Node next;
    }

    public Stack() {
        first = null;
        N = 0;
    }

    public boolean isEmpty() {
        return first == null;
    }

    public int size() {
        return N;
    }

    public void push(Item item) {
        Node oldfirst = first;
        first = new Node();
        first.item = item;
        first.next = oldfirst;
        N++;
    }

    public Item pop() throws SyntaxErrorException {
    try {
        Item item = first.item;        // save item to return
        first = first.next;            // delete first node
        N--;
        return item;                   // return the saved item
    } catch (NullPointerException e) {
        throw new SyntaxErrorException("");
    }   
    }

    public Item peek() {
        if (isEmpty()) 
      throw new RuntimeException("Stack underflow");
        return first.item;
    }
}

// this is the helper class for queue
class Queue<Item> implements Iterable<Item> {
    private int N;               // number of elements on queue
    private Node<Item> first;    // beginning of queue
    private Node<Item> last;     // end of queue

    private static class Node<Item> {
        private Item item;
        private Node<Item> next;
    }

    public Queue() {
        first = null;
        last  = null;
        N = 0;
    }

    public boolean isEmpty() {
        return first == null;
    }

    public int size() {
        return N;     
    }

    public Item peek() {
        if (isEmpty()) throw new NoSuchElementException("Queue underflow");
        return first.item;
    }

    public void enqueue(Item item) {
        Node<Item> oldlast = last;
        last = new Node<Item>();
        last.item = item;
        last.next = null;
        if (isEmpty()) first = last;
        else           oldlast.next = last;
        N++;
    }

    public Item dequeue() {
        if (isEmpty()) throw new NoSuchElementException("Queue underflow");
        Item item = first.item;
        first = first.next;
        N--;
        if (isEmpty()) last = null;   // to avoid loitering
        return item;
    }

    public String toString() {
        StringBuilder s = new StringBuilder();
        for (Item item : this)
            s.append(item + " ");
        return s.toString();
    } 

    public Iterator<Item> iterator()  {
        return new ListIterator<Item>(first);  
    }

    private class ListIterator<Item> implements Iterator<Item> {
        private Node<Item> current;

        public ListIterator(Node<Item> first) {
            current = first;
        }

        public boolean hasNext()  { return current != null;                     }
        public void remove()      { throw new UnsupportedOperationException();  }

        public Item next() {
            if (!hasNext()) throw new NoSuchElementException();
            Item item = current.item;
            current = current.next; 
            return item;
        }
    }
}

// this is the symbol table implemented by separate chaining
class SymbolTable<Key, Value> {
    private int N;           
    private Node first;      

    // a helper linked list data type
    private class Node {
        private Key key;
        private Value val;
        private Node next;

        public Node(Key key, Value val, Node next)  {
            this.key  = key;
            this.val  = val;
            this.next = next;
        }
    }

    public SymbolTable() { }

    public int size() {
        return N;
    }

    public boolean isEmpty() {
        return size() == 0;
    }

    public boolean contains(Key key) {
        return get(key) != null;
    }

    public Value get(Key key) {
        for (Node x = first; x != null; x = x.next) {
            if (key.equals(x.key)) return x.val;
        }
        return null;
    }

    // insert a new pair of records
    public void put(Key key, Value val) {
        if (val == null) { delete(key); return; }
        for (Node x = first; x != null; x = x.next)
            if (key.equals(x.key)) { x.val = val; return; }
        first = new Node(key, val, first);
        N++;
    }

    // remove a pair of records
    public void delete(Key key) {
        first = delete(first, key);
    }

    private Node delete(Node x, Key key) {
        if (x == null) return null;
        if (key.equals(x.key)) { N--; return x.next; }
        x.next = delete(x.next, key);
        return x;
    }

        public Iterable<Key> keys()  {
        Queue<Key> queue = new Queue<Key>();
        for (Node x = first; x != null; x = x.next)
            queue.enqueue(x.key);
        return queue;
    }
}

// this the the main hash table class
class HashTable<Key, Value> {
    private static final int INIT_CAPACITY = 10;
    private int N;                                // number of key-value pairs
    private int M;                                // hash table size
    private SymbolTable<Key, Value>[] st;  

    public HashTable() {
        this(INIT_CAPACITY);
    } 

    // create separate chaining hash table with M lists
    public HashTable(int M) {
        this.M = M;
        st = (SymbolTable<Key, Value>[]) new SymbolTable[M];
        for (int i = 0; i < M; i++)
            st[i] = new SymbolTable<Key, Value>();
    } 

    // resize the hash table 
    private void resize(int chains) {
        HashTable<Key, Value> temp = new HashTable<Key, Value>(chains);
        for (int i = 0; i < M; i++) {
            for (Key key : st[i].keys()) {
                temp.put(key, st[i].get(key));
            }
        }
        this.M  = temp.M;
        this.N  = temp.N;
        this.st = temp.st;
    }

    // calculate the hash values
    private int hash(Key key) {
        return (key.hashCode() & 0x7fffffff) % M;
    } 

    public int size() {
        return N;
    } 

    public boolean isEmpty() {
        return size() == 0;
    }

    public boolean contains(Key key) {
        return get(key) != null;
    } 

    // get the record
    public Value get(Key key) {
        int i = hash(key);
        return st[i].get(key);
    } 

    // insert the record
    public void put(Key key, Value val) {
        if (val == null) { delete(key); return; }

        // double table size if average length of list >= 10
        if (N >= 10*M) resize(2*M);

        int i = hash(key);
        if (!st[i].contains(key)) N++;
        st[i].put(key, val);
    } 

    // delete a record
    public void delete(Key key) {
        int i = hash(key);
        if (st[i].contains(key)) N--;
        st[i].delete(key);

        // halve table size if average length of list <= 2
        if (M > INIT_CAPACITY && N <= 2*M) resize(M/2);
    }

    // return keys in symbol table as an Iterable
    public Iterable<Key> keys() {
        Queue<Key> queue = new Queue<Key>();
        for (int i = 0; i < M; i++) {
            for (Key key : st[i].keys())
                queue.enqueue(key);
        }
        return queue;
    }
}


// main operation class
public class Pycalc {
  public static void main(String[] args) {
        // declare a hashtable and initialize it
        HashTable<String, Double> st = new HashTable<String, Double>();
        //read a command line
        Scanner scanner = new Scanner(System.in);

        System.out.println("Welcome to the commandline calculator (version 1.0)");
        System.out.println("You can assign a value to a proper variable using '='");
        System.out.println("Or print some evaluations using 'print'   \nEnjoy! \n");
        System.out.println("Please start to input or type 'exit' to exit the program");

while(true) {
        System.out.print("> ");
        String line = scanner.nextLine();

        if(!line.equals("exit")) {
          try {
            Tokenizer token = new Tokenizer(line);
            String firstToken = token.getToken();
            // look at the first token to see whether it's "print"...
            if(firstToken.equals("print")) {
                InToPost intoposttranslator = new InToPost(token);

                    Queue<String> theQ = intoposttranslator.translate();
                    Evaluate eva = new Evaluate(theQ);
                    Double result = eva.evaluate(st);
                        if(InToPost.isInteger(result.toString()))
                            System.out.println(result.intValue());
                        else 
                            System.out.println(result);
                    continue;
            } else if(InToPost.isIdentifier(firstToken)) {
                String secondToken = token.getToken();
                if(!secondToken.equals("=")) {
                    throw new SyntaxErrorException("");
                } else {
                    InToPost intoposttranslator2 = new InToPost(token);
                        Queue<String> theQ2 = intoposttranslator2.translate();
                        Evaluate eva2 = new Evaluate(theQ2);
                        Double result = eva2.evaluate(st);
                        st.put(firstToken, result);
                        continue;
                }
            } else if(!InToPost.isIdentifier(firstToken) && token.getToken().equals("=")) {
                System.out.println("Error: invalid variable name");
            } else 
                throw new SyntaxErrorException("");
          } catch(SyntaxErrorException e) {
                System.out.println("Error: invalid sytax");
                continue;
            }
            catch(VariableNotFoundException e) {
                System.out.println(e.getMessage());
            }
        } else {
            System.out.println("Thank you for using the commandline calculator (version 1.0), bye!");
            System.exit(-1);
          }
      }
    }  
}





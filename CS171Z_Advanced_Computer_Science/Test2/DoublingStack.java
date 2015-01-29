/*************************************************************************
 *  Compilation:  javac DoublingStack.java
 *  Execution:    java DoublingStack
 *  
 *  Stack implementation with an array. Resizes by doubling and halving.
 *  Modified from DoublingStack.java with iterable, iterator removed.
 *
 *************************************************************************/

import java.util.NoSuchElementException;

public class DoublingStack<Item> {
    private Item[] a;         // array of items
    private int N = 0;        // number of elements on stack
    private boolean TRACE = false;

    // create an empty stack
    public DoublingStack(boolean trace) {
	TRACE = trace;
        a = (Item[]) new Object[2];
    }
    public DoublingStack() {
        a = (Item[]) new Object[2];
    }

    public boolean isEmpty() { return N == 0; }
    public int size()        { return N;      }

    // resize the underlying array holding the elements
    private void resize(int capacity) {
        assert(capacity >= N);
        Item[] temp = (Item[]) new Object[capacity];
	if (TRACE) System.out.println("Stack resized: " + capacity);
        for (int i = 0; i < N; i++)
            temp[i] = a[i];
        a = temp;
    }

    // push a new item onto the stack
    public void push(Item item) {
        if (N == a.length)       // double size of array if necessary
	    resize(2*a.length);    
        a[N++] = item;           // add item
    }

    // delete and return the item most recently added
    public Item pop() {
        if (isEmpty()) { 
	    throw new RuntimeException("Stack underflow error"); 
	}
        Item item = a[N-1];
        a[N-1] = null;           // to avoid loitering
        N--;
        // shrink size of array if necessary
        if (N > 0 && N == a.length/4) 
	    resize(a.length/2);
        return item;
    }


    // string representation (inefficient because of string concatenation)
    public String toString() {
        String s = "[ ";
        for (int i = N - 1; i >= 0; i--)
            s += a[i] + " ";
        s += "]";
        return s;
    }

    /***********************************************************************
     * Test routine.
     **********************************************************************/
    public static void main(String[] args) {
        DoublingStack<String> stack;

	if (args.length > 0 && args[0].equals("-t"))
	    stack = new DoublingStack<String>(true);
	else
	    stack = new DoublingStack<String>();

        stack.push("how");
        stack.push("now");
        stack.push("brown");
        stack.push("cow");
        stack.push("the");
        stack.push("rain");
        stack.push("in");
        stack.push("Spain");
        stack.push("falls");
        stack.push("mainly");
        stack.push("in");
        stack.push("the");
        stack.push("plain");

	System.out.println(stack);

        while (!stack.isEmpty())
            System.out.println(stack.pop());
    }

}
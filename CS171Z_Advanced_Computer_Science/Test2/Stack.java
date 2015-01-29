/*************************************************************************
 *  Compilation:  javac Stack.java
 *  Execution:    java Stack < input.txt
 *
 *  A generic stack, implemented using a linked list. Each stack
 *  element is of type Item.
 *  
 *************************************************************************/

import java.util.NoSuchElementException;

/**
 *  The <tt>Stack</tt> class represents a last-in-first-out (LIFO) stack of generic items.
 *  It supports the usual <em>push</em> and <em>pop</em> operations, along with methods
 *  for peeking at the top item, testing if the stack is empty, and iterating through
 *  the items in LIFO order.
 */
public class Stack<Item> {
    private int N;          // size of the stack
    private Node first;     // top of stack

    // helper linked list class
    private class Node {
        private Item item;
        private Node next;
    }

    /**
     * Create an empty stack.
     */
    public Stack() {
        first = null;
        N = 0;
    }

    /**
     * Is the stack empty?
     */
    public boolean isEmpty() {
        return first == null;
    }

    /**
     * Return the number of items in the stack.
     */
    public int size() {
        return N;
    }

    /**
     * Add the item to the stack.
     */
    public void push(Item item) {
        Node oldfirst = first;
        first = new Node();
        first.item = item;
        first.next = oldfirst;
        N++;
    }

    /**
     * Delete and return the item most recently added to the stack.
     * Throw an exception if no such item exists because the stack is empty.
     */
    public Item pop() {
        if (isEmpty()) 
	    throw new RuntimeException("Stack underflow");
        Item item = first.item;        // save item to return
        first = first.next;            // delete first node
        N--;
        return item;                   // return the saved item
    }


    /**
     * Return the item most recently added to the stack.
     * Throw an exception if no such item exists because the stack is empty.
     */
    public Item peek() {
        if (isEmpty()) 
	    throw new RuntimeException("Stack underflow");
        return first.item;
    }

    /**
     * Return string representation.
     */
    public String toString() {
        StringBuilder s = new StringBuilder();
	Node ptr = first;
	// reference chasing
	while (ptr != null) {
            s.append(ptr.item + " ");
	    ptr = ptr.next;
	}
        return s.toString();
    }
       

    /**
     * A test client.
     */
    public static void main(String[] args) {
        Stack<String> stack = new Stack<String>();
        stack.push("Hello");
        stack.push("World");
        stack.push("how");
        stack.push("are");
        stack.push("you");

	System.out.println(stack);

        while (!stack.isEmpty())
            System.out.println(stack.pop());
    }
}

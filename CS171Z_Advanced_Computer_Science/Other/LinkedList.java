/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *
 *  DATE SUBMIITED: 10/07/2013
 *  Compilation:  javac LinkedList.java
 *  Execution:    java LinkedList
 *************************************************************************/
import java.util.*;

public class LinkedList<Item extends Comparable<Item>> {
		private Node found;
		private Node first;  // start of the list
		private boolean hasTheItem;
		private int count = 0;

		//helper linked list class
		private class Node {
			Item item;
			Node next;
			Node previous;
		}


	// add item to the list if it does not already exist,
	// return true if a new Node is created, false if not
	public boolean insert(Item item){
		if(contains(item, first)) {
				return false;
		} else {
			if(isEmpty()) {
				first = new Node();
				first.item = item;
				count++;
				return true;
			} else {
					Node oldfirst = first;
					first = new Node();
					first.item = item;
					first.next = oldfirst;
					oldfirst.previous = first;
					count++;
					return true;
			}
		}
	}

	public void printFirst(){
		if(first == null) System.out.print("Usage: Empty List----");
		else System.out.println(first.item);
	}

	public Item remove(Item item) {
		// remove the item if it exists in the list, leave the list unchanged otherwise
		Node ptr = first;
		Node prev = null;
		Item x = null;

		while (ptr != null) {
			if (ptr.item.equals(item)) {
				if (prev == null)
					first = ptr.next;
				else 
					prev.next = ptr.next;
				x = item;
				break;
			}
		prev = ptr;
		ptr = ptr.next;
		}
		
		return x;
	}

	//check whether the linkedlist is empty
	public boolean isEmpty() {
		return count == 0;
	}

	// a method to check whether the linkedlist has a specific element
	public boolean contains(Item item, Node start) {
		if(start == null) {
			found = null;
			return false;
		} else if(start.item.equals(item)) {
			found = start;
			return true;
		} else {
			return contains(item, start.next);
		}
	}

	// implement the iterator
	public Iterator<Item> iterator() { return new ListIterator(); }

	public class ListIterator implements Iterator<Item> {
		private Node current = first;

		public boolean hasNext() {return current != null; }
		public void remove() {throw new UnsupportedOperationException(); }
		public Item next() {
			Item item = current.item;
			current = current.next;
			return item;
		}
	}

	// cliet test
	public static void main(String[] args){
		LinkedList<Integer> xyz = new LinkedList<Integer>();

		System.out.println(xyz.insert(1));
		System.out.println(xyz.insert(2));
		System.out.println(xyz.insert(3));
		System.out.println(xyz.insert(4));				
		System.out.println(xyz.insert(5));
		System.out.println(xyz.remove(5));
		System.out.println(xyz.remove(3));
		System.out.println(xyz.remove(2));
		System.out.println(xyz.remove(7));
		System.out.println(xyz.remove(1));


		System.out.println("---------------");
		xyz.printFirst();
		Iterator<Integer> it = xyz.iterator();
		while(it.hasNext()){
			System.out.println(it.next());
		}
	}
}





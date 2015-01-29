import java.util.*;

public class Queue<Item> {
	private Node first;
	private Node last;
	private int N;

	private class Node {
		private Item item;
		private Node next;
	}

	public Queue() {
		first = null;
		last = null;
		N = 0;
	}

	public int size() {
		return N;
	}

	public boolean isEmpty(){
		return first == null;
	}

	public void enqueue(Item item) {
		Node oldlast = last;
		last = new Node();
		last.item = item;
		last.next = null;

		if(isEmpty()) first = last;
		else oldlast.next = last;
		N--;
	}

	public Item dequeue(){
		if(isEmpty()) throw new RuntimeException("@!!");
		Item item = first.item;
		first = first.next;
		N--;

		if(isEmpty()) last = null;
		return item;
	}

	@Override
	public String toString() {
		Node pr = first;
		StringBuilder s = new StringBuilder();
		while(pr != null) {
			s = s.append(pr.item + " ");
			pr = pr.next;
		}
		return s.toString();
	}

	public static void main(String[] args) {
		Queuee<Integer> xxx = new Queuee<Integer>();
		xxx.enqueue(1);
		xxx.enqueue(33);
		xxx.dequeue();
		System.out.println(xxx);
	}
}
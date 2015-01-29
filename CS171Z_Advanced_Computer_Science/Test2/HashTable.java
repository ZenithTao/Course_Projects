/*************************************************************************
 *  Compilation:  javac HashTable.java 
 *  Execution:    java HashTable
 *
 *  Demonstrates an implementation of hash table using separate
 *  chaining.  Uses ArrayList since Java does not allow generic arrays.
 *************************************************************************/

import java.util.*; 

public class HashTable<Key extends Comparable<Key>,Item> {

    private ArrayList theTable;

    // constructor
    public HashTable(int size) {
	theTable = new ArrayList();
	for (int i = 0; i < size; i++) // initialize all to the empty list
	    theTable.add(i,new LinkedList<Key,Item>());
    }

    public void insert(Key key, Item value)  {
	int index = Math.abs(key.hashCode() % theTable.size());
	((LinkedList<Key,Item>) theTable.get(index)).insert(key,value);
    } 

    public void delete(Key key) {
	int index = Math.abs(key.hashCode() % theTable.size());
	Item item = ((LinkedList<Key,Item>) theTable.get(index)).remove(key);
    } 

    public Item find(Key key) {
	int index = Math.abs(key.hashCode() % theTable.size());
	return ((LinkedList<Key,Item>) theTable.get(index)).lookup(key); 
    }

    public void display() {
	for (int i = 0; i < theTable.size(); i++)
	    System.out.print("[" + i + "]:" + theTable.get(i));
	System.out.println();
    }
	    

    public static void main(String[] args) {
	HashTable<String,Integer> st = new HashTable<String,Integer>(10);

	st.insert("xyz",1);
	st.insert("abc",2);
	st.insert("ac",3);
	st.insert("george",4);
	st.insert("john",5);

	st.display();

	st.delete("georg");
	st.display();

	st.delete("george");
	st.display();

	System.out.println("looking for george:" + st.find("george"));
	System.out.println("looking for john:" + st.find("john"));
	System.out.println("looking for jon:" + st.find("jon"));

    }
    
}
// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Liang Tao

// PRedBlackBST: a persistent left-leaning red-black tree, without delete.
// You need to implement persistent insertion (the "put" method).  If you
// have time, you should also try to implmement the StackIterator (see below).
// Our parent class PBST has already defined some useful things:
//   * some PBST methods (like size, contains, get) do not need any change.
//   * the Node class already has a 'color' field and a setColor method.
//   * the isRed(x) method, like in the book (treats null as black)
//   * the check() method, which checks for correct red-black tree structure.
//   * all fields are declared final, to support persistence.

public class PRedBlackBST<Key extends Comparable<Key>, Value>
    extends PBST<Key, Value>
{
    // Constructors: these just call the parent class constructors.
    public PRedBlackBST() { super(); }
    PRedBlackBST(Node r) { super(r); }

    // Color constants, just for convenience.
    static final boolean RED   = true;
    static final boolean BLACK = false;

    // The setRoot(Node r) method.  Its declared return type is still
    // PBST, but the actual type returned is PRedBlackBST.  It could
    // return this tree, if nothing has actually changed.
    PBST<Key,Value> setRoot(Node r) {
        return r==root ? this : new PRedBlackBST<Key,Value>(r);
    }

    // TODO: public put method (top level).
    // Should call the recursive put method, make sure the root
    // is black, and then call setRoot.
    public PBST<Key,Value> put(Key key, Value val) { 
        Node r = put(root, key, val).setColor(BLACK);
        return setRoot(r);
    }

    // TODO: recursive put (or insert) method.  This should be
    // like the recursive method in share/book/RedBlackBST.java,
    // except that it builds new nodes in a persistent way.
    Node put(Node h, Key key, Value val) {

        if (h == null) return new Node(key, val, null, null, RED);
        
        int cmp = key.compareTo(h.key);  
        Node new_node;
        if (cmp < 0) new_node = h.setLeft(put(h.left, key, val));
        else if (cmp > 0)  new_node = h.setRight(put(h.right, key, val));
        else new_node = h.setVal(val);
        
        // test the case that should be addressed
        if (isRed(new_node.right) && !isRed(new_node.left)) 
            new_node = rotateLeft(new_node);
        if (isRed(new_node.left) && isRed(new_node.left.left)) 
            new_node = rotateRight(new_node);
        if (isRed(new_node.left) && isRed(new_node.right))     
            new_node = flipColors(new_node);

        return new_node;
    }

    // TODO: persistent version of rotateRight(h), two new nodes.
    Node rotateRight(Node h) {
        Node newh = new Node(h.key, h.val, h.left.right, h.right, RED);
        Node x = new Node(h.left.key, h.left.val, h.left.left, newh, h.color);
        return x;
    }
 
    // TODO: persistent version of rotateLeft(h), two new nodes.
    Node rotateLeft(Node h) {
        Node newh = new Node(h.key, h.val, h.left, h.right.left, RED);
        Node x = new Node(h.right.key, h.right.val, newh,
                            h.right.right, h.color);

        return x;
    }

    // TODO: presistent version of flipColors(h), three new nodes.
    Node flipColors(Node h) {
        Node a = h.left.setColor(BLACK);
        Node b = h.right.setColor(BLACK);
        Node newh = new Node(h.key, h.val, a, b, RED);
        return newh;
    }
    // TODO: once you have done the above "TODO"s, you should be
    // able to pass the test performed by Driver.testPRedBlackBST().
    // So, go check on that.
    
    // TODO: if you have finished the above test, and still have time,
    // then there is one more challenge: replace the IntIterator
    // (inherited from PBST) with a faster StackIterator, below.
    
    // Just like IntIterator, a StackIterator lets us visits the keys
    // in order.  It uses a stack of O(H) nodes (where H is height)
    // to keep track of the unvisited parts of the tree.  It turns
    // out this approach is faster, only O(1) amortized time per visit.
    // Like IntIterator, we will not implement the remove() method, just
    // the hasNext() and next() methods.
    //
    // IDEA: for each Node x on the stack, we still need to visit x,
    // followed by all the nodes in its x.right subtree.
    // So, just to construct the initial stack, you should push the
    // "left spine" of the initial tree onto the stack.

    // Note: this starts a C-style multiline comment!
     
    // This line uses StackIterator instead of IntIterator:
    public java.util.Iterator<Key> iterator() { return new StackIterator(); }
   
    class StackIterator implements java.util.Iterator<Key> {
        java.util.Stack<Node> todo = new java.util.Stack<Node>();
        private Node current;
        // TODO: constructor, and the hasNext() and next() methods.
        // I'll give you remove, since we are not supporting deletion:
        public StackIterator() {
            current = root;
        }

        public Key next() {
            while (current != null) {
                todo.push(current);
                current = current.left;
            }
            
            current = todo.pop();
            Node return_node = current;
            current = current.right;

            return return_node.key;
        }

        public boolean hasNext() {
            return (!todo.isEmpty() || current != null);
        }

        public void remove() { // we cannot remove
            throw new UnsupportedOperationException();
        }
    }
    
    //end of the multiline comment 

}

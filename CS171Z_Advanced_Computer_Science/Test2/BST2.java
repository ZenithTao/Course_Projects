/*************************************************************************
 *  Compilation:  javac -cp :../stdlib.jar BST2.java
 *  Execution:    java -cp :../stdlib.jar BST2
 *
 *  A Binary Search Tree implementation with some fancy
 *  pointer manipulations in MorrisTraversal(), remove(), and
 *  findMedian().
 * 
 *************************************************************************/

public class BST2<Item extends Comparable<Item>> {
    private Node root;             // root of BST

    private class Node {
        private Item item;          
        private Node left, right;  // left and right subtrees
        private int N;             // number of nodes in subtree

        public Node(Item i, int n) {
            this.item = i;
            this.N = n;
	    this.left = null;
	    this.right = null;
        }
    }

    public BST2() {
	root = null;
    }

    public boolean isEmpty() {
        return size() == 0;
    }

    // return number of nodes in the tree
    public int size() {
        return size(root);
    }

    // return number of nodes rooted at x
    private int size(Node x) {
        if (x == null) return 0;
        else return x.N;
    }

   /***********************************************************************
    *  Search BST for given item, returns true or false
    ***********************************************************************/
    // does there exist a key-value pair with given key?
    public boolean contains(Item i) {
        return found(i,root);
    }

    private boolean found(Item i,Node x) {
        if (x == null) return false;
        int cmp = i.compareTo(x.item);
        if (cmp < 0) 
	    return found(i,x.left);
        else if (cmp > 0) 
	    return found(i,x.right);
        else 
	    return true;
    }


   /***********************************************************************
    *  Insert new node if it does not exists
    ***********************************************************************/
    public void put(Item i) {
        root = put(root, i);
    }

    private Node put(Node x, Item i) {
        if (x == null) 
	    return new Node(i, 1);

        int cmp = i.compareTo(x.item);
        if (cmp < 0)
	    x.left  = put(x.left,  i);
        else if (cmp > 0)
	    x.right = put(x.right, i);
        else ; // already exists, don't do anything

        x.N = 1 + size(x.left) + size(x.right);
        return x;
    }

   /***********************************************************************
    *  Remove a node 
    ***********************************************************************/

    public void remove(Item i) {
	root = remove(root,i);
    }

    private Node remove(Node x, Item i) {
        if (x == null) return null;

        int cmp = i.compareTo(x.item);
        if (cmp == 0)
            if (x.left == null)
                return x.right;

            else if (x.right == null)
                return x.left;
            else {
                Node min = removeMin(x.right);
                x.item = min.item;
                x.N = size(x.left) + size(x.right) + 1;
                return x;
            }
        else if (cmp > 0)
            x.right = remove(x.right,i);
        else
            x.left = remove(x.left,i);

        x.N = size(x.left) + size(x.right) + 1;
        return x;

    }

    /** removeMin(), helper to remove() ***/

    private Node removeMin(Node x) {
       if (x.left == null) 
	   return x.right;

       x.left = removeMin(x.left);

       x.N = size(x.left) + size(x.right) + 1;
       return x;
    }

   /***********************************************************************
    *  good exercise: find median with the help of knowing the size
    ***********************************************************************/

    public Item findMedian() {
	int median = (size(root) + 1) / 2;

	return findNth(root,median);
    }

    private Item findNth(Node x, int nth) {
	if (nth == size(x.left) + 1)
	    return x.item;
	else if (nth <= size(x.left))
	    return findNth(x.left, nth);
	else
	    return findNth(x.right, nth - size(x.left) - 1);
    }

   /* Function to traverse binary tree without recursion and
      without stack */
    private void MorrisTraversal() {
        Node current, pred;

	current = root;

	if(current == null) return;
 
	while(current != null) {
	    if(current.left == null) {
		System.out.println("r1: " + current.item);
		current = current.right;
	    }
	    else {
		/* Find the inorder predecessor of current */
		pred = current.left;
		while(pred.right != null && pred.right != current)
		    pred = pred.right;
 
		/* Make current as right child of its inorder predecessor */
		if(pred.right == null) {
		    pred.right = current;
		    current = current.left;
		}
 
		/* Revert the changes made in if part to restore the original
		   tree i.e., fix the right child of predecssor */
		else {
		    pred.right = null;
		    System.out.println("r2: " + current.item);
		    current = current.right;
		} 
	    } 
	}

    }

   /***********************************************************************
    *  Display the tree -- a preorder traversal
    ***********************************************************************/
   
    public void print() {
	print(root,0);
    }

    private void print(Node x, int level) {
	// indent based on the level
	for (int i = 0; i < level; i++) 
	    System.out.print(" | ");
	    
	if (x != null) {
	    // print the node
	    System.out.println(x.item);
	    
	    // print all the left subtree
	    print(x.left, level+1);

	    for (int i = 0; i < level; i++) 
		System.out.print(" | ");
	    System.out.println("(" + level + ")");
	    
	    // print all the right subtree	    
	    print(x.right, level+1);
	}
	else 
	    System.out.println("*");
    }

   /*****************************************************************************
    *  Test client
    *****************************************************************************/

    public static void helpMenu() {
	System.out.println("Usage: java -cp :../stdlib.jar BST2\n");
	System.out.println(" c\tclear the tree; start with an empty tree");
	System.out.println(" h\tthis help menu");
	System.out.println(" i <n>\tthis insert <n>");
	System.out.println(" l\tlist the content in order");
	System.out.println(" m\tshow the median value");
	System.out.println(" p\tprint the tree structure");
	System.out.println(" r <n>\tremove <n>\n");
    }

    public static void main(String[] args) { 

        BST2<Integer> st = new BST2<Integer>();

	System.out.println("h (for help)");
	while (! StdIn.isEmpty()) {
	    char a = StdIn.readChar();
	    if (a == 'c')       // clear -- start over
		st = new BST2<Integer>();
	    else if (a == 'h')
		helpMenu();
	    else if (a == 'i')  // insert
		st.put(StdIn.readInt());
	    else if (a == 'l')  // list the content in order 
		st.MorrisTraversal();
	    else if (a == 'm')  // find the median
		System.out.println("median: " + st.findMedian());
	    else if (a == 'p')  // print the tree structure
		st.print();
	    else if (a == 'r')  // remove
		st.remove(StdIn.readInt());
	    else;               // ignore
	}

	System.out.println("Ciao ... Tusch ... Au revoir ...");

    }
}

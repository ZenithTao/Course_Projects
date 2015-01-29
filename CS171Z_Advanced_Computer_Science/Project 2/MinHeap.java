// this is the priority class
class Heap {
	private int[] itarray;
	private int N;

	public Heap(int capacity) {
		N = 0;
		itarray = new int[capacity];
	}

	public boolean isEmpty() {
		return N == 0;
	}

	public void insert(int path) {
		if(N == itarray.length - 1)
			resize(2 * itarray.length);
		itarray[N] = path;
		trikleUp(N++);
	}

	// a helper function to help insert an element
	public void trikleUp(int index) {
		int parent = (index - 1) / 2;
		int last = itarray[index];

		while (index > 0 && itarray[parent] > last) {
			itarray[index] = itarray[parent];
			index = parent;
			parent = (parent - 1) / 2;
		}
		itarray[index] = last;
	}


	public int remove() {
		int root = itarray[0];
		if(N > 0 && N == itarray.length / 4)
			resize(itarray.length / 2);
		itarray[0] = itarray[--N];
		trikleDown(0);
		return root;
	}

	// a helper functino to help to remove an element
	public void trikleDown(int index) {
		int largerChild;
		int top = itarray[index];

        while (index < (N / 2)) {
            int leftChild = 2 * index + 1;
            int rightChild = leftChild + 1;
            
            if (rightChild < N && itarray[leftChild] > (itarray[rightChild])) 
                largerChild = rightChild;
            else 
            	largerChild = leftChild;

            if(top <= itarray[largerChild])
            	break;
    
            itarray[index] = itarray[largerChild];
            index = largerChild;
        }        
        itarray[index] = top;
    }

	// a helper function to resize the array
	public void resize(int capacity) {
		int[] copy = new int[capacity];
		for(int i = 0; i < N; i++) {
			copy[i] = itarray[i];
		}
		itarray = copy;
	}
}

public class MinHeap {
	    public static void main(String[] args) {
    	Heap x = new Heap(100);
    	x.insert(100);
    	x.insert(100);
    	x.insert(120);
    	x.insert(10000);
    	x.insert(11);
    	x.insert(12);
    	x.insert(1);

    	System.out.println(x.remove());
    	System.out.println(x.remove());
    	System.out.println(x.remove());
    	System.out.println(x.remove());
    	System.out.println(x.remove());
    	System.out.println(x.remove());
    	    	System.out.println(x.remove());
    	    	    	System.out.println(x.remove());




    }
}





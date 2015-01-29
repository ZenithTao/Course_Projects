/*************************************************************************
 *  Compilation:  javac Merge.java
 *  Execution:    java Merge N
 *
 *  A more generic version of mergesort using Comparable.
 *
 *************************************************************************/

public class MergeSort {

    Comparable [] theArray;

    public MergeSort(Comparable[] given) {
	theArray = given;
    }
    public void sort() {
        sort(0, theArray.length);
    } 

    // Sort theArray[lo, hi). 
    public void sort(int lo, int hi) {
        int N = hi - lo;        // number of elements to sort

        // 0- or 1-element file, so we're done
        if (N <= 1) return; 

        // recursively sort left and right halves
        int mid = lo + N/2; 
        sort(lo, mid); 
        sort(mid, hi); 

        // merge two sorted subarrays
        Comparable[] aux = new Comparable[N];
        int i = lo, j = mid;

        for (int k = 0; k < N; k++) {
            if      (i == mid)  
		aux[k] = theArray[j++];
            else if (j == hi)   
		aux[k] = theArray[i++];
            else if (theArray[j].compareTo(theArray[i]) < 0) 
		aux[k] = theArray[j++];
            else                               
		aux[k] = theArray[i++];
        }

        // copy back
        for (int k = 0; k < N; k++) {
            theArray[lo + k] = aux[k]; 
        }
    } 



   /***********************************************************************
    *  Check if array is sorted - useful for debugging
    ***********************************************************************/
    private boolean isSorted() {
        for (int i = 1; i < theArray.length; i++)
            if (theArray[i].compareTo(theArray[i-1]) < 0) return false;
        return true;
    }

   /***********************************************************************
    *  Show results
    ***********************************************************************/
    public void show() {
        for (int i = 0; i < theArray.length; i++)
            System.out.println(theArray[i]);
    }


    // test client
    public static void main(String[] args) {

	if (args.length < 1) {
	    System.out.println("Usage: java MergeSort N");
	    System.exit(0);
	}

        int N = Integer.parseInt(args[0]);

        // generate N random real numbers between 0 and 1
        long start = System.currentTimeMillis();
        Double[] a = new Double[N];  // Note that Double implements Comparable, 
	                             // but double does not
        for (int i = 0; i < N; i++)
            a[i] = Math.random();
        long stop = System.currentTimeMillis();
        double elapsed = (stop - start) / 1000.0;
        System.out.println("Generating input:  " + elapsed + " seconds");

	// create an instance of the MergeSort data type
	MergeSort myMerge = new MergeSort(a);

        // sort them
        start = System.currentTimeMillis();
        myMerge.sort();
        stop = System.currentTimeMillis();
        elapsed = (stop - start) / 1000.0;
        System.out.println("Mergesort: " + elapsed + " seconds");
        System.out.println(myMerge.isSorted());

        if (N < 100) myMerge.show();
    }
}

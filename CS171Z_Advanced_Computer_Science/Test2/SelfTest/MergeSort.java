import java.util.*;

public class MergeSort {
	public static void main(String[] args) {
		Random rand = new Random(13234);
		int[] mn = new int[10000];
		for(int i = 0; i <= 9999; i++) {
			mn[i] = rand.nextInt(134234234);
		}

		sort(mn);
		for(int x : mn) System.out.print(x + " ");
	}

	public static void sort(int[] theArray) {
		msort(0, theArray.length, theArray);
	}

	public static void msort(int lo, int hi, int[] theArray) {
		int N = hi - lo;

		if(N <= 1) return;

		int mid = lo + N/2;
		msort(lo, mid, theArray);
		msort(mid, hi, theArray);

		int[] copy = new int[N];
		int i = lo, j = mid;

		for(int k = 0; k < N; k++) {
			if(i == mid)
				copy[k] = theArray[j++];
			else if (j == hi)
				copy[k] = theArray[i++];
			else if(theArray[i] < theArray[j])
				copy[k] = theArray[i++];
			else 
				copy[k] = theArray[j++];
		}

		for(int k = 0; k < N; k++)
			theArray[k + lo] = copy[k];

	}
}
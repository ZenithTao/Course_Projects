/*************************************************************************
 *  Compilation:  javac Amicable.java
 *  Execution:    java Amicable N
 *  
 *  Purpose:      
 *  Prints all amicable pairs whose sum is less than or equal to N
 *************************************************************************/

public class AmicHashh {
    public static int sumOfDivisors(int k) {
	int sum = 0;
	for (int i = 1; i < k; i++) 
	    if (k % i == 0) 
		sum += i;
	return sum;
    }

    public static void main(String[] args) {

	if (args.length < 1) {
	    System.out.println("Usage: java Amicable N");
	    System.exit(0);
	}

	int N = Integer.parseInt(args[0]);

	for (int i = 1; i <= N; i++) {
	    int sumOfI = sumOfDivisors(i);
	    if (i < sumOfI && i + sumOfI < N)
		if (sumOfDivisors(sumOfI) == i)
		    System.out.println("(" + i + "," + sumOfI + ")");		
	    }
    }
}

	    
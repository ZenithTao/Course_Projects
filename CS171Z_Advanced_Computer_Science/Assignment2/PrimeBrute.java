/*************************************************************************
 *  Compilation:  javac PrimeBrute.java
 *  Execution:    java PrimeBrute N
 *  
 *  Purpose:      Computes the prime numbers <= N
 *  Approach:     brute force: for each number between 2 and N, check if
 *                it is a prime number
 *************************************************************************/


public class PrimeBrute {

    public static boolean prime(int n) {
	// pre: n a non-negative integer
	// post: true if n is prime, false otherwise
	boolean isPrime = true;
	if (n < 2) isPrime = false; // the only even prime

	// try all possible factors i of N
	// if if N has a factor, then it has one less than or equal to sqrt(N),
	// so for efficiency we only need to check i <= sqrt(N) or equivalently i*i <= N
	for (long i = 2; i*i <= n; i++) {

	    // if i divides evenly into N, N is not prime, so break out of loop
	    if (n % i == 0) {
		isPrime = false;
		break;
	    }
	}
	return isPrime;
    }

    public static void main(String[] args) { 

	if (args.length < 1) {
	    System.out.println("Usage: java PrimeSieve N [-s(ilent)]");
	    System.exit(0);
	}

        int N = Integer.parseInt(args[0]);

        int primes = 0;
        for (int i = 2; i <= N; i++) {

	    if (prime(i)){
		primes++;
		if (args.length == 2 && args[1].equals("-s"))
		    ; // do nothing
		else
		    System.out.print(i + " ");
	    }
	}

        System.out.println("\nThe number of primes <= " + N + " is " + primes);
    }
}
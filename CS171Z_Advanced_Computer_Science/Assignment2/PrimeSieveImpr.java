/*************************************************************************
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHERS. LIANG TAO
DATE SUBMIITED: 09/23/2013
 *  Compilation:  javac PrimeSieveImpr.java
 *  Execution  :   java PrimeSieveImpr 10000
*************************************************************************/


public class PrimeSieveImpr {

    public static void main(String[] args) { 

    if (args.length < 1) {
        System.out.println("Usage: java PrimeSieve N [-s(ilent)]");
        System.exit(0);
    }

        int N = Integer.parseInt(args[0]);

        // initially assume all integers are prime
    boolean[] isPrime = new boolean[N/2];
    for(int i = 1; i<N/2; i++){
        isPrime[i]=true;
    }
    int tripCount = 0;

        // mark non-primes <= N using Sieve of Eratosthenes
    int m = (int) Math.floor(Math.sqrt(N));
    for(int i = 1; i < m; i++){
        if(isPrime[i]){
        for(int j = 2 * i * (i + 1); j < N/2; j = j + 2 * i + 1){
            isPrime[j] = false;
            tripCount++;
        }
        }
    }
    System.out.println("Number of times in the inner loop: " + tripCount);

        // count and display primes
        System.out.print(2 + " ");
        int primes = 1;
        for (int i = 1; i < N/2; i++) {
            if (isPrime[i]) {
        primes++;
        if (args.length == 2 && args[1].equals("-s"))
            ; // do nothing
        else
            System.out.print((2 * i + 1) + " ");
        }
        }
        System.out.println("\nThe number of primes <= " + N + " is " + primes);
    }
}

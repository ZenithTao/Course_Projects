/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHERS. LIANG TAO

Compilation:  javac PrimeSievenew.java
Execution:    java -Xmx1100m PrimeSievenew N
   
Purpose:      Computes the prime numbers <= N
Approach:     the Sieve of Eratosthenes
*/

public class PrimeSievenew {
    public static void main(String[] args) { 

	   if (args.length < 1) {
	    System.out.println("Usage: java PrimeSieve N [-s(ilent)]");
	    System.exit(0);
	   }else{
      int N = Integer.parseInt(args[0]);
      new Eratosthenes(N);
     }
  }
}  

class Eratosthenes{
    public Eratosthenes(int N){
        boolean[] isPrime = new boolean[N + 1];
        for (int i = 2; i<=N; i++){
            isPrime[i] = true;
        }
        sieve(2);
        for (int i = 3; i <= Math.sqrt(N);i+=2){
            if(isPrime[i]){
                sieve(i);
            }
        }

        System.out.println("Number of times in the inner loop: " + tripCount);

        // count and display primes
        int primes = 0;
        for (int i = 2; i <= N; i++) {
          if (isPrime[i]) {
              primes++;
          }
          if (args.length == 2 && args[1].equals("-s")) {
          ; } // do nothing
          else { System.out.print(i + " ");
          }
             System.out.println("\nThe number of primes <= " + N + " is " + primes);
          }
      }

     // mark primes 
     public void sieve(int max){
        int tripCount = 0;
        int limit = max;
        int sieveBound = (limit - 1) /2;
        boolean isPirme = new boolean[sieveBound];
        int l = (int) Math.floor(Math.sqrt(limit));
            

    }
}

/*************************************************************************
 *  Compilation:  javac FibTail.java
 *  Execution:    java FibTail N
 *
 *  Computes and prints the first N Fibonacci numbers.
 *
 *  Uses tail-recursive implementation (counting up) of Fibonacci
 *
 *************************************************************************/

public class FibTail {
    public static long fibtail(long n2, long n1, int count, int n) {
	// note the two base cases combined
	// System.out.println(":" + n2 + " " + n1 + " " + count + " " + n);
        if (count > n) 
	    return n1;
        else 
	    return fibtail(n1, n2 + n1, count + 1, n);
    }

    public static long fib(int n) {
	return fibtail(0,1,2,n);
    }

    public static void main(String[] args) {
        int N = Integer.parseInt(args[0]);
        for (int i = 1; i <= N; i++)
            System.out.println(i + ": " + fib(i));
    }

}

/*************************************************************************
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHERS. LIANG TAO
DATE SUBMIITED: 09/23/2013
 *  Compilation:  javac Minvalue.java
 *  Execution  :   java Minvalue 3333

 % java Minvalue N;
*************************************************************************/

import java.util.*;

public class Minvalue{
	public static void main(String[] args) {

		int N = Integer.parseInt(args[0]);
		int[] arr = new int[N];
		Random rand = new Random();
		System.out.println("Input N is " + N);
		System.out.print("Numbers in the array [ ");
		//generate the values in the array
		for(int i = 0; i <= N - 1; i++){
			int x = rand.nextInt(N);
			arr[i] = x;
			System.out.print(arr[i] + " ");
		}
		System.out.println(" ]");

		System.out.println("The smallest number in the array is " + min(N,N,arr));
	}
		//start the main tail-recursion
	public static int min(int n, int mvalue, int[] marray){
		if(n < 1) return mvalue;
		else if(marray[n - 1] <= mvalue) return min(n-1, marray[n-1], marray);
		else return min(n-1, mvalue, marray);
	}
}
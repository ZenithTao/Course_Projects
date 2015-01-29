import java.math.BigInteger;

public class Combination{
	public static void main(String[] args) {

	int n = Integer.parseInt(args[0]);
	int k = Integer.parseInt(args[1]);

	if(k < (n / 2)){
		System.out.println("The result of combination is " + recurCal(BigInteger.valueOf(0), n, k));
	} else {
		System.out.println("The result of combination is " + recurCal(BigInteger.valueOf(0), n, n - k));
	}
	}
	public static BigInteger recurCal(BigInteger sum, int n, int k){
		if(n < 1 || k < 1) return sum;
		else if(n == 1 && k == 1) return sum.add(BigInteger.valueOf(1));
		else if(n == 1 && k > 1) return sum;
	    else if(n > 1 && k == 1) return (sum.add(BigInteger.valueOf(1)).add(BigInteger.valueOf(n - 1)));
	    else return (sum.add(recurCal(sum, n - 1, k - 1))).add(recurCal(sum, n - 1, k));
	}
}
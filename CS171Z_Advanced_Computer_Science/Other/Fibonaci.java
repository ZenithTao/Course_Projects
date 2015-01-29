public class Fibonaci{
	public static int fibonaci(int n2, int n1, int count, int limit){
		if (count > limit){
			return n1;
		} else {
			return fibonaci(n1, n1 + n2, count + 1, limit);
		}
	}

	public static void main(String args[]){
		int N = Integer.parseInt(args[0]);
		for (int i = 1; i <= N; i++){
		System.out.println(fibonaci(0, 1, 2, i));
		}
	}
}
public class Pyth{
	public static void main(String[] args) {
		int N = Integer.parseInt(args[0]);
		int count = 0;
		int a, b, c;
		search:
		for(int m = 1; m <= Integer.MAX_VALUE; m++){
			for(int n = m+1; n <= Integer.MAX_VALUE; n++){
				a = m*m - n*n;
				b = 2*m*n;
				c = m*m + m*m;
				if(a*a + b*b == c*c){
					System.out.printf("Found a Pythagorean triple: %d  %d  %d \n", a, b, c);
					count++;
				}
				if (count == N){
					break search;
				}
			}
		}
	}
}
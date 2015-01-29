public class Amichash{
	public static int sumOfDivision(int n){
		int sum = 0;
		for(int i = 1; i <= n; i++){
			if (n % i == 0) sum += i;
		}
		return sum;
	}

	public static void main(String[] args){
		int N = Integer.parseInt(args[0]);
		for(int i = 1; i <= N; i++){
			int sumi = sumOfDivision(i);
			if(i < sumi && i + sumi < N){
				if( sumOfDivision(sumi) == i){
					System.out.print(i + " " + sumi);
				}
			}
		}

	}
}
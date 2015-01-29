public class Isqrt{
	public static void main(String[] args) {
		int N = Integer.parseInt(args[0]);
		int i = 0;
		while(!((i - 1) * (i - 1) < N && i * i >= N)){
			i++;
		}
		System.out.println(i);
	}
}
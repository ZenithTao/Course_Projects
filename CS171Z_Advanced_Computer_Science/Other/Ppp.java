public class Ppp{
	public static void main(String[] args) {
		double N = Double.parseDouble(args[0]);
		double l = 0;
		double u = N;
		double m = (u + l) / 2;
		double epsilon = 1e-15;

		while(Math.abs(m - N/m) > epsilon * m){
			if (m * m < N){
				l = m;
			} else if (m * m > N){
				u = m;
			}
				m = (u + l)/2;
			System.out.println(m);
		}
			System.out.println(m);
	}
}
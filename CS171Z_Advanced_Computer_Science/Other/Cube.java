/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHERS. LIANG TAO
*/

public class Cube {
	public static void main(String[] args){
		double N = Double.parseDouble(args[0]);
		double guess = 1;
		double e0 = Math.abs(guess*guess*guess - N);

		if(args.length>1){
			System.out.print("Wrong arguments, please input only one number");
			System.exit(1);
		}

		do{
			guess = guess + ((N/(guess*guess)-guess)/3);
			e0 = Math.abs(guess*guess*guess - N);
			System.out.println(guess);
		} while(e0>1.0e-6);
	}
}
/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHERS. LIANG TAO
*/

public class Pythtriples{
	public static void main(String[] args) {
		long x, y, z;
		int count=0;
		int N= Integer.parseInt(args[0]);

		System.out.println("The first " + N + " Pythagorean Triples are : ");
		search:{
		for(x=1;x<Long.MAX_VALUE;x++){
			for(y=x;y>0;y--){
				for(z=y;z>0;z--){
					if(x*x == y*y + z*z){
						System.out.println("Found a Pythagorean Triples: " + x + " "
							+ y + " "+ z + " ");
						count++;
						if(count==N){
							break search;
						}
					}
				}
			}
		}} 
	}
}
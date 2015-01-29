import java.util.*;
public class Ioo{
	public static void main(String[] args) {
		Random rand = new Random();
		ArrayList al = new ArrayList();
		int n = 0;
		while(n<1000){
			int xxx = rand.nextInt(1000);
			if(!al.contains(xxx)){
				n++;
				al.add(xxx);
			}
		}
		System.out.print(al);
	}
}
import java.util.*;

public class Ooo{
	public static void main(String[] args) {
		List<Double> abc = new ArrayList<Double>(Arrays.asList(1.1,2.3,3.4,4.1,5.5));
		display(abc.iterator());
	}
	public static void display(Iterator<Double> it){
		while(it.hasNext()){
			Double p = it.next();
			System.out.print(p + " ");
		}
	}
}
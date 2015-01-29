import java.util.*;

public class Poo{
	public static void main(String[] args) {
		ArrayList xyz = new ArrayList();
		xyz.add(1);
		xyz.add(2);
		xyz.add(3.5);
		for(Object x : xyz) System.out.print(x);
	}
}
import java.util.*;

public class Ppp{
	public static void main(String[] args) {
		LinkedList<String> xyz = new LinkedList<String>();
		for(int i = 1; i <= 10; i++){
			String x = "abcdefg";
			if(xyz.insert(x)) System.out.println(xyz.insert(x));
		}

	}
}
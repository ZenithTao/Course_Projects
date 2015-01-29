import java.util.Calendar;
public class Dddmain{
	public static void main(String[] args) {

	Calendar calendar = Calendar.getInstance();
   	int y = calendar.get(Calendar.MONTH);
   	int m = calendar.get(Calendar.YEAR);
   	System.out.println(y + "   " + m);
   	
 }
}

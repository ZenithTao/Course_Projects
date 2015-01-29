import java.util.Calendar;

public class Bbb{
  public static void main(String[] args){
  	
  	if( args.length == 1 && args[0].equals("cal")) {
	    Calendar calendar = Calendar.getInstance();
   	    int y = calendar.get(Calendar.MONTH);
   		int m = calendar.get(Calendar.YEAR);
   		System.out.print( y + "   " + m);
   	}
  }
}
import java.util.Calendar;

class Cal{
   private static int year;
   private static int month;
   

   //One args
    Cal(){
   	Calendar calendar = Calendar.getInstance();
   	int m = calendar.get(Calendar.MONTH);
    int y = calendar.get(Calendar.YEAR);
    System.out.print(" Current time: Year " + y + "  Month " + m);
   }
   
   //Two args
    Cal(int month, int year){
   	
	char[] all = new char[20];
	char[] monthname = (getMonth(month)).toCharArray();
	int monthlength = monthname.length;
	int frontlength;

	//Show the year & month in the required format
	if (monthlength % 2 !=0 ){
		frontlength = (19-monthlength-5)/2;
		for(int p=0;p<=frontlength;p++){
		all[p] = ' ';
		System.out.print(all[p]);
		}
		System.out.println(getMonth(month) + " " + year);
	} else {
		frontlength = (19-monthlength-6)/2;
		for(int p=0;p<=frontlength;p++){
		all[p] = ' ';
		System.out.print(all[p]);
		}
		System.out.println(getMonth(month) + " " + year);
	}

		System.out.println("Su " + "Mo " + "Tu " + "We " + "Th " + "Fr " + "Sa ");
	}
  
  public static String getMonth(int month){
  	String monthname = null;
    switch(month){
	case 1: monthname = "January"; break;
	case 2: monthname = "Feburary"; break;
	case 3: monthname = "March"; break;
	case 4: monthname = "April"; break;
	case 5: monthname = "May"; break;
	case 6: monthname = "June"; break;
	case 7: monthname = "July"; break;
	case 8: monthname = "August"; break;
	case 9: monthname = "September"; break;
	case 10: monthname = "October"; break;
	case 11: monthname = "November"; break;
	case 12: monthname = "December"; break;
	default: monthname = "Invalid month"; break;
	}
	 return monthname;
    }
}
}

//main method
public class C{
  public static void main(String[] args){
  	
  	if( args[0].equals("cal") && args.length ==1 ) {
  		new Cal();
 	}else if(args[0].equals("cal") && args.length ==3) {
  		new Cal(Integer.parseInt(args[1]), Integer.parseInt(args[2]));
    }
  } 
}







	
/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *
 *  DATE SUBMIITED: 12/03/2013
 *  Compilation:  javac LinkedList.java
 *  Execution:    java LinkedList
 *************************************************************************/
import java.util.Calendar;

class Cal{
   private static int year;
   private static int month;
   

   //One args
   public Cal(){
   	Calendar calendar = Calendar.getInstance();
   	int m = calendar.get(Calendar.MONTH);
    int y = calendar.get(Calendar.YEAR);
    System.out.println(" Current time: Year " + y + "  Month " + m);
   }
   
   //Two args
   public Cal(int month, int year){

    if((month>12) || (month<1) || (year<=0)){
    	System.out.print("Wrong number of month/year, please input the right ones: ");
    	System.exit(1);
    }else{

	char[] all = new char[20];
	char[] MONTH_NAME = (getMonth(month)).toCharArray();
	int MONTH_LENGTH = MONTH_NAME.length;
	int FRONT_LENGTH;

	//Print the year & month in the required format
	if (MONTH_LENGTH % 2 !=0 ){
		FRONT_LENGTH = (19-MONTH_LENGTH-5)/2;
		for(int p=0;p<=FRONT_LENGTH;p++){
		all[p] = ' ';
		System.out.print(all[p]);
		}
		System.out.println(getMonth(month) + " " + year);
	} else {
		FRONT_LENGTH = (19-MONTH_LENGTH-6)/2;
		for(int p=0;p<=FRONT_LENGTH;p++){
		all[p] = ' ';
		System.out.print(all[p]);
		}
		System.out.println(getMonth(month) + " " + year);
	}

		System.out.println("Su " + "Mo " + "Tu " + "We " + "Th " + "Fr " + "Sa ");
    

	//Print the days in the required format
	int y0 = year - (14 - month)/12;
	int x = y0 + y0/4 - y0/100 + y0/400;
	int m0 = month + 12*((14 - month)/12) - 2;
	int d0 = (1 + x + (31 * m0)/12) % 7;


	int[] daysofmonth = new int[getDays(month,year)];
	for(int t=0; t<=(getDays(month,year)-1);t++){
		daysofmonth[t] = (t+1);
	}

	char[] blank = new char[d0*3+1];
	for(int a=1; a<=d0*3+1; a++){
		System.out.print(' ');
	}

	for(int l=0; l<=(getDays(month,year)-1); l++){
		if (daysofmonth[l]==1){
			System.out.print(daysofmonth[l] + " ");
		}
		else if(daysofmonth[l]<=9 && daysofmonth[l]>=2){
			System.out.print(" " + daysofmonth[l] + " ");
		} else {
			System.out.print( daysofmonth[l] + " ");
		}
		if(l%7==(6-d0)){
			System.out.println();
		}
	  }
    }
  }

   //test the Leap year
   public static boolean Leap(int year){
	if ((year % 4 == 0) && (year % 100 != 0))
	    return true;

	// divisible by 4, by 100, and by 400 -> true
	else if (((year % 4 == 0) && (year % 100 == 0) && (year % 400 == 0)))
	    return true;

	// divisible by 4 but also by 100 -> false
	else if ((year % 4 == 0) && (year % 100 == 0)) 
	    return false;


	// none of the above -> false
	else 
	    return false;
   }
  

  //get the day with month$year
  public static int getDays(int month,int year){
  	int days = 0 ;
	if ((Leap(year)==true) && (month==2)){
	   days = 29;
	} else {
	  switch(month){
	  case 1: days = 31; break;
	  case 3: days = 31; break;
	  case 4: days = 30; break;
	  case 5: days = 31; break;
	  case 6: days = 30; break;
	  case 7: days = 31; break;
	  case 8: days =31; break;
	  case 9: days =30; break;
	  case 10: days = 31; break;
	  case 11: days = 30; break;
	  case 12: days = 31; break;
	  default: days = 28; break;
	  }
	}
	  return days;
  }
  
  //get the month name with month
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



//main method
public class Calend{
  public static void main(String[] args){
  	
  	if( args.length == 0) {
  		new Cal();
 	}else if(args.length == 2) {
  		new Cal(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
  		System.out.println();
    }
  } 
}







	
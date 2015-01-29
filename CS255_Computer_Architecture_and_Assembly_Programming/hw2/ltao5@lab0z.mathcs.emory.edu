// Liang Tao
// Email: liang.tao@emory.edu    EmoryId: 2019389
//I worked on this assignment alone, using only this semester's course materials.

public class OctoNumber {
  // first required method, covert a octodecimal string to corresponding 
  //  Integer
  public static int parseOcto(String s) {
  	int startpos, sign;
  	int value;
  	int i;

  	if (s.charAt(0) == '-') {
  		sign = -1;
  		startpos = 1;
  	}
  	else {
  		sign = 1;
  		startpos = 0;
  	}

  	value = 0;
  	for (i = startpos; i < s.length(); i++) {
      int value_of_digit;

      // deal with the inappropriate input
      if(!isAppChar(s.charAt(i))) {
        System.out.print("Inappropriate input, please try again...");
        System.exit(1);
      } 

      /* if the character is between 0 and 9, then use 2's compliment to calculate 
          the acutal decimal number */
      if (s.charAt(i) >= '0' && s.charAt(i) <= '9')
  		   value_of_digit = (int)(s.charAt(i)) - (int)'0';
      else
      // or just use the helper method "transfer()""
         value_of_digit = transferToDec(s.charAt(i));
      

  		value = 18 * value + value_of_digit;

  	if (sign == -1)	
  		value = -value;
    }

    return value;
  }


  //second required task, convert a Integer to a Octodecimal string
  public static String toString(int a) {
    int sign, i, j;
    String result;
    int next_digit;
    char next_char;

    if (a == 0)
      return ("0");

    if (a < 0) {
      sign = -1;
      a = -a;
    } else {  
      sign = 1;
    }

    result = "";

    do {
      next_digit = a % 18;
      // if the next_digit is between 0 and 9
      if (next_digit >= 0 && next_digit <= 9) 
        next_char = (char) (next_digit + '0');
      else
        next_char = transferToChar(next_digit);

      result = next_char + result;
      // update the value
      a = a / 18;
    } while (a > 0);

    if (sign == -1)
      result = "-" + result;
      

    return result;

  }

    // a private method to transfer the character in Octodecimal to int
    public static int transferToDec(char c) {
      int m = 0;
      switch (c) {
        case '%': m = 10; break;
        case '^': m = 11; break;
        case '#': m = 12; break;
        case '$': m = 13; break;
        case '&': m = 14; break;
        case '*': m = 15; break;
        case '/': m = 16; break;
        case '@': m = 17; break;
      }
      return m;
    }

    // another helper method to transfer the decimal larger than 9 to char
    public static char transferToChar(int i) {
      char c = '?';
      switch(i) {
        case 10: c = '%'; break;
        case 11: c = '^'; break;
        case 12: c = '#'; break;
        case 13: c = '$'; break;
        case 14: c = '&'; break;
        case 15: c = '*'; break;
        case 16: c = '/'; break;
        case 17: c = '@'; break;
      }
      return c;
    }


  // a helper function to test whether the character is appropriate
    public static boolean isAppChar(char c) {
      if((c >= '0' && c <= '9') || (c == '%') || (c == '#') || (c == '^')
                                || (c == '$') || (c == '@') || (c == '&')
                                || (c == '*') || (c == '/'))
        return true;
      else
        return false;
    }

  // main method to test the function of the program.
  public static void main(String[] args) {
    // test the function of parseOcto
    String x = "10";
    System.out.println(parseOcto(x));
    // test the function of toString
    int y = 318;
    System.out.println(toString(y));
  }
}

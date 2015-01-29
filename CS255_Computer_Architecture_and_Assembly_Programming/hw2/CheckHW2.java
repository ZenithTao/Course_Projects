// Test program for cs255 hw2
//
// For hw2, you must write another program in file "OctoNumber.java"
// that contains the static methods:
//
//         public static int parseOcto(String s)
//         public static String toString(int a)
//
// Compile the program (after you have written "OctoNumber.java"
// with:
//         javac CheckHW2.java
//
// and run it with the command:
//         java CheckHW2
//

import java.util.Scanner;

public class CheckHW2 {
   public static void main (String[] args) {
      Scanner in = new Scanner(System.in);

      String s;
      int x, y, z;

      System.out.print ("Enter first Octodecimal number x = ");
      s = in.next();
      x = OctoNumber.parseOcto(s);  // Convert Octodecimal number to binary
      System.out.println ("\nCHECK - parseOcto() returns: " + x + "(decimal)");
      System.out.println ();

      

      System.out.print ("Enter second Octodecimal number y = ");
      s = in.next();
      y = OctoNumber.parseOcto(s);  // Convert octodecimal number to binary
      System.out.println ("\nCHECK - parseOcto() returns: " + y + "(decimal)");
      System.out.println ();

      z = x + y;
      System.out.println ("In Base 18, x + y = " + OctoNumber.toString(z) );

   }
}
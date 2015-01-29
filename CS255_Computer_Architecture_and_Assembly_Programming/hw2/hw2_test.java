// Test program for cs255 hw2
// DO NOT make any changes to this program
//
// For hw2, you must write another program in file "OctaNumber.java"
// that contains the static methods:
//
//         public static int parseOcta(String s)
//         public static String toString(int value)
//
// Compile the program (after you have written "OctaNumber.java"
// with:
//         javac hw2_test.java
//
// and run it with the command:
//         java hw2_test
//
//	Thanks to Dr. Cheung for the test code.

import java.util.Scanner;


public class hw2_test
{
   public static void main (String[] args) 
   {
      Scanner in = new Scanner(System.in);

      String s;
      int x, y, z;

      System.out.print ("Enter the first Octadecimal number x = ");
      s = in.next();
      x = OctoNumber.parseOcto(s);  // Convert octadecimal number to binary
      System.out.println ("CHECK - parseOcta() returns: " + x + "(decimal)");
      System.out.println ();

      

      System.out.print ("Enter second Octadecimal number y = ");
      s = in.next();
      y = OctoNumber.parseOcto(s);  // Convert octadecimal number to binary
      System.out.println ("CHECK - parseOcta() returns: " + y + "(decimal)");
      System.out.println ();

      z = x + y;
      System.out.println ("In Base 18, x + y = " + OctoNumber.toString(z) );

   }
}
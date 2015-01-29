/*************************************************************************
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHERS. LIANG TAO
DATE SUBMIITED: 09/23/2013
 *  Compilation:  javac -cp :./stdlib.jar BouncingBallnew.java
 *  Execution 1 :   java -cp :./stdlib.jar BouncingBallnew 222
 *  Execution 2 :   java -cp :./stdlib.jar BouncingBallnew -d 222
 *  Dependencies: StdDraw.java or stdlib.jar
*************************************************************************/

import java.util.*;
public class BouncingBallnew { 
    public static void main(String[] args) {
        public boolean flag = false;

        if(args.length == 1){
            int N = Integer.parseInt(args[0]);
            new BB(N, flag);
        } else if(args[0].equals("-d") && args.length == 2){
            int N = Integer.parseInt(args[1]);
            flag = true;
            new BB(N, flag);
        } else {
            System.out.println("Wrong input arguments, plese type in the right ones....");
            System.exit(1);
        }
    }
}


class BB{
    public BB(int N, boolean flag){

        // set the scale of the coordinate system
        StdDraw.setXscale(-1.0, 1.0);
        StdDraw.setYscale(-1.0, 1.0);

        // initial values
        double rx = 0.480, ry = 0.860;     // position
        double vx = 0.015, vy = 0.023;     // velocity
        double radius = 0.05;              // radius

        // test the value of N
        if(N > 2500){
            System.out.println("Input N is too large, please choose a smaller number: ");
            System.exit(0);
        }
            
        // establish the Array recording the the location of filled square
        ArrayList<Integer> record = new ArrayList<Integer>();
        Random rand = new Random();
        int n = 0;
        
        while(n < N){
            int i = rand.nextInt(2500);
            if(!record.contains(i)) record.add(i);
            n++;
        }


        // main animation loop
        while (true)  { 
             // collision with the obstacles
            for(int i=0; i < record.size(); i++){
                int m = record.get(i);
                double p = -1 + ((m%50)*0.04 + 0.02);
                double q = -1 + ((m/50)*0.04 + 0.02);
                double distance = Math.sqrt(Math.pow(Math.abs((rx - p)),2) + Math.pow(Math.abs((ry - q)),2));
                if(distance < 0.07){
                    record.remove(i);
                    if(flag = true) {
                        int nn = N-1;
                        while(nn < N){
                        int ii = rand.nextInt(2500);
                             if(!record.contains(ii)) record.add(ii);
                             nn++;
                        }
                    } else {
                        if(Math.abs(rx + vx) > Math.abs(p) - 0.02) vx = -vx;
                        if(Math.abs(ry + vy) > Math.abs(q) - 0.02) vy = -vy;
                        break;
                    }
                }
            }



             // bounce off wall according to law of elastic collision
             if (Math.abs(rx + vx) > 1.0 - radius) vx = -vx;
             if (Math.abs(ry + vy) > 1.0 - radius) vy = -vy;


            // update position
            rx = rx + vx; 
            ry = ry + vy; 

            // clear the background
            StdDraw.clear();
            StdDraw.setPenColor(StdDraw.BLACK);
            StdDraw.filledSquare(0,0,1.0);

            // draw ball on the screen
            StdDraw.setPenColor(StdDraw.RED); 
            StdDraw.filledCircle(rx,ry,radius);


            // draw obstacles
            for(int m : record){
                double p = -1 + ((m%50)*0.04 + 0.02);
                double q = -1 + ((m/50)*0.04 + 0.02);
                StdDraw.setPenColor(StdDraw.YELLOW); 
                StdDraw.filledSquare(p,q,0.02);  
            }

            // display and pause for 20 ms
            StdDraw.show(10); 
            }
        }
    }
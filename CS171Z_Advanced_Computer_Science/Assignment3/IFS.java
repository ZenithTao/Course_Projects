/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *  DATE SUBMIITED: 10/07/2013************************************************************************
 *  Compilation:  javac -cp :./stdlib.jar IFS.java
 *  Execution:    java -cp :./stdlib.jar IFS N + (N*3 numbers) < input.txt
 *  Dependencies: StdDraw.java
 *
 *  command: java -cp :./stdlib.jar IFS 100000 0.17 1.0 1.0 0.67 1.0 1.0 0 1.0 1.0 < sierpinski.txt
 *************************************************************************/

import java.awt.Color;
public class IFS {
    public static void main(String[] args) {

	if (args.length < 1) {
	    System.out.println("Usage: java IFS T (number of points to plot)");
	    System.exit(0);
	}

        // number of iterations
        int T = Integer.parseInt(args[0]);
        float a = Float.parseFloat(args[1]);
        float b = Float.parseFloat(args[2]);
        float c = Float.parseFloat(args[3]);
        float d = Float.parseFloat(args[4]);
        float e = Float.parseFloat(args[5]);
        float f = Float.parseFloat(args[6]);
        float g = Float.parseFloat(args[7]);
        float h = Float.parseFloat(args[8]);
        float i = Float.parseFloat(args[9]);

        // probability distribution for choosing each rule
        double[] dist = StdArrayIO.readDouble1D();

        // update matrices
        double[][] cx = StdArrayIO.readDouble2D();
        double[][] cy = StdArrayIO.readDouble2D();

        // current value of (x, y)
        double x = 0.0, y = 0.0;

  
        // do T iterations of the chaos game
        for (int t = 0; t < T; t++) { 

            // pick a random rule according to the probability distribution
            int r = StdRandom.discrete(dist); 

            // do the update
            double x0 = cx[r][0]*x + cx[r][1]*y + cx[r][2]; 
            double y0 = cy[r][0]*x + cy[r][1]*y + cy[r][2]; 
            x = x0; 
            y = y0; 

            // draw the resulting point
          if (x >= 0 && x <= 0.5 && y >=0 && y <= 0.433){
            StdDraw.setPenColor(Color.getHSBColor(a, b, c));
            StdDraw.point(x,y);
          } else if (x >= 0.5 && x <= 1.0 && y >=0 && y <= 0.433){
            StdDraw.setPenColor(Color.getHSBColor(d, e, f));
            StdDraw.point(x,y);
          } else {
            StdDraw.setPenColor(Color.getHSBColor(g, h, i));
            StdDraw.point(x,y);      
          }

            // for efficiency, display only every 100 iterations
            if (t % 100 == 0) StdDraw.show(10);
        } 

        // ensure everything gets drawn
        StdDraw.show(0);
    } 
} 


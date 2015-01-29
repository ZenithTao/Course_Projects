/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *
 *  DATE SUBMIITED: 10/26/2013
 *  Compilation:  javac -cp :./stdlib.jar Time.java
 *  Execution:    java  -cp :./stdlib.jar Time
 *  Dependencies: StdDraw.java
 *************************************************************************/

public class FunctionGraphNeww{
    public static void main(String[] args) {

	if (args.length < 1) {
	    System.out.println("Usage: java FunctionGraph <N>");
	    System.exit(0);
	}

        // number of line segments to plot
        int N = Integer.parseInt(args[0]);

        // number of pairs
        int n = StdIn.readInt();

        // read the input numbers
		double[] coef = new double[n];
		double[] expo = new double[n];

		double[] x = new double[N + 1];
		double[] y = new double[N + 1];
		double[] xn = new double[N + 1];
		double[] yn = new double[N + 1];

		for(int i = 0; i < n; i++){
			coef[i] = StdIn.readDouble();
			expo[i] = StdIn.readDouble();
		}

		
		for(int i = 0; i <= N; i++){
			x[i] = Math.PI * 10 * i / N;
			double SUM_Y = 0;
			for(int j = 0; j < n; j++){	
				SUM_Y += coef[j] * (Math.pow(x[i], expo[j]));
			}
			y[i] = SUM_Y;
		}

		for(int i = 0; i <= N; i++){
			xn[i] = - Math.PI * 10 * i / N;
			double SUM_Y = 0;
			for(int j = 0; j < n; j++){	
				SUM_Y += coef[j] * (Math.pow(xn[i], expo[j]));
			}
			yn[i] = SUM_Y;
		}

        // rescale the coordinate system
        StdDraw.setXscale(-Math.PI * 10, Math.PI * 10);
        StdDraw.setYscale(-200, 200);

        // plot the approximation to the function
    	for(int i = 0; i < N; i++){
    		StdDraw.line(x[i], y[i] , x[i+1], y[i+1]);
    		StdDraw.line(xn[i], yn[i] , xn[i+1], yn[i+1]);

   		}
   	}
}

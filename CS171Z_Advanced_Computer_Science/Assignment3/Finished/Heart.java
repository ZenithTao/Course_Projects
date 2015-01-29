public class Heart{
	public static void main(String[] args) {
	
	int N = 10000;
	double x[] = new double[N + 1];
	double y[] = new double[N + 1];
	double xx[] = new double[N + 1];
	double yy[] = new double[N + 1];
	

	for (int i = 0; i <= N; i++) {
		if (i%500==0)
	    x[i] = Math.PI * i / N;
	    y[i] = (Math.pow((x[i]/3), 2/3) - Math.pow((Math.pow(x[i]/3,4/3) - 4 * Math.pow(x[i]/3,2) + 4), 0.5)) / (2 / 3);
	}

	for (int i = 0; i <= N; i++) {
		if (i%500==0)
	    xx[i] = Math.PI * i / N;
	    yy[i] = (Math.pow((x[i]/3), 2/3) + Math.pow((Math.pow(x[i]/3,4/3) - 4 * Math.pow(x[i]/3,2) + 4), 0.5)) / (2 / 3);
	}
        // rescale the coordinate system
        StdDraw.setXscale(0, Math.PI);
        StdDraw.setYscale(-2.0, +2.0);

        // plot the approximation to the function
        for (int i = 0; i < N; i++) {
        if (i%500==0)
	    StdDraw.line(x[i], y[i], x[i+1], y[i+1]);
	    StdDraw.line(xx[i], yy[i], xx[i+1], yy[i+1]);
        }
    }
}

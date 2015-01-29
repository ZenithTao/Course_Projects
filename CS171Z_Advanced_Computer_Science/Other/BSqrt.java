public class BSqrt { 

    public static void main(String[] args) { 

    if (args.length < 1) {
        System.out.println("usage: java SSqrt N");
        System.exit(1);
    }

        // read in the command-line argument
        double c = Double.parseDouble(args[0]);
        double epsilon = 1e-15;    // relative error tolerance
        double u = c;              // estimate of the square root of c
    double l = 0;
    double m = (u + l) / 2;

    System.out.println(m);
        // repeatedly apply Newton update step until desired precision
        while (Math.abs(m - c/m) > epsilon * m) {
        if (m * m < c)
        l = m;
        else if (m * m > c)
        u = m;
        m = (u + l) / 2; // recompute m
        System.out.println(m);
        }

        // print out the estimate of the square root of c
        System.out.println(m);
    }

}

// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Liang Tao

// Edit this file.  Look for the specific "TODO" items below.  See
// Notes.txt for general instructions, and 2OPT.txt for a more
// specific discussion of 2-opt moves.  There is one more "TODO" item
// in GeomGraph.java, if you have time for it.

import java.util.Iterator;

public class TSP
{
    // These methods mimic the Makefile tests (to invoke from BlueJ).
    // The bigger ones are too slow until you edit GeomGraph.java
    public static void run10()   { run("tsp10.txt",    4); }
    public static void run100()  { run("tsp100.txt",  10); }
    public static void runbier() { run("bier127.txt", 20); }
    public static void run1000() { run("tsp1000.txt", 15); }
    public static void rncirc()  { run("circuit1290.txt", 20); }
    public static void runusa()  { run("usa13509.txt", 20); }
    public static void rungerm() { run("germany15112.txt", 20); }
    public static void runmona() { run("mona-20k.txt", 20); }

    // Main entry point: run(FILENAME, D) from command line arguments.
    public static void main(String[] args)
    {
        if (args.length == 0)
            run10(); // a default test
        else if (args.length == 2)
            run(args[0], Integer.parseInt(args[1]));
        else {
            System.err.println("Expected two arguments: FILENAME D");
            System.exit(1);
        }
    }

    // These are helper methods for doing the graphics.

    // Draw a single edge, in the current color.
    // Note e does not actually have to be an edge in G,
    // we just need G to convert vertex ids to points.
    static void draw(GeomGraph G, Edge e) {
        int v = e.either(), w = e.other(v);
        Point p = G.point(v), q = G.point(w);
        p.drawTo(q);
    }

    // Draw a list of edges (in the current color).
    static void draw(GeomGraph G, Iterable<Edge> edges) {
        for (Edge e: edges) draw(G, e);
    }

    // Draw all vertices and edges of G.
    static void draw(GeomGraph G) {
        int V = G.V();
        for (int u=0; u<V; ++u) {
            Point p = G.point(u);
            p.draw();
            draw(G, G.adj(u));
        }
    }

    // Ok, here is most of the work!
    // The first few steps (finding G and the MST) are done for you.
    public static void run(String fileName, int D) {
        
        // Step 1: read the file.
        In in = new In(fileName);
        // First line is bounding-box dimensions (width and height)
        int width = in.readInt(), height = in.readInt();
        // read the sequence of points (remainin input lines)
        Bag<Point> bp = new Bag<Point>();
        while (!in.isEmpty()) {
            double x = in.readDouble();
            double y = in.readDouble();
            bp.add(new Point(x, y));
        }
        // copy points into the points[] array, preserving
        // input order so points[0] is the first, and so on.
        int V = bp.size();
        Point[] points = new Point[V];
        {
            int v=V;
            for (Point p: bp) points[--v] = p;
        }

        // Step 2: construct and draw G.  For bigger input files,
        // constructing may be the slowest step!  Note we limit D
        // to at most V-1, since that is the maximum possible degree.
        GeomGraph G = new GeomGraph(points, Math.min(D, V-1));
        // Draw G to a StdDraw window, and save the image (as G.png).
        // We leave the StdDraw window at its default pixel size
        // (512 by 512) but rescale its axes to fit our image.
        int maxwh = Math.max(width, height);
        StdDraw.setXscale(0, maxwh);
        StdDraw.setYscale(0, maxwh);
        StdDraw.show(0);        // start deferred drawing mode
        draw(G);
        StdDraw.show(0);        // show the final image
        StdDraw.save("G.png");  // save it to a file

        // Step 3: compute MST, plot it in RED, save "MST.png",
        // and print the total weight of the MST on System.out.
        MST T = new MST(G);
        double wt = T.weight();
        Iterable<Edge> ET = T.edges(); // edge set of T
        StdDraw.clear();
        StdDraw.setPenColor(StdDraw.RED);
        draw(G, ET);
        StdDraw.show(0);
        StdDraw.save("MST.png");
        System.out.println(wt);     // first required line of output

        // Step 4: check that the MST is connected. If not, then G was
        // not connected, which could happen if D is too small.
        if (T.components() != 1) {
            System.err.println("G is not connected, giving up");
            System.exit(1);
        }

        // Step 5 (TODO):
        //
        // Compute a TSP (a cyclic tour of the points), by doing a DFS on the
        // MST, starting at 0.  The tour should visit the vertices in preorder,
        // that is the order they are first discovered by DFS (so 0 is first).
        // Since the tour may "shortcut" across non-tree edges from one vertex
        // to the next, the tour may use edges that are not part of G or T.
        // That is OK, since we are allowed to use any possible geometric edge
        // the edge weight is always just the point-to-point distance.
        // (Note the Point class has a method to compute distances.)
        // The tour should be cyclic, meaning that we count (and draw) an edge
        // from its last vertex back to its first.
        //
        // I recommend that you represent the tour as simply an int array, say
        // "int[] tour".  So tour[0] is the id of the first vertex (0) in the
        // tour, tour[1] is the next vertex id, on up to tour[V-1].
        //
        // You should print out two lines to System.out:

        EdgeWeightedGraph EdgeG = new EdgeWeightedGraph(V);
        for (Edge e : ET) EdgeG.addEdge(e);
        // new Tour object
        Tour t = new Tour(points);
        t.dfs(EdgeG, 0);
        //   first line: the sequence of vertex ids (array contents starts with 0),
        //  second line: the total weight of the tour (counting the back edge)
        t.print(); 


        // Step 6 (TODO):
        //
        //(as we did above), and set the plotting color
        // to StdDraw.GREEN.  Draw the edges of the tour, and save the resulting
        // image to "TSP1.png".
         StdDraw.clear(); // Clear the graphics image
         StdDraw.setPenColor(StdDraw.GREEN);
         for (int i = 0; i < V; i++) {
            int v = t.position(i);
            int vv = t.position((i + 1) % V);
            points[v].drawTo(points[vv]); // draw the edges
         }
         StdDraw.show(0);   // like the steps above
         StdDraw.save("TSP1.png"); // save the TSP1 to the required image
         
         
        // Step 7 (TODO):
        //
        // Improve your TSP by doing "2-opt" moves, until you cannot find a
        // 2-opt move that improves it at all (or, by more than a tiny amount,
        // like say 1e-6).  See 2OPT.txt for more details on this process.
        //
        // Note that to keep this from being too time-consuming, we only consider
        // 2-opt moves that result from adding an edge of G (so, we do not consider
        // all O(V^2) possible edges).   Since we do not consider *all* possible
        // edges, it is possible that one or two crossings may survive; that is
        // probably OK as long as they are rare.
        //
        // Finally print out the two lines again (tour sequence and weight)
        // like we did for the first tour.
         boolean done = false;
         while (!done) {
            done = true;
            for (Edge e : G.edges()) {
                int v = e.either();
                int vv = e.other(v);
                if(t.TwoOpt(v, vv)) done = false;
            }
         }
         t.print();
         
        // Step 8 (TODO):
        //
        // Clear the graphics image again, set the color to StdDraw.BLUE, and
        // draw the tour one more time.  Same the image as "TSP2.png".
         StdDraw.clear();
         StdDraw.setPenColor(StdDraw.BLUE);
         for(int i = 0; i < V; i++) {
            int v = t.position(i);
            int vv = t.position((i + 1) % V);
            points[v].drawTo(points[vv]);
         }
         StdDraw.show(0);
         StdDraw.save("TSP2.png");
         
        // Step 9 (mostly done):
        //
        // Exit, you have two choices here.
        //
        // If you use "System.exit(0)", then the graphic window closes immediately,
        // and the program ends without any input from the user (you).
        // Otherwise, the program waits and will not end until you close the window.
        // For turnin purposes, System.exit(0) is more convenient,
        // but for testing purposes it is nicer to see that final image.
        //
        // Your choice:
        // System.exit(0);
        System.err.println("Close the window to exit...");
    }
}
    
    // Implement a tour class for the above steps
    class Tour {
        private Point[] points;
        private boolean[] marked;  // an array of marked sign
        private int[] tour;    // order of the points
        private int[] tourInverse; //for the purpose of inverse
        private int length;      // length of the tour
        private int N = 0;
        
        public Tour (Point[] outpoints) { // constructor
            points = outpoints;
            length = outpoints.length;
            System.out.println(this.points);
            marked = new boolean[length]; // construct other arrays using length
            tour = new int[length];      
            tourInverse = new int[length];
        }
        
        // add vertex v to the end 
        void add (int v) {
            tour[N] = v;
            tourInverse[v] = N; 
            N++;
        }
        
        int position(int v) {
            return tour[v];
        }
        
        int after(int v) { // next vertex
            return tour[(tourInverse[v] + 1) % length];
        }
        
        // dfs search
        void dfs(EdgeWeightedGraph G, int v) {
            marked[v] = true;
            add(v);
            for(Edge e: G.adj(v)) {
                int w = e.other(v);
                if(!marked[w]) {
                    dfs(G, w); // recursively do the dfs
                }
            }
        }
        
        // Total Euclidean distance
        double eucliDistance() {
            double distance = 0.0;
            for (int i = 0; i < length; i++) {
                distance += points[tour[i]].distanceTo(points[tour[(i+1)%length]]);
            }
        
        return distance;
        }
        
        // function to reverse subarray
        void reverseST (int a, int b) {
           while (true) {
            if (a == b) break;
            // else
            int c = tour[a];
            tour[a] = tour[b];
            tour[b] = c;
            tourInverse[tour[a]] = a;
            tourInverse[tour[b]] = b;
            
            a = (a + 1) % length;
            
            if (a == b) break; // check again
            
            b = (b + length - 1) % length;
            } 
           
        }
        
        // 2-opt function 
        boolean TwoOpt (int v, int w) {
            int vnext = after(v);
            int wnext = after(w);
            
            if (wnext == v || vnext == w) {
                return false;
            }
            
        double v2w = points[v].distanceTo(points[w]);
        double vnext2wnext = points[vnext].distanceTo(points[wnext]);
        double v2vnext = points[v].distanceTo(points[vnext]);
        double w2wnext = points[w].distanceTo(points[wnext]);
        
        double dist2opt = (v2w + vnext2wnext) - (v2vnext + w2wnext);
        if (dist2opt >= -0.000001) {
            return false;
        }
        
        // reverse the subarray
        int ww = tourInverse[wnext];
        int vv = tourInverse[v];
        
        //reverseST
        reverseST(ww, vv);
        return true;
        }
        
        // do the print
        void print() {
            for (int i = 0; i < N; i++) {
                System.out.print(tour[i] + " ");
            }
            System.out.println("");
            System.out.println(eucliDistance());
        }
}
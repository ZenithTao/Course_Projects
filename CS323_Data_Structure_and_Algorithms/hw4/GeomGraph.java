// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Liang Tao
import java.util.*;

// TODO: make the constructor faster, by using a QuadTree to find the
// nearest neighbors of each point.   Basically you need to construct
// a QuadTree to model the array of Point objects, and then repeatedly
// use its "pairsNear(p)" method to find the points which are closest
// to a given point.
//
// Note the QuadTree is something like a symbol table.  The Point objects
// are its keys, and the QuadTree maps those keys to values.  For our use,
// I suggest the "value" associated with a Point should be an Integer: the
// corresponding vertex id of that Point.
//
// Finally, you can get an unbalanced QuadTree if you insert the items
// in a bad order.  One easy fix to this is to insert the points in a
// random order, then the expected depth is O(log N).


// A GeomGraph is an EdgeWeightedGraph, with vertices correspond to
// points in the plane (Point objects).  Furthermore, we add edges
// from each vertex to its D nearest neighbors, with distances as the
// edge weights.  (For large N we fix some D like 20, so the graph is
// sparse.)  Our constructor may create duplicate edges, since the
// same edge may be added twice, once from each end.  We don't mind
// that.

public class GeomGraph extends EdgeWeightedGraph
{
    // Our array of Point objects, given in the constructor.
    final private Point[] points;

    // Add edges from v to its D closest neighbors, in O(V) time.
    private void addEdges(int v, int D)
    {
        Point p = points[v];
        int V = points.length;
        // Construct ALL possible edges from v.
        Edge[] all = new Edge[V];
        for (int w=0; w<V; ++w)
            all[w] = new Edge(v, w, p.distanceTo(points[w]));
        // Sort the Edge objects by weight.
        java.util.Arrays.sort(all);
        // Add the first D edges (skipping the self-loop, all[0]).
        for (int i=1; i<=D; ++i)
            addEdge(all[i]);
    }

    // Given v, return the corresponding Point object, points[v].
    public Point point(int v) { return points[v]; }

    // Return degree of vertex v (length of its adjacency list)
    public int degree(int v) { return ((Bag<Edge>)adj(v)).size(); }

    // Constructor: we save the points array, and connect
    // each vertex to its D closest neighbors.  O(V^2) time.
    public GeomGraph(Point[] points, int D) {
        super(points.length);
        this.points = points;
        int V = V();
        
        //using a QuadTree to find the nearest neighbors of each point.
        QuadTree<Integer> quadtree = new QuadTree<Integer>();
        for (int v = 0; v < V; ++v) {
            quadtree.insert(points[v], v);
        }
        
        for (int v=0; v<V; ++v) {
            Point p = points[v];
            // use the valuesNear method in quatree
            Iterator<Integer> iterator = quadtree.valuesNear(p);
            iterator.next();
            for (int i = 0; i < D; i++) {
                int w = iterator.next();
                addEdge(new Edge(v, w, p.distanceTo(points[w])));
            }
        }
    }
}

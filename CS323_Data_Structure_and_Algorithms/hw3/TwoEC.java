// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Liang Tao

// Note you are welcome to reuse code from share/book/.
// Online, that is http://mathcs.emory.edu/~cs323000/book/

// Summary: TwoEC should compute the 2-edge-connected components of a
// given graph, using an API similar to CC (share/book/CC.java).
//
// Additional requirements:
//    * do not modify the given Graph object, G
//    * your solution should use O(V+E) time and O(V) space, like DFS
//    * for all v, id(v) should be at least 0 and less than G.V()
//
// Step 1:
//
//     Copy the code from share/book/Bridge.java
//     Rename the class and its constructor.
//     Modify it to be quiet (do not print out the bridge edges)
//     you may remove the "components()" and "main()" methods
//     Add private "id" and "size" arrays (like low and pre)

// Step 2:
//
//     Figure out a two-pass DFS strategy, and do both of those
//     passes in your constructor.
//
//     The first DFS pass should be like Bridge.java: you compute
//     pre[v] and low[v] for all v.  You can also compute size[v],
//     which is the size of the DFS-subtree rooted at v, after bridge
//     edges have been deleted.  (You do not actually have to delete
//     the bridge edges, you just don't count sizes over a bridge.)
//
//     The second DFS pass should visit vertices in exactly the same
//     order as before, but now we can detect bridge edges when we
//     first see them (look for low[w]==pre[w]).  Using that, we can
//     now compute id[v] and (the final version of) size[v], for each v.
//
public class TwoEC {
    private int[] low;
    private int[] pre;
    private int cnt;
    private int cnt_twopass;     // usde for second pass
    private int cnt_two_edge_com;   // number of the two-edge-components
    private int bridges;
    private int[] id;     // component identifier for vertex v
    private int[] size; // array including the size of 2EC component containing specific v
    private int[] pre_twopass; // used for second pass

    public TwoEC(Graph G) {
        low  = new int[G.V()];
        pre  = new int[G.V()];
        size = new int[G.V()];
        pre_twopass = new int[G.V()];
        id   = new int[G.V()];

        for (int v = 0; v < G.V(); v++) low[v] = -1;
        for (int v = 0; v < G.V(); v++) {
            pre[v] = -1;
            pre_twopass[v] = -1;
            id[v] = -1;
        }

        // first pass
        for (int v = 0; v < G.V(); v++)
            if (pre[v] == -1)
                dfs1(G, v, v);

        // second pass
        for (int v = 0; v < G.V(); v++) 
            if (pre_twopass[v] == -1)
                dfs2(G, v, v);

    }

    // first pass dfs, like the original bridge
    private void dfs1(Graph G, int u, int v) {
        pre[v] = cnt++;
        low[v] = pre[v];
        size[v] = 1;
        for (int w : G.adj(v)) {
            if (pre[w] == -1) {
                dfs1(G, v, w);
                low[v] = Math.min(low[v], low[w]);
                if (low[w] == pre[w]) {
                    //v-w is a bridge
                    bridges++;
                } else
                    size[v] += size[w]; //else, just update the size
            }

            // update low number - ignore reverse of edge leading to v
            else if (w != u)
                low[v] = Math.min(low[v], pre[w]);
        }
    }

    // second pass dfs
    private void dfs2(Graph G, int u, int v) {
        pre_twopass[v] = cnt_twopass++;

        if (low[v] == pre_twopass[v]) {     // detect bridges  
           id[v] = cnt_two_edge_com++;      // update the total # of 2ec
        } else {
            id[v] = id[u];          
            size[v] = size[u];      // size of 2ec component which contains v
        }

        for (int w : G.adj(v)) {
            if (pre_twopass[w] == -1)
                dfs2(G, v, w);
        }
    }

// Step 3:
//
// Add the public methods count(), id(v), and size(v), which
// resemble those in the CC.java API

// 2-edge-connected components in the graph
// and the formula: #(2EC components) = #(connected components) +  #(bridge edges)
    public int count() {     // -- return the number of 2EC components
        return cnt_two_edge_com;
    }

    public int id(int v) {  // -- identify the 2EC component containing v
        return id[v];
    }

    public int size(int v) { // -- size of 2EC component containing v
        return size[v];
    }

}



//
// Step 4:
//
//     Test your code! The Makefile defines several test inputs
//     (try just "make" to get a list of possibilities).
//     When you  are done or out of time, "make turnin"

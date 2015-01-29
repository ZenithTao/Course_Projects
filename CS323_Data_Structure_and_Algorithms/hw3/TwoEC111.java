// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Shimiao Huang

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
//
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
//     now compute id[v] and (the final version of) size[v], for each v
//
// Step 3:
//
//     Add the public methods count(), id(v), and size(v), which
//     resemble those in the CC.java API:
//        int count()     -- return the number of 2EC components
//        int id(int v)   -- identify the 2EC component containing v
//        int size(int v) -- size of 2EC component containing v
//
// Step 4:
//
//     Test your code! The Makefile defines several test inputs
//     (try just "make" to get a list of possibilities).
//     When you  are done or out of time, "make turnin"

public class TwoEC {
    private int[] low;
    private int[] pre;
    private int cnt;
    private int cnt2;
    private int bridges;
    
    private int ec_count;
       
    private int[] id;
    private int[] size;
    
    private int[] pre2;

    public TwoEC(Graph G) {
        low = new int[G.V()];
        pre = new int[G.V()];
        size = new int[G.V()];
        pre2 = new int[G.V()];
        id = new int[G.V()];
        
        
        for (int v = 0; v < G.V(); v++) low[v] = -1;
        for (int v = 0; v < G.V(); v++) {
        	pre[v] = -1;
        	pre2[v] = -1;
        	id[v] = -1;
        }

        for (int v = 0; v < G.V(); v++)
            if (pre[v] == -1)
                dfs1(G, v, v);
        
        for (int v = 0; v < G.V(); v++)
            if (pre2[v] == -1)
                dfs2(G, v, v);
        
    }
   
    // 1st DFS 
    private void dfs1(Graph G, int u, int v) {
        pre[v] = cnt++;
        low[v] = pre[v];
        size[v] = 1;
        for (int w : G.adj(v)) {
            if (pre[w] == -1) {           	
                dfs1(G, v, w);               
                low[v] = Math.min(low[v], low[w]);
                if (low[w] == pre[w]) {
                    //StdOut.println(v + "-" + w + " is a bridge");
                    bridges++;
                    
                }else {
                	size[v] += size[w] ;  // size of subtree.
                }               
            }
            // update low number - ignore reverse of edge leading to v
            else if (w != u)
                low[v] = Math.min(low[v], pre[w]);
        }
    }
  
    // 2nd DFS
    private void dfs2(Graph G, int u, int v) {
    	pre2[v] = cnt2++;
    	//size2[v] = 1;
    	  	
    	if (low[v] == pre2[v]){
    		id[v] = ec_count++;
    	  
    	}else {
    		id[v] = id[u];
    		size[v] = size[u];    // size of 2EC component containing v
    						      // if a b c are in same 2EC component, size of a=b=c=3
    		                      // This size[] is different from size[] in 1st DFS
    	}
    	
    	for(int w : G.adj(v)){
    		if(pre2[w] == -1){
    			dfs2(G, v, w);
    		}    		  		
    	}   
    	//System.out.println(id[v]);
    }
    

    
    public int count(){
    	return ec_count;
    }
    
    public int id(int v){
    	return id[v];
    }
    
    public int size(int v){
    	return size[v];
    }
   
}


// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Liang Tao
// TODO: this is almost complete, you just need to write sumDepths(),
// which will then be inherited by the other two implementations.

// Naive quick-union UF (no weights, no path-compression).
// This is mostly from the book.

public class QuickUnionUF implements UFI 
{
    int[] id;    // id[i] = parent of i
    int count;   // number of component
    int depth;
    
    // instantiate N isolated components 0 through N-1
    public QuickUnionUF(int N) {
        id = new int[N];
        count = N;
        for (int i = 0; i < N; i++)
            id[i] = i;
    }

    // return the number N of items (they are 0 to N-1)
    public int size() { return id.length; }
    
    // return number of connected components
    public int count() { return count; }

    // return root of component corresponding to element p
    public int find(int p) {
        while (p != id[p]){
            p = id[p];
            depth++;
        }
        return p;
    }

    // are elements p and q in the same component?
    public boolean connected(int p, int q) {
        return find(p) == find(q);
    }

    // merge components containing p and q
    public void union(int p, int q) {
        int i = find(p);
        int j = find(q);
        if (i == j) return;
        id[i] = j; 
        count--;
    }
    
    // sumDepths() should return the sum, over all items, of the
    // depth of that item.  Its depth is the number of steps
    // from the item to its root.  So initially, when all
    // the items are roots, sumDepths() should return 0.
    // This method should *not* modify anything, in particular
    // it should not do any path compression.
    public int sumDepths()
    {
        // this is only correct initially.
        int l = id.length;
        int sum = 0;
            for (int i = 0; i < l; i++) {
                if (i != id[i]) {
                    int p = i;
                    while (p != id[p]){
                        p = id[p];
                        sum++;
                    }
                }
            }
            
        return sum;
    }
}

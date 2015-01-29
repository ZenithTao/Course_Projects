// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Liang Tao
// Weighted quick-union UF (no path compression).
// From book, except we are extending QuickUnionUF.

// TODO: finish this.  You need to declare the sz array,
// write the constructor, and override the union method.
// You do not need to override the find method here.

public class WeightedQuickUnionUF extends QuickUnionUF
{ 
    private int[] sz;
   
    public WeightedQuickUnionUF(int N) {
        super(N);
        sz = new int[N];
        for (int i = 0; i < N; i++) sz[i] = 1;
    }
    
    @Override
    public void union(int p, int q) {
        int i = find(p);
        int j = find(q);
        if (i == j) return;
        
        // Make smaller root point to larger one.
        if (sz[i] < sz[j]) { id[i] = j; sz[j] += sz[i]; }
        else               { id[j] = i; sz[i] += sz[j]; }
        count--;
    }
}
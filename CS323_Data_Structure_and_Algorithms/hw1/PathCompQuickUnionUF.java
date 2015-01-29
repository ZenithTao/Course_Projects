// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Liang Tao
// Quick-union UF (path compression, but without weights).
// We extend QuickUnion.

// TODO: write this class, so it extends QuickUnionUF.
// You should only need to define the constructor and find(p).
// Your find method may do full path compression or
// half path compression.
// You should not override the union method here.

public class PathCompQuickUnionUF extends QuickUnionUF
{

    // instantiate N isolated components 0 through N-1
    public PathCompQuickUnionUF(int N) 
    {
        super(N);
    }
    
    // return root of component corresponding to element p
    public int find(int p) {
        while (p != id[p]) {
            id[p] = id[id[p]];
            p = id[p];
            depth++;
        }
        return p;
    }
}

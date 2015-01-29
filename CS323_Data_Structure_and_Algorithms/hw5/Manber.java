// For homework, copy whatever code you need from this file into
// your BWT.java file, and adapt it to your needs there.

// This program uses Manber's suffix-sorting algorithm, to sort all
// cyclic shifts of a string s in O(N lg N) time. For example:
//    java Manber 'yabbadabbadoo!'
//    java Manber 'abcabcabc'
//
// This code does not rely on String.substring (except for the demo
// output), so it should still be fast even in recent versions of Java
// where String.substring no longer shares storage.

// Original: http://algs4.cs.princeton.edu/63suffix/  (Manber.java)

// Local modifications:
//  * changed main(), removed dependence on In (share/book/In.java)
//  * added constant R (just to document the ASCII charset assumption)
//  * comments, whitespace, "show" output
import java.util.*;

public class Manber
{
    // We assume all char codes are in extended ASCII range (0 to 255)
    static int R = 256;

    private int N;               // length of input string
    private String text;         // input text
    private int[] index;         // offset of ith string in order
    private int[] rank;          // rank of ith string
    private int[] newrank;       // rank of ith string (temporary)
    private int offset;

    public Manber(String s)
    {
        N    = s.length();
        text = s;
        index   = new int[N+1];
        rank    = new int[N+1];
        newrank = new int[N+1];

        // sentinels

        index[N] = N;
        rank[N] = -1;

        msd();
        doit();
    }

    // do one pass of msd sorting by rank at given offset
    private void doit()
    {
        for (offset = 1; offset < N; offset += offset)
        {
            // System.out.println("offset = " + offset);
            int count = 0;
            for (int i = 1; i <= N; i++) {
                if (rank[index[i]] == rank[index[i-1]]) count++;
                else if (count > 0) {
                    // sort
                    int left = i-1-count;
                    int right = i-1;
                    quicksort(left, right);

                    // now fix up ranks
                    int r = rank[index[left]];
                    for (int j = left + 1; j <= right; j++) {
                        if (less(index[j-1], index[j]))  {
                            r = rank[index[left]] + j - left;
                        }
                        newrank[index[j]] = r;
                    }

                    // copy back - note can't update rank too eagerly
                    for (int j = left + 1; j <= right; j++) {
                        rank[index[j]] = newrank[index[j]];
                    }

                    count = 0;
                }
            }
        }
    }

    // sort by leading char
    private void msd()
    {
        // calculate frequencies
        int[] freq = new int[R];
        for (int i = 0; i < N; i++)
            freq[text.charAt(i)]++;

        // calculate cumulative frequencies
        int[] cumm = new int[R];
        for (int i = 1; i < R; i++)
            cumm[i] = cumm[i-1] + freq[i-1];

        // compute ranks
        for (int i = 0; i < N; i++)
            rank[i] = cumm[text.charAt(i)];

        // sort by first char
        for (int i = 0; i < N; i++)
            index[cumm[text.charAt(i)]++] = i;
    }

    // for debugging only
    public void show()
    {
        String texttext = text + text;  // make cyclic
        System.out.println("substring(index[j]...) j rank[index[j]] index[j]");
        for (int i = 0; i < N; i++)
        {
            String s = texttext.substring(index[i],
                                          index[i] + Math.min(40, N));
            System.out.println(s + " " + i + " " + rank[index[i]] +
                               " " + index[i]);
        }
        System.out.println();
        StringBuilder x = new StringBuilder();
        for(int i = 0; i < text.length(); i ++) {
            x.append(texttext.charAt(index[i] + text.length() - 1));
        }
        System.out.println(Arrays.toString(index));
        System.out.println("the result is:   " + x.toString());
    }


    // test client
    public static void main(String[] args)
    {
        // In in = new In();
        // String s = in.readAll();
        String s = "banana%";
        if (args.length > 0) s = args[0];
        Manber m = new Manber(s);
        m.show();
    }

    /**********************************************************************
     *  Helper functions for comparing suffixes.
     **********************************************************************/

    /**********************************************************************
     * Is the substring text[v+offset .. N] lexicographically less than the
     * substring text[w+offset .. N] ?
     **********************************************************************/
    private boolean less(int v, int w)
    {
        assert rank[v] == rank[w];
        if (v + offset >= N) v -= N;
        if (w + offset >= N) w -= N;
        return rank[v + offset] < rank[w + offset];
    }

    /*************************************************************************
     *  Quicksort code from Sedgewick 7.1, 7.2.
     *************************************************************************/

    // swap pointer sort indices
    private void exch(int i, int j)
    {
        int swap = index[i];
        index[i] = index[j];
        index[j] = swap;
    }

    // SUGGEST REPLACING WITH 3-WAY QUICKSORT SINCE ELEMENTS ARE
    // RANKS AND THERE MAY BE DUPLICATES
    void quicksort(int l, int r)
    {
        if (r <= l) return;
        int i = partition(l, r);
        quicksort(l, i-1);
        quicksort(i+1, r);
    }

    int partition(int l, int r)
    {
        int i = l-1, j = r;
        int v = index[r];       // could use random pivot here
        while (true)
        {
            // find item on left to swap
            while (less(index[++i], v))
                ;
            // find item on right to swap
            while (less(v, index[--j]))
                if (j == l) break;
            // check if pointers cross
            if (i >= j) break;
            exch(i, j);
        }
        // swap with partition element
        exch(i, r);
        return i;
    }
}

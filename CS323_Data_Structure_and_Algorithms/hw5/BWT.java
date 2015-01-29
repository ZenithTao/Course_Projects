// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Liang Tao

// See Notes.txt for the TODO items.  Briefly:
//   1. implement transformBack(), the reverse transform
//   2. re-implement transform(), using the Manber algorithm
//
// You are welcome to copy code from hw5/demo/.  You may assume all
// input chars are in the extended ASCII range, 0 to 255 (R=256).

public class BWT
{
    // The forward Burrows-Wheeler Transform.
    // The mark char should not appear in the input.
    static String transform(String input, char mark)
    {
        assert input.indexOf(mark)<0;
        int len = input.length();
        String s = input + mark;
        // a new Manber class to use the Manber method
        Manber m = new Manber(s);
        String output = m.getLastChar(); // get the string output
        
        return output;
    }

    // The reverse Burrows-Wheeler Transform.
    // The mark char should appear in the string.
    static String transformBack(String bw, char mark)
    {
        int at = bw.indexOf(mark), size = bw.length();
        // The mark should appear once, but not twice.
        assert at >= 0;
        assert bw.indexOf(mark, at+1) < 0;
        // TODO: finish this! See Notes.txt for method sketch.
        //throw new UnsupportedOperationException
        //  ("transformBack is not implemented");
        // Use a StringBuilder or char[], not a String, to build up
        // the answer.  Otherwise, might take quadratic time!
        char[] L = bw.toCharArray();
        // calculate the charArray F
        char[] F = new char[L.length];
        int[] T = new int[L.length];
        
        // use the method charSort Method to get T
        CharSort.sort(L, F, T);
        // use a stringbuilder to contruct the output
        StringBuilder sbstr = new StringBuilder();
        // then recover the original string
        // start from the mark, use F array to rebuild the original string
        int start = at;
        // a whileoop to iterate 
        while(true) {
            sbstr.append(F[T[at]]);
            at = T[at];
            if(at == start) break;
        }
        
        String output = sbstr.toString();
        output = new StringBuffer(output).reverse().toString();
        //delete the mark, get the original output
        return output.substring(0, output.length() - 1);
    }
}

// add a charsort method
class CharSort
{
    // We assume all char codes are in extended ASCII range (0 to 255)
    static int R = 256;

    // Sort from input to output
    static void sort(char[] input, char[] output, int[] T)
    {
        int N = input.length;
        assert output.length >= N; // long enough to receive answer

        // compute frequency counts
        int[] count = new int[R+1];
        for (int i = 0; i < N; ++i)
            ++count[input[i] + 1];

        // Now for each char code c, count[c+1] equals the number
        // of chars in the input with code equal to c.

        // compute cumulative counts
        for (int r = 0; r < R; ++r)
            count[r+1] += count[r];

        // Now for each char code c, count[c] equals the number
        // of chars in the input with code strictly less than c.
        assert count[0] == 0;
        assert count[R] == N;

        // Finally, move each char from the input to the output.
        // For BWT, this is where you finally figure out T.
        for (int i = 0; i < N; ++i) {
            output[count[input[i]]++] = input[i];
            T[i] = count[input[i]] - 1;
        }
    }
}

class Manber
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
    public String getLastChar()
    {
        String texttext = text + text;  // make cyclic
        // a stringbuilder to get the last character of every strings
        StringBuilder x = new StringBuilder();
        for(int i = 0; i < text.length(); i ++) {
            x.append(texttext.charAt(index[i] + text.length() - 1));
        }
        // return the result string
        return x.toString();
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

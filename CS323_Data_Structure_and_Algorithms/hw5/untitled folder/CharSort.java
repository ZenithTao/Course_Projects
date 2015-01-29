// For homework, copy whatever code you need from this file into
// your BWT.java file, and adapt it to your needs there.

// This program stably sorts an array of chars using key-indexed
// counting sort.  Pulled from inner loop of share/book/LSD.java

public class CharSort
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
            T[i] = count[input[i]]- 1;
        }
    }

    // Simple command-line demonstration.
    public static void main(String[] args)
    {
        char[] L = {'a', 'n', 'n', 'b', '%', 'a', 'a', 'a'};
        if (args.length > 0)
            L = args[0].toCharArray();
        char[] F = new char[L.length];
        int [] T = new int[L.length];
        System.out.print("input:  ");
        System.out.println(L);
        sort(L, F, T);
        System.out.print("output: ");
        System.out.println(F);
        
        System.out.print("T array: \n" );
        for(int i = 0; i < T.length; i++)
            System.out.printf("T[%d] = %d\n", i, T[i]);
    }
}

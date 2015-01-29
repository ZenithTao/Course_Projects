// Test Driver for the BWT class.

// We expect this to be invoked from the command line with three
// arguments:
//
//   java -ea Driver INFILE Z OUTFILE

// Here Z should be a single character, and the other two arguments
// are filenames (text files).  First, we read all the text from
// INFILE.  If that text does not contain the "marker" character (Z),
// then apply the forward BWT, and write the resulting text to
// OUTFILE.  If the text does contain the marker character, then apply
// the reverse BWT, and write the resulting text to OUTFILE.  In
// either case, we refuse to overwrite an existing OUTFILE.

import java.io.FileReader;
import java.io.FileWriter;
import java.io.File;

public class Driver
{
    static void die(String msg)
    {
        System.err.printf("error: %s\n", msg);
        System.exit(1);
    }

    public static void main(String[] args)
    {
        if (args.length != 3)
            die("expected three arguments");
        String inFileName = args[0];
        String markStr = args[1];
        String outFileName = args[2];
        if (markStr.length()!=1)
            die("mark arg should be one char");
        char mark = markStr.charAt(0);

        // Read the input string.
        String input = null;
        try {
            System.out.printf("Reading file %s\n", inFileName);
            StringBuilder sb = new StringBuilder();
            FileReader rd = new FileReader(inFileName);
            char[] buf = new char[256];
            while (true) {
                int got = rd.read(buf);
                if (got < 0) break;
                sb.append(buf, 0, got);
            }
            rd.close();
            input = sb.toString();
        } catch(Exception e) {
            die("while reading: " + e);
        }

        String result;
        // Check whether the input contains the marker char.
        if (input.indexOf(mark) < 0) {
            System.out.printf("Transforming %d chars\n", input.length());
            result = BWT.transform(input, mark);
        } else {
            System.out.printf("UnTransforming %d chars\n", input.length());
            result = BWT.transformBack(input, mark);
        }

        // Now write the result to outFileName.
        try {
            File outFile = new File(outFileName);
            if (outFile.exists())
                die("will not overwrite existing file " + outFileName);
            System.out.printf("Writing result to %s\n", outFileName);
            FileWriter wr = new FileWriter(outFile);
            wr.write(result);
            wr.close();
        } catch(Exception e) {
            die("while writing: " + e);
        }
        System.out.println();   // done
    }
}

/*************************************************************************
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHERS. LIANG TAO
DATE SUBMIITED: 10/07/2013
 *  Compilation: javac -cp :./stdlib.jar Playnew.java
 *  Execution:   java -cp :./stdlib.jar Playnew < elise.txt
 *  Dependencies: stdlib.jar
*************************************************************************/ 

public class Playnew {

    public static void main(String[] args) {

         StdDraw.setCanvasSize(1000, 500);
        // repeat as long as there are more integers to read in
        while (!StdIn.isEmpty()) {

            // read in the pitch, where 0 = Concert A (A4)
            int pitch = StdIn.readInt();

            // read in duration in seconds
            double duration = StdIn.readDouble();

            // build sine wave with desired frequency
            double hz = 440 * Math.pow(2, pitch / 12.0);
            int N = (int) (StdAudio.SAMPLE_RATE * duration);
            double[] a = new double[N+1];
            // draw the scale
            StdDraw.setXscale(0, 2 * Math.PI * 600 * hz / StdAudio.SAMPLE_RATE);
            StdDraw.setYscale(-3, + 3);
            for (int i = 0; i <= N; i++){
                a[i] = Math.sin(2 * Math.PI * (i) * hz / StdAudio.SAMPLE_RATE);
                if(i > 1 && i < 500) StdDraw.line(2 * Math.PI * (i - 1) * hz / StdAudio.SAMPLE_RATE, a[i - 1],
                                            2 * Math.PI * (i) * hz / StdAudio.SAMPLE_RATE, a[i]);

            }

            // play it using standard audio
            
            StdAudio.play(a);
            StdDraw.show(30); 
            StdDraw.clear();
        }   
    }
}

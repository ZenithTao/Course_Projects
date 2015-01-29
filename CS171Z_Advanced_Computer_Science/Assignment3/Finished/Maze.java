/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *  DATE SUBMIITED: 10/07/2013
 *  Compilation:  javac -cp :./stdlib.jar Maze.java 
 *  Execution:    java  -cp :./stdlib.jar Maze
 *************************************************************************/

import java.awt.*;
import java.util.*;

public class Maze{
	public static void main(String[] args) {
		StdDraw.line(0, 0, 1, 0);
		StdDraw.line(0, 0, 0, 0.8);
		StdDraw.line(1, 0, 1, 0.8);
		StdDraw.line(0, 0.8, 1, 0.8);

		Chamber chama = new Chamber();
		chama.draw(0, 0, 100, 80);
	}
}

class Chamber{
	public void draw(int x0, int x1, int y0, int y1){

		// generate the coordinate of division point
		int x = StdRandom.uniform(x0 + 1, y0);
		int y = StdRandom.uniform(x1 + 1, y1);

		System.out.println(x + " " + y + " " + x0 + " " + x1 + " " + y0 + " " + y1);

		// draw the walls
		StdDraw.line(x0 * 0.01, y * 0.01, y0 * 0.01, y * 0.01);
		StdDraw.line(x * 0.01, x1 * 0.01, x * 0.01, y1 * 0.01);
		// randomly pick one chamber not to open a gap
		int[] randarray = new int[4];
		int ungap = StdRandom.uniform(0, 4);
		for(int i = 0; i <=3 ; i++){
			if (i == ungap) randarray[i] = 1;
		}

		// open gap and do the recursion
		if (y0 - x > 1 && randarray[0] == 0) {
			int i = StdRandom.uniform(x, y0);
			StdDraw.setPenColor(Color.WHITE);
			StdDraw.line(i * 0.01, y * 0.01, (i + 1) * 0.01, y * 0.01);
			StdDraw.setPenColor(Color.BLACK);
		}

		if (x - x0 > 1 && randarray[1] == 0){
			int i = StdRandom.uniform(x0, x);
			StdDraw.setPenColor(Color.WHITE);
			StdDraw.line(i * 0.01, y * 0.01, (i + 1) * 0.01, y * 0.01);
			StdDraw.setPenColor(Color.BLACK);
		}

		if (y1 - y > 1 && randarray[2] == 0) {
			int i = StdRandom.uniform(y, y1);
			StdDraw.setPenColor(Color.WHITE);
			StdDraw.line(x * 0.01, i * 0.01, x * 0.01, (i + 1) * 0.01);
			StdDraw.setPenColor(Color.BLACK);
		} 

		if (y - x1 > 1 && randarray[3] == 0) {
			int i = StdRandom.uniform(x1, y);
			StdDraw.setPenColor(Color.WHITE);
			StdDraw.line(x * 0.01, i * 0.01, x * 0.01, (i + 1) * 0.01);
			StdDraw.setPenColor(Color.BLACK);
		}

		// recursion
		if (y0 - x > 1 && y - x1 > 1) draw(x, x1, y0, y);
		if (y0 - x > 1 && y1 - y > 1) draw(x, y, y0, y1);
		if (x - x0 > 1 && y1 - y > 1) draw(x0, y, x, y1);
		if (x - x0 > 1 && y - x1 > 1) draw(x0, x1, x, y);

    }
}




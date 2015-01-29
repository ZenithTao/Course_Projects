/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *
 *  DATE SUBMIITED: 10/07/2013
 *  Compilation:  javac -cp :./stdlib.jar Time.java 
 *  Execution:    java  -cp :./stdlib.jar Time
 *  Dependencies: StdDraw.java
 *
 *************************************************************************/

import java.awt.Color;
import java.util.*;  // for HashMap

// A figure is any type of drawing object.
class Figure {

    protected double theX;   // the x-coordinate
    protected double theY;   // the y-coordinate

    private Color theColor; // the color of the figure
    protected Draw theCanvas;  // the canvas on which the figure is 
                               // to be drawn

    public Figure(Draw canvas) {

	theX = 0;
	theY = 0;

	theCanvas = canvas;

	theCanvas.setPenColor(theCanvas.BLACK);
    }

    public double getX() { return theX; }
    public double getY() { return theY; }

    public void setLocation(double x, double y) {
	// set the x,y coordinates of the figure
	theX = x;
	theY = y;
    }

    public void setColor(Color color) {
	theColor = color;
    }

    public Color getColor() {
	return theColor;
    }

    public void draw() {
	// set the pen color to the current color specified
	// in theColor, but leave the actual drawing to the
	// subclass object
	theCanvas.setPenColor(theColor);
    }

    public void show(int delay) {
	theCanvas.show(delay);
    }
}


// A circle is a particular type of Figure
class Circle extends Figure {

    // its unique features:
    private double theRadius;  // the radius
    private boolean filled;    // filled or not
    
    public Circle(Draw canvas) {
	super(canvas);  // make sure to create the *whole* object
	theRadius = 1;  // default radius is 1
	filled = false; // default is not filled
    }

    public void setRadius(double radius) {
	theRadius = radius;
    }

    public double getArea() {
	return Math.pow(theRadius,2) * Math.PI;
    }

    public void setFilled() {
	filled = true;
    }

    public void unSetFilled() {
	filled = false;
    }

    public void move(double x, double y) {
	// intended for making small moves
	Color savedColor = getColor();
	
	this.setRadius(theRadius + .01);
	this.setColor(Color.WHITE);
	this.draw();
	
	this.setLocation(x,y);
	this.setRadius(theRadius - .01);
	this.setColor(savedColor);
	this.draw();
    }

    public void moveTo(double x, double y, int steps) {
	// smoothly move to x and y in the specified number of small steps
	//
	// this is for illustration, but may not be the most useful design

	double distX = Math.abs(x - theX);
	double distY = Math.abs(y - theY);

	double dx = // the x distance to travel in each step
	    (x > theX ? distX / steps: -distX / steps);
	double dy = // the y distance to travel in each step
	    (y > theY ? distY / steps: -distY / steps);

	for (int i = 0; i < steps; i++) {
	    theCanvas.show(20);

	    Color savedColor = getColor();

	    this.setRadius(theRadius + .01);
	    this.setColor(Color.WHITE);
	    this.draw();

	    this.setLocation(theX + dx,theY + dy);
	    this.setRadius(theRadius - .01);
	    this.setColor(savedColor);
	    this.draw();
	}
    }


    public void draw() {
	super.draw();  // correctly set the pen color

	if (filled)
	    theCanvas.filledCircle(theX, theY, theRadius);
	else
	    theCanvas.circle(theX, theY, theRadius);
    }
}

// A line is also a paticular type of Figure
class Line extends Figure {
	public Line(Draw canvas) {
		super(canvas);
	}

	// main circle method
	public void startCircle(int i) {
		 this.setColor(Color.BLUE);
		 this.draw(0.85 * Math.sin(Math.toRadians(i * 6)), 
						0.85 * Math.cos(Math.toRadians(i * 6)));
		 this.setColor(Color.RED);
		 this.draw(0.85 * Math.sin(Math.toRadians((i - 1) * 6)), 
						0.85 * Math.cos(Math.toRadians((i - 1) * 6)));
		 this.show(1000);
    }

	public void draw(double x, double y) {
		super.draw();  // correctly set the pen color
		theCanvas.line(theX, theY, x, y );
    }
}


// A Text object extends Figure
class Text extends Figure{

	public Text(Draw canvas) {
		super(canvas);
	}

	public void drawText(double x, double y, String s){
		super.draw();
		theCanvas.text(x, y, s);
	}
}

// A clock object implements text, line and circle

class Clock {
	private static Draw canvas = new Draw();
	private static Line lineabc = new Line(canvas);
	private static Text textabc = new Text(canvas);
	
	public Clock(){
		this(canvas);
	}

	public Clock(Draw canvas){
		//draw the canvas
		this.canvas = canvas;
		canvas.setXscale(-1.5, 1.5);
		canvas.setYscale(-1.5, 1.5);
		
		//draw the clock circle
		Circle circleA = new Circle(canvas);
		circleA.setRadius(1);
		circleA.setColor(Color.BLACK);
		circleA.draw();

		//draw the scale on the clock
		for(int i = 1; i <= 12; i++){
			textabc.drawText(0.92 * Math.sin(Math.toRadians(i * 30)), 
						0.92 * Math.cos(Math.toRadians(i * 30)), Integer.toString(i));
		}
	}

	//The tick method to move the pointer
	public void tick(){
		for(int i = 1; i <=60; i++){
			lineabc.startCircle(i);
		}
	}
}


// this is a client, to test and demonstrate the figure/circle classes
public class Time {   
    public static void main(String[] args) {

    	Clock myClock = new Clock();

    	while(true) {
    		myClock.tick();
    	}
    }
}



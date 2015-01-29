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
	private double x;
	private double y;
	public Line(Draw canvas) {
		super(canvas);
	}

	public void Draw(){
		super.draw();
		theCanvas.line(theX, theY, x, y);
	}

}


// A Text object extends Figure
class Figure extends Figure{

	public Figure(Draw canvas) {
		super(canvas);
	}
}


// this is a client, to test and demonstrate the figure/circle classes

class OODemo {   

    public static void main(String[] args) {

	Draw canvasA = new Draw();
	Draw canvasB = new Draw();

	canvasA.setXscale(-1.5, 1.5);
	canvasA.setYscale(-1.5, 1.5);

	canvasB.setXscale(-1.5, 1.5);
	canvasB.setYscale(-1.5, 1.5);

	Circle circleA = new Circle(canvasA);
	Circle circleB = new Circle(canvasB);
	Circle circleC = new Circle(canvasA);  // same canvas as circleA

	circleA.setRadius(1);
	circleA.setFilled();
	circleA.setColor(Color.BLACK);
	circleA.draw();

	circleC.setRadius(.5);
	circleC.setFilled();
	circleC.setColor(Color.YELLOW);
	circleC.draw();

	circleB.setRadius(.2);
	circleB.setFilled();
	circleB.setColor(Color.RED);
	circleB.draw();

	// this is just a loop to allow me time to separate the canvases
	while (true) {
	    if (canvasB.mousePressed())
		break;
	}

	// we can move by using the class supplied moveTo()
	circleB.moveTo(-1,1,50);

	// this is just a loop to allow me time to talk
	while (true) {
	    if (canvasB.mousePressed())
		break;
	}

	// or we can move by calculating the new positions for each step
	int steps = 40;
	for (int i = 0; i < steps; i++) {
	    canvasB.show(steps);
	    circleB.move(circleB.getX() + 1.0 / steps,circleB.getY());
	}

	// more time to talk
	while (true) {
	    if (canvasA.mousePressed())
		break;
	}

	// this shows that animation isn't easy -- need to maintain
	// a representation of all the objects on the canvas and
	// repaint frequently

	double mX = canvasA.mouseX();  // get the x coordinate of the mouse

	if (mX > 0) { 
	    // if we just move the circle, we leave a trail of unpainted space
	    circleC.moveTo(-1,1,20);
	}
	else {
	    // need to repaint the objects underneath
	    steps = 100;
	    for (int i = 0; i < steps; i++) {
		canvasA.show(20);
		circleA.draw();
		circleC.move(circleC.getX() - 1.0/steps,
			     circleC.getY() + 1.0/steps);
	    }
	    circleA.draw();
	    circleC.draw();

	    canvasA.show(20);
	}
    }
}


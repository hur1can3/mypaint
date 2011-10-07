//======================================================== file = mypaint.c  ===
//= Program that lets the user draw basic lines and shapes                     =
//==============================================================================
//=  Author: Matthew Levandowski                                               =
//=          Email: levandma@gvsu.edu                                          =
//=          Grand Valley State University                                     =
//=          CIS 367 Computer Graphics                                         =
//==============================================================================
//----- Include files ----------------------------------------------------------
#include "mypaint.h"
#include <iostream>

//----- Constant defines -------------------------------------------------------
using namespace std;
const char *windowName = "CIS367 mypaint: Matthew Levandowski";

//------------------------------------------------------------------------------
//-  Function that draws a rectangle with  lines                               -
//------------------------------------------------------------------------------
void mypaint::drawRectangle(int startx, int starty, int endx, int endy) {
	int length = abs(endx - startx);
	int width = abs(endy - starty);
	drawLine(startx, starty, startx + length, starty);
	drawLine(startx + length, starty, startx + length, starty - width);
	drawLine(startx + length, starty - width, startx, starty - width);
	drawLine(startx, starty - width, startx, starty);
	glFlush();
	cerr << "rectangle drawn." << endl;
}

//------------------------------------------------------------------------------
//-  Function that compares two RGBColor types                                 -
//------------------------------------------------------------------------------
bool color_comp(RGBColor color1, RGBColor color2) {
	if (color1.r == color2.r && color1.b == color2.b && color1.g == color2.g) {
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
//-  Function that fills a closed shape with floodfill algorithm               -
//------------------------------------------------------------------------------
void mypaint::flood_fill(int x, int y, RGBColor fillc) {
	RGBColor cur = getPixel(x, y);
	if (color_comp(cur, WHITE)) {
		setPixel(x, y, fillc);
		flood_fill(x - 1, y, fillc);
		flood_fill(x + 1, y, fillc);
		flood_fill(x, y - 1, fillc);
		flood_fill(x, y + 1, fillc);
	}
}

//------------------------------------------------------------------------------
//-  Function that draws a line using the Bresenham line function              -
//------------------------------------------------------------------------------
void mypaint::drawLine(int startx, int starty, int endx, int endy) {
	int dx = abs(endx - startx), sx = startx < endx ? 1 : -1;
	int dy = -abs(endy - starty), sy = starty < endy ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	for (;;) /* loop */
	{
		setPixel(startx, starty, currentDrawColor);
		if (startx == endx && starty == endy)
			break;
		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			startx += sx;
		}
		if (e2 <= dx) {
			err += dx;
			starty += sy;
		}
	}
	glFlush();
	cerr << "line drawn." << endl;
}

//------------------------------------------------------------------------------
//-  Function that draws a circle using the Bresenham circle function          -
//------------------------------------------------------------------------------
void mypaint::drawCircle(int x0, int y0, int radius) {

	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	setPixel(x0, y0 + radius, currentDrawColor);
	setPixel(x0, y0 - radius, currentDrawColor);
	setPixel(x0 + radius, y0, currentDrawColor);
	setPixel(x0 - radius, y0, currentDrawColor);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		setPixel(x0 + x, y0 + y, currentDrawColor);
		setPixel(x0 - x, y0 + y, currentDrawColor);
		setPixel(x0 + x, y0 - y, currentDrawColor);
		setPixel(x0 - x, y0 - y, currentDrawColor);
		setPixel(x0 + y, y0 + x, currentDrawColor);
		setPixel(x0 - y, y0 + x, currentDrawColor);
		setPixel(x0 + y, y0 - x, currentDrawColor);
		setPixel(x0 - y, y0 - x, currentDrawColor);
	}
	glFlush();
	cerr << "circle drawn." << endl;
}

//------------------------------------------------------------------------------
//-  Function that controls what is drawn based upon mouse actions             -
//------------------------------------------------------------------------------
void mypaint::mouseClick(int x, int y) {
	// What we do depends on what mode we're in
	switch (nMode) {
	case MODE_POINTS:
		setPixel(x, y, currentDrawColor);
		cerr << "pixel drawn." << endl;
		break;

	case MODE_LINES:
		// If we haven't gotten our start point yet, this
		// is our start point.
		if (!nGotStart) {
			nStartX = x;
			nStartY = y;
			nGotStart = 1;
		}
		// We've already gotten our start point, so go ahead and draw
		// a line.
		else {
			drawLine(nStartX, nStartY, x, y);
			nGotStart = 0;
		}
		break;

	case MODE_CIRCLES:
		// If we haven't gotten our start point yet, this
		// is our start point.
		if (!nGotStart) {
			nStartX = x;
			nStartY = y;
			nGotStart = 1;
		} else {
			// compute a radius based on the center point and the current point
			int radius = (int) sqrt(
					(double) ((x - nStartX) * (x - nStartX) + (y - nStartY)
							* (y - nStartY)));
			drawCircle(nStartX, nStartY, radius);
			nGotStart = 0;
		}

		break;

	case MODE_RECTANGLES:
		// If we haven't gotten our start point yet, this
		// is our start point.
		if (!nGotStart) {
			nStartX = x;
			nStartY = y;
			nGotStart = 1;
		}

		// We've already gotten our start point, so go ahead and draw
		// a line.
		else {
			drawRectangle(nStartX, nStartY, x, y);
			nGotStart = 0;
		}
		break;

	case MODE_FLOODFILL:
		flood_fill(x, y, currentDrawColor);
		cerr << "Floodfill done." << endl;
		break;

	};
}

//------------------------------------------------------------------------------
//-  Pixel drawing fuction                                                     -
//------------------------------------------------------------------------------
void mypaint::setPixel(const int& x, const int& y, const RGBColor& color) {
	// Draw pixel
	glColor3f(color.r, color.g, color.b);
	glBegin( GL_POINTS);
	glVertex2i(x * nPixelSize + nPixelSize / 2, y * nPixelSize + nPixelSize / 2);
	glEnd();
	glFlush();
}

//------------------------------------------------------------------------------
//-  Pixel reading fuction that gets the RGBColor of a pixel                   -
//------------------------------------------------------------------------------
RGBColor mypaint::getPixel(const int& x, const int& y) {
	RGBColor toReturn;
	glReadPixels(x * nPixelSize + nPixelSize / 2,
			y * nPixelSize + nPixelSize / 2, 1, 1, GL_RGB, GL_FLOAT,
			((float*) (&toReturn)));
	return toReturn;
}

//------------------------------------------------------------------------------
//-  Initialize our program                                                    -
//------------------------------------------------------------------------------
void mypaint::initialize() {
	// White background
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(currentDrawColor.r, currentDrawColor.g, currentDrawColor.b);
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) screenWidth, 0.0, (GLdouble) screenHeight);
	// setup size of points / lines with pixel size
	glPointSize((GLfloat) nPixelSize);
	//set up anti-aliasing
	glEnable( GL_POLYGON_SMOOTH);
	glEnable( GL_LINE_SMOOTH);
	glEnable( GL_POINT_SMOOTH);
	int buf, sbuf;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &buf);
	glGetIntegerv(GL_SAMPLES, &sbuf);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable( GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

//------------------------------------------------------------------------------
//-  Function that redraws the window                                          -
//------------------------------------------------------------------------------
void mypaint::redraw(void) {
	glClear( GL_COLOR_BUFFER_BIT);
	glFinish();
	nGotStart = 0;
}

//------------------------------------------------------------------------------
//-  Handles progrom on window resize                                          -
//------------------------------------------------------------------------------
void mypaint::reshape(int w, int h) {
	cerr << "Screen resized to " << w << " " << h << endl;
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, screenWidth, screenHeight);
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) screenWidth, 0.0, (GLdouble) screenHeight);
	glClear( GL_COLOR_BUFFER_BIT);
	glFinish();
	nGotStart = 0;
	numPixelsX = screenWidth / nPixelSize;
	numPixelsY = screenHeight / nPixelSize;
}

//------------------------------------------------------------------------------
//-  Function that handles keyboard presses                                    -
//------------------------------------------------------------------------------
void mypaint::keyDown(unsigned char key, int x, int y) {
	if (key >= '0' && key <= '9') {
		int color = key - '0';
		currentDrawColor = colors[color];
		cerr << "Color changed." << endl;
		return;
	}
	key = tolower(key);
	switch (key) {
	case 'p':
		cerr << "Point mode\n";
		nMode = MODE_POINTS;
		break;
	case 'l':
		cerr << "Line mode\n";
		nMode = MODE_LINES;
		nGotStart = 0;
		break;
	case 'c':
		cerr << "Circle mode\n";
		nMode = MODE_CIRCLES;
		nGotStart = 0;
		break;
	case 'r':
		cerr << "Rectangle mode\n";
		nMode = MODE_RECTANGLES;
		nGotStart = 0;
		break;
	case 'f':
		cerr << "Floodfill mode\n";
		nMode = MODE_FLOODFILL;
		nGotStart = 0;
		break;
	case 'b':
		if (nPixelSize == SMALL_PIX_SIZE)
			nPixelSize = BIG_PIX_SIZE;
		else
			nPixelSize = SMALL_PIX_SIZE;
		glPointSize((GLfloat) nPixelSize);
		glClear( GL_COLOR_BUFFER_BIT);
		glFinish();
		nGotStart = 0;
		numPixelsX = screenWidth / nPixelSize;
		numPixelsY = screenHeight / nPixelSize;
		cerr << "Pixel size now: " << nPixelSize << endl;
		break;
	case 'x':
		glClear(GL_COLOR_BUFFER_BIT);
		cerr << "Screen cleared." << endl;
		glFinish();
		nGotStart = 0;
		break;
	case 'q':
		exit(1);
		break;
	}
}

//------------------------------------------------------------------------------
//-  Constructor
//------------------------------------------------------------------------------
mypaint::mypaint() {
	nMode = MODE_POINTS;
	nStartX = 0;
	nStartY = 0;
	currentDrawColor = BLACK;

}

//------------------------------------------------------------------------------
//-  Keyboard callback function
//------------------------------------------------------------------------------
void keyDownCallback(unsigned char key, int x, int y) {
	instance->keyDown(key, x, y);
}

//------------------------------------------------------------------------------
//-  User Menu Choice callback function
//------------------------------------------------------------------------------
void userChoiceCallBack(int value) {
	instance->userChoice(value);
}

//------------------------------------------------------------------------------
//-  Redraw window callback function
//------------------------------------------------------------------------------
void redrawCallback() {
	instance->redraw();
}

//------------------------------------------------------------------------------
//-  Window reshape callback function
//------------------------------------------------------------------------------
void reshapeCallback(int w, int h) {
	instance->reshape(w, h);
}

// Track whether the left button is pressed
int buttonDown = 0;

// Mouse click function
//   button = GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON
//   state =  GLUT_UP or GLUT_DOWN
//   x, y = mouse location
void mouseClickCallback(int button, int state, int x, int y) {

	// Catch left mouse buttons and pass them to the user.
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		instance->mouseClick(x / nPixelSize,
				((screenHeight - 1) - y) / nPixelSize);
		buttonDown = 1;
	} else if (button == GLUT_LEFT_BUTTON) {
		buttonDown = 0;
	}
}

//***************************************************************************
// DESTRUCTOR
//***************************************************************************
mypaint::~mypaint() {
}

//------------------------------------------------------------------------------
//-  Handles user menu choices
//------------------------------------------------------------------------------
void mypaint::userChoice(int value) {
	switch (value) {
	case 1:
		cerr << "Point mode\n";
		nMode = MODE_POINTS;
		break;
	case 2:
		cerr << "Line mode\n";
		nMode = MODE_LINES;
		nGotStart = 0;
		break;
	case 3:
		cerr << "Circle mode\n";
		nMode = MODE_CIRCLES;
		nGotStart = 0;
		break;
	case 4:
		cerr << "Rectangle mode\n";
		nMode = MODE_RECTANGLES;
		nGotStart = 0;
		break;
	case 5:
		cerr << "Floodfill mode\n";
		nMode = MODE_FLOODFILL;
		nGotStart = 0;
		break;
	case 6:
		if (nPixelSize == SMALL_PIX_SIZE)
			nPixelSize = BIG_PIX_SIZE;
		else
			nPixelSize = SMALL_PIX_SIZE;
		glPointSize((GLfloat) nPixelSize);
		glClear( GL_COLOR_BUFFER_BIT);
		glFinish();
		nGotStart = 0;
		numPixelsX = screenWidth / nPixelSize;
		numPixelsY = screenHeight / nPixelSize;
		cerr << "Pixel size now: " << nPixelSize << endl;
		break;
	case 7:
		glClear(GL_COLOR_BUFFER_BIT);
		cerr << "Screen cleared." << endl;
		glFinish();
		nGotStart = 0;
		break;
	case 8:
		exit(1);
		break;
	}

}

//------------------------------------------------------------------------------
//-  Helper function to create menu
//------------------------------------------------------------------------------
void createMenu(void) {
	glutCreateMenu(userChoiceCallBack);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Circle", 3);
	glutAddMenuEntry("Rectangle", 4);
	glutAddMenuEntry("FloodFill", 5);
	glutAddMenuEntry("Switch Pixel Size", 6);
	glutAddMenuEntry("Clear Screen", 7);
	glutAddMenuEntry("Exit", 8);
	glutAttachMenu ( GLUT_RIGHT_BUTTON);
}

//==============================================================================
//=  Main program                                                              =
//==============================================================================
int main(int argc, char** argv) {
	instance = new mypaint();
	cerr << controlsMessage;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(windowName);
	glutDisplayFunc(redrawCallback);
	glutMouseFunc(mouseClickCallback);
	glutKeyboardFunc(keyDownCallback);
	glutReshapeFunc(reshapeCallback);
	createMenu();
	instance->initialize();

	glutMainLoop();
	return 0;
}

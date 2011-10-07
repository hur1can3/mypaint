//======================================================== file = mypaint.h  ===
//=  Author: Matthew Levandowski                                               =
//=          Email: levandma@gvsu.edu                                          =
//=          Grand Valley State University                                     =
//=          CIS 367 Computer Graphics                                         =
//==============================================================================
// windows includes
#ifdef WIN32
# include <windows.h>
#endif

// apple includes
#ifdef __APPLE__  // detect OS X
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>

// linux includes
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif

// includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
//-  Class that holds floats for rgb colors
//------------------------------------------------------------------------------
class RGBColor
{
public:
	float r,g,b;

	// A default constructor, makes the color white
	RGBColor()
	{
		r = g = b = 1.0;
	}
	RGBColor(const float& r, const float& g, const float& b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

};

//------------------------------------------------------------------------------
//-  main mypaint class
//------------------------------------------------------------------------------
class mypaint
{
public:
	mypaint (); // constructor
	virtual ~mypaint(); // destructor

	void mouseClick(int x, int y);
	void redraw();
	void reshape (int w, int h);
	void keyDown(unsigned char key, int x, int y);
	void initialize();
	void userChoice (int value);

private:

	void drawLine(int startx, int starty, int endx, int endy);
	void drawCircle(int x, int y, int radius);
	void drawRectangle(int startx, int starty, int endx, int endy);
	void flood_fill(int x, int y, RGBColor fillc);
	void setPixel(const int& x, const int& y, const RGBColor& color);
	RGBColor getPixel(const int& x, const int& y);

	// Modes
	enum
	{
		MODE_POINTS=0,
		MODE_LINES,
		MODE_CIRCLES,
		MODE_RECTANGLES,
		MODE_FLOODFILL,
	};

	int nMode;

	// Mouse start location for those primitives that take two points.
	int nGotStart;
	int nStartX;
	int nStartY;

	// Number of pixels in the x and y directions
	// Automatically updated if the user resizes the window
	int numPixelsX;
	int numPixelsY;

	// This is the color currently being used to draw to the screen.
	RGBColor currentDrawColor;
};

// GLOBAL:
void redrawCallback();
void keyDownCallback(unsigned char key, int x, int y);
void redrawCallback();
void reshapeCallback(int w, int h);

// GLOBAL constants:
static const char *controlsMessage = "mypaint Controls:\n"
	"  Modes:\n"
	"    'P' points \n"
	"    'L' lines \n"
	"    'C' circles \n"
	"    'R' rectangles\n"
	"    'F' floodfill\n"
	"  \n"
	"  Change color:\n"
	"    [0-9]\n"
	"  \n"
	"  Other Controls:\n"
	"    'X' clear screen\n"
	"    'B' toggle pixel size (big or small)\n"
	"    'Q' quit\n\n";

// Setup simple colors
static const RGBColor WHITE(1.0,1.0,1.0);
static const RGBColor BLACK(0.0,0.0,0.0);
static const RGBColor RED (1.0,0.0,0.0);
static const RGBColor GREEN(0.0,1.0,0.0);
static const RGBColor BLUE (0.0,0.0,1.0);
static const RGBColor CYAN (0.0,1.0,1.0);
static const RGBColor YELLOW (1.0,1.0,0.0);
static const RGBColor MAGENTA(1.0,0.0,1.0);
static const RGBColor ORANGE(1.0, 0.5, 0.0);
static const RGBColor PURPLE(1.0, 0.0, 1.0);

// colors array
static const RGBColor colors[10] = { WHITE, BLACK, RED, GREEN, BLUE, CYAN,
		YELLOW, MAGENTA, ORANGE, PURPLE };

// Virtual pixel sizes
static const int SMALL_PIX_SIZE = 2;
static const int BIG_PIX_SIZE = 8;
static int nPixelSize = SMALL_PIX_SIZE;

// Screen size
static int screenHeight = 480;
static int screenWidth = 640;

//make a fake instance
static mypaint *instance = NULL;

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <iostream>
#include <vector>
#include <cmath>

#include "ImageIO/ImageIO.h"

/** Special vars */

static const unsigned char ESC = 27;

/** ImageIO handlers */

static int inW, inH, outW, outH;
static unsigned char *outPixmap;
static std::string input, output;

static ImageIO ioOrigin = ImageIO();
static ImageIO ioWarped = ImageIO();

/** Warp functions */

float X(float u, float v)
{
	return u + 2 * v;
}

float Y(float u, float v)
{
	return v;
}

float U(float x, float y)
{
	return x - 2 * y;
}

float V(float x, float y)
{
	return y;
}

/** To draw on screen, outpixmap ALWAYS uses RGBA */

void setupOutPixmap(int w, int h) 
{
	outPixmap = new unsigned char[RGBA * w * h];
	for (int i = 0; i < h; ++i) 
		for (int j = 0; j < w; ++j) 
			for (int channel = 0; channel < RGBA; ++channel)
				/** Init alpha channel to 0, make no color value pixel to be transparent */
				outPixmap[(i * h) * RGBA + channel] = 0;
}

/** Calculate output image size */

void setOutputSize(int &w, int &h)
{
	int maxX, maxY = 0;
	int *xs = new int[2]; // x coordinates
	int *ys = new int[2]; // y coordinates

	xs[0] = 0;	xs[1] = inW; 
	ys[0] = 0;	ys[1] = inH;

	for (int i = 0; i < 2; ++i) {
	 	for (int j = 0; j < 2; ++j) {
	 		maxX = fmax(maxX, X(xs[i], ys[j]));
	 		maxY = fmax(maxY, Y(xs[i], ys[j]));
	 	}
	}

	w = maxX;
	h = maxY;
	setupOutPixmap(w, h);
}


void forwardMap(int inW, int inH, unsigned char *inPixmap)
{

}

void inverseMap(int inW, int inH, unsigned char *inPixmap)
{
	int k, l;
	float x, y, u, v;

	setOutputSize(outW, outH);

	for (int i = 0; i < outH; ++i) {
		for (int j = 0; j < outW; ++j) {
			/** Calculate corresponding coordinates, use nearest-neighbour interpolation */
			x = (float)(j + 0.5) / outW;
			y = (float)(i + 0.5) / outH;
			u = U(x, y);
			v = V(x, y);
			k = (int)floor(v * inH);
			l = (int)floor(u * inW);

			if (k < 0 || k > inH || l < 0 || l > inW) 
				continue;

			for (int channel = 0; channel < RGBA; ++channel) 
				outPixmap[(i * outW + j) * RGBA + channel] = inPixmap[(k * inW + l) * RGBA + channel];

		}
	}
}

bool parseCommandLine(int argc, char* argv[]) 
{
  switch (argc) {
  case 2: case 3:
  	input  = argv[1];
    output = argv[2] != NULL ? argv[2] : "output.png";
    return true; break;

  default:
  	std::cerr << "Usage: warp inimage.png [outimage.png]" << std:: endl;
    exit(1);
  	return false; break;
  }
}

void loadImage()
{
	ioOrigin.loadFile(input);

	inW = ioOrigin.getWidth();
	inH = ioOrigin.getHeight();

	inverseMap(inW, inH, ioOrigin.pixmap);
	ioWarped.setPixmap(outW, outH, outPixmap);
}

void displayOriginWindow() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ioOrigin.draw();
}

void displayWarpedWindow()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ioWarped.draw();
}

void handleReshape(int width, int height) 
{
	glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);	
}

void handleKeyboard(unsigned char key, int x, int y) 
{
	switch(key) {
		case 'w': case 'W': 
			ioWarped.saveImage(output); break;
  	case 'q': case 'Q': case ESC: 
  		exit(0); break;
  }	
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

  if (parseCommandLine(argc, argv)) {
		loadImage();	
	}

	// Origin image window
	glutInitWindowSize(inW, inH);
	glutCreateWindow("Original Image");
	glutDisplayFunc(displayOriginWindow);
	glutKeyboardFunc(handleKeyboard);
	glutReshapeFunc(handleReshape);

	// Warped image window
	glutInitWindowSize(outW, outH);
	glutCreateWindow("Warped Image");
	glutDisplayFunc(displayWarpedWindow);
	glutKeyboardFunc(handleKeyboard);
	glutReshapeFunc(handleReshape);

	glutMainLoop();
	
	return 0;
}


/* Header File */

/* Ryan McCoppin 's project 1
 * September 14, 2012
 * Computer Vision, CEG 7550
 * Due: October 11, 2012 @ 3:30pm

 */

#pragma once

#include <iostream> // Used for debugging & learning code
#include <algorithm> // Use for sorting
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <time.h>
#include <vector>
#include "glut.h"
#include "byteimage.h"
#include "windows.h"

#define PI 4*atan(1.0)

// Point structure
struct point {
	int x;
	int y;
	int I;
	int radx;
	int rady;
	int area;
	double avg;
	double r;
};

inline void display_point(point& p) {
	std::cout << "point: " << p.x << ", " << p.y << ", I: " << p.I << ", area avg: " << p.avg << std::endl;
	std::cout << "\tarea: " << p.area << ", radx: " << p.radx << ", rady: " << p.rady << std::endl;
}

ByteImage openImageDialog();
void saveImageDialog(const ByteImage&);
void viewer(const ByteImage&);
void bilateral_filter(ByteImage&, double, double);
void gaussian_filter(ByteImage&, double);
void edge_detector(ByteImage&, double);
//void edge_detector(ByteImage&, double, double);
void edge_gradients(ByteImage&, double);
void edge_gradients(ByteImage&, double, double);
double kmeans(ByteImage&, int);
double kmeans(ByteImage&, int, double);
void dmeans(ByteImage&, int);
void dmeans(ByteImage&, int, double);
void registration(ByteImage&, ByteImage&, std::vector<point>, int);
void display();




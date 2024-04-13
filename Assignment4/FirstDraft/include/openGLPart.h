#pragma once
#include<GL/glut.h>
#include<math.h>
#include <iostream>
#include <vector>
#include <tuple>
#define X_MAX 1
#define Y_MAX 1
#define EYE_Z 5
#define TAIL 0.2
#define THICKNESS 0.03
#define END 0.01
#define INITIAL_LENGTH 0.5
#define MAX_SEGMENTS 1000
using namespace std;
using std::vector;

enum ColorPattern {
    CHECKERED,
    V_STRIPPED,
    H_STRIPPED,
    SOLID
};

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};
typedef struct {
    double x, y, z;
}Point;

extern Point snakeBody[MAX_SEGMENTS];
extern int bodySegments;
extern GLfloat randomColors[300000];
void torus(int lats, int longs, double fromTheta, double toTheta, double r, double R, ColorPattern color);
void cylinder(int lats, int longs, double r, double L, ColorPattern color);
void cone(int lats, int longs, double r, double R, double L, ColorPattern color);
void sphere(double r, int lats, int longs, ColorPattern color);
void ellipsoid(double rx, double ry, double rz, int lats, int longs, ColorPattern color);
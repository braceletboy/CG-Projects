#pragma once
#include<GL/glut.h>
#include<math.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
using namespace std;
#define X_MAX 1.0
#define Y_MAX 1.0
#define EYE_Z 5.0
#define DNEAR 0
#define BUNNY_SIZE 0.1
//#define DFAR 4
#define TAIL 0.2
#define TAIL_THICKNESS 0.25//25% of the original thickness... 0.0075
#define THICKNESS 0.03
#define NUMBER_OF_BUNNIES 10
#define GROW_WHEN_EAT 25
#define END 0.01
#define INITIAL_LENGTH 0.5
#define MAX_SEGMENTS 1000
#define SPEED 0.008
#define BLINKING_PERIOD 100
#define FIELD_DEPTH 3
#define INITIAL_Z 2
#define LOW_RES 1
using namespace std;
using std::vector;

enum ColorPattern {
    CHECKERED,
    V_STRIPPED,
    H_STRIPPED,
    SOLID,
    SOLID_WHITE,
    SOLID_BLACK,
    NO_COLOR
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
extern int bodySegments, lats, longs;
extern GLfloat randomColors[300000];
extern std::vector<GLfloat> vertices;
extern std::vector<GLuint> faces;
extern std::vector<GLfloat> normals;
extern int firstIndex;
extern Point bunnyLocation[NUMBER_OF_BUNNIES];
Point diff(Point p2, Point p1);
void torus(int lats, int longs, double fromTheta, double toTheta, double r1, double r2, double R, ColorPattern color, double r0, double tail, double tailRange, double headRange, double rx);
void cone(int lats, int longs, double r1, double r2, double L, ColorPattern color);
void head(double rx, double r, int lats, int longs, ColorPattern color);
void eyeBall(double rx, double ry, double rz, int lats, int longs, ColorPattern color, ColorPattern frontColor, bool orientation);
double length(double r);
double euclideanDistanceSquare(Point p1, Point p2);
Point randomBunnyLocation(int x, int y, int z, int max, int except);
void load_bunny_data(const char file[], std::vector<GLfloat>& vertices, std::vector<GLuint>& faces, std::vector<GLfloat>& normals);
void bunny(double size, Point loc);
void conicSnake(double r, double tailR, ColorPattern color, Point snakeUp);
bool snakeCrossesItself();
int eatsBunny();
#ifndef TOOLS_H
#define TOOLS_H
#include <GL/glut.h>
#define X_MAX 640
#define Y_MAX 480
#define X_POSITION 200
#define Y_POSITION 200
#define SEARCH_RANGE 10
typedef struct {
	GLfloat x;
	GLfloat y;
} Point;
struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
};
void draw_function_cartesian_system(GLfloat init_x, GLfloat final_x, GLfloat(*f)(GLfloat), GLfloat(*inverse)(GLfloat));
GLfloat translated_quadratic(GLfloat x);
GLfloat inverse1_translated_quadratic(GLfloat y);
GLfloat inverse2_translated_quadratic(GLfloat y);
GLfloat translated_sin(GLfloat x);
GLfloat inverse1_translated_sin(GLfloat y);
GLfloat inverse2_translated_sin(GLfloat y);
Color getPixelColor(GLint x, GLint y);
#endif

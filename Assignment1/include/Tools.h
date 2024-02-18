#ifndef TOOLS_H
#define TOOLS_H
#include <GL/glut.h>
#define X_MAX 1080
#define Y_MAX 720
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
GLfloat cosine(GLfloat x);
GLfloat inverse_cosine_1(GLfloat y);
GLfloat inverse_cosine_2(GLfloat y);
GLfloat inverse_cosine_3(GLfloat y);
GLfloat quadratic_first_eq(GLfloat x);
GLfloat inverse_quadratic_first_eq1(GLfloat y);
GLfloat inverse_quadratic_first_eq2(GLfloat y);
GLfloat cubic(GLfloat x);
GLfloat inverse_cubic(GLfloat y);
GLfloat quadratic_second_eq(GLfloat x);
GLfloat inverse_quadratic_second_eq1(GLfloat y);
GLfloat inverse_quadratic_second_eq2(GLfloat y);
Color getPixelColor(GLint x, GLint y);
#endif

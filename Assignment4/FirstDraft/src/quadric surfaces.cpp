#include "openGLPart.h"
void torus(int lats, int longs, double fromTheta, double toTheta, double r, double R, ColorPattern color)
//r is the cross section radius, R is the axial radius, 
//fromTheta and toTheta specify the range of angles covered by the torus. For the complete torus use [0,2pi]
{
    int i, j, k;
    double t, thetaRange = toTheta - fromTheta, twopi = 2 * (double)M_PI, phi0, phi1, theta;
    for (i = 0; i < lats; i++) {
        phi0 = ((i + 1) % lats + 0.5) * twopi / lats;
        phi1 = (i % lats + 0.5) * twopi / lats;
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            if (thetaRange == twopi)
                t = j % longs;
            else if (j == longs)
                continue;
            else
                t = j;
            theta = t * thetaRange / longs + fromTheta;
            switch (color) {
            case CHECKERED:
                glColor3fv(randomColors + 3 * (i * longs + j % longs));
                break;
            case V_STRIPPED:
                glColor3fv(randomColors + 3 * i);
                break;
            case H_STRIPPED:
                glColor3fv(randomColors + 3 * j % longs);
                break;
            case SOLID:
                glColor4f(0, .7, 0, .8);
                break;
            }
            glVertex3f((R + r * cos(phi0)) * cos(theta), 
                (R + r * cos(phi0)) * sin(theta), r * sin(phi0));
            glVertex3f((R + r * cos(phi1)) * cos(theta), 
                (R + r * cos(phi1)) * sin(theta), r * sin(phi1));
        }
        glEnd();
    }
}
void cylinder(int angle_steps, int latitudes, double r, double L, ColorPattern color)//r is the radius and L is the length
{
    if (L <= 0)
        return;
    int i, j, k;
    double twopi = 2 * (double)M_PI, phi0, phi1;
    for (i = 0; i < angle_steps; i++) {
        phi0 = ((i + 1) % angle_steps + 0.5) * twopi / angle_steps;
        phi1 = (i % angle_steps + 0.5) * twopi / angle_steps;
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= latitudes; j++) {
            switch (color) {
            case CHECKERED:
                glColor3fv(randomColors + 3 * (i * latitudes + j % latitudes));
                break;
            case V_STRIPPED:
                glColor3fv(randomColors + 3 * i);
                break;
            case H_STRIPPED:
                glColor3fv(randomColors + 3 * j % latitudes);
                break;
            case SOLID:
                glColor4f(0, .7, 0, .8);
                break;
            }
            glVertex3f(j * L / latitudes, r * cos(phi0), r * sin(phi0));
            glVertex3f(j * L / latitudes, r * cos(phi1), r * sin(phi1));
        }
        glEnd();
    }
}
void cone(int lats, int longs, double r, double R, double L, ColorPattern color)//L is the length of cone, r and R are the radii of head and tail 
{
    int i, j, k;
    double twopi = 2 * (double)M_PI, phi0, phi1, radius;
    for (i = 0; i < lats; i++) {
        phi0 = ((i + 1) % lats + 0.5) * twopi / lats;
        phi1 = (i % lats + 0.5) * twopi / lats;
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            radius = r + (R - r) * j / longs;
            switch (color) {
            case CHECKERED:
                glColor3fv(randomColors + 3 * (i * longs + j % longs));
                break;
            case V_STRIPPED:
                glColor3fv(randomColors + 3 * i);
                break;
            case H_STRIPPED:
                glColor3fv(randomColors + 3 * j % longs);
                break;
            case SOLID:
                glColor4f(0, .7, 0, .8);
                break;
            }
            glVertex3f(j * L / longs, radius * cos(phi0), radius * sin(phi0));
            glVertex3f(j * L / longs, radius * cos(phi1), radius * sin(phi1));
        }
        glEnd();
    }
}
void sphere(double r, int lats, int longs, ColorPattern color) {
    int i, j;
    for (i = 0; i <= lats; i++) {
        double phi0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double phi1 = M_PI * (-0.5 + (double)i / lats);
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            double theta = 2 * M_PI * (double)(j - 1) / longs;
            double x = cos(theta);
            double y = sin(theta);
            switch (color) {
            case CHECKERED:
                glColor3fv(randomColors + 3 * (i * longs + j % longs));
                break;
            case V_STRIPPED:
                glColor3fv(randomColors + 3 * i);
                break;
            case H_STRIPPED:
                glColor3fv(randomColors + 3 * j % longs);
                break;
            case SOLID:
                glColor4f(0, .7, 0, .8);
                break;
            }
            glVertex3f(r * x * cos(phi0), r * y * cos(phi0), r * sin(phi0));
            glVertex3f(r * x * cos(phi1), r * y * cos(phi1), r * sin(phi1));
        }
        glEnd();
    }
}

void ellipsoid(double rx, double ry, double rz, int lats, int longs, ColorPattern color) {
    int i, j;
    for (i = 0; i <= lats; i++) {
        double phi0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double phi1 = M_PI * (-0.5 + (double)i / lats);
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            double theta = 2 * M_PI * (double)(j - 1) / longs;
            double x = cos(theta);
            double y = sin(theta);
            switch (color) {
            case CHECKERED:
                glColor3fv(randomColors + 3 * (i * longs + j % longs));
                break;
            case V_STRIPPED:
                glColor3fv(randomColors + 3 * i);
                break;
            case H_STRIPPED:
                glColor3fv(randomColors + 3 * j % longs);
                break;
            case SOLID:
                glColor4f(0, .7, 0, .8);
                break;
            }
            glVertex3f(rx * x * cos(phi0), ry * y * cos(phi0), rz * sin(phi0));
            glVertex3f(rx * x * cos(phi1), ry * y * cos(phi1), rz * sin(phi1));
        }
        glEnd();
    }
}
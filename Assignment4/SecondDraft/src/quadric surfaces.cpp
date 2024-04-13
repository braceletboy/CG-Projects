#include "openGLPart.h"
void cone(int lats, int longs, double r1, double r2, double L, ColorPattern color)
//L is the length of cone, r1 and r2 are the radii of head and tail 
{
    int i, j, k;
    if (longs == 0)
        longs = 1;
    double twopi = 2 * (double)M_PI, phi0, phi1, radius;
    for (i = 0; i < lats; i++) {
        phi0 = ((i + 1) % lats + 0.5) * twopi / lats;
        phi1 = (i % lats + 0.5) * twopi / lats;
        glBegin(GL_QUAD_STRIP);
        glVertex3f(0, 0,0);
        glVertex3f(0, 0.0001,0.0001);
        for (j = 0; j <= longs; j++) {
            radius = r1 + (r2 - r1) * j / longs;
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
        glVertex3f(L, 0, 0);
        glVertex3f(L, 0.0001, 0.0001);
        glEnd();
    }
}
void eyeBall(double rx, double ry, double rz, int lats, int longs, ColorPattern color, ColorPattern frontColor, bool orientation) {
    int i, j;
    for (i = 0; i <= lats; i++) {
        double phi0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double phi1 = M_PI * (-0.5 + (double)i / lats);
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            double theta = 2 * M_PI * (double)(j - 1) / longs;
            double x = cos(theta);
            double y = sin(theta);
            if (phi0 < 0 && orientation || phi0 >= 0 && !orientation)
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
            else
                if (frontColor == SOLID_BLACK)
                    glColor4f(0, 0, 0, .8);
                else
                    glColor4f(1, 1, 1, .8);
            glVertex3f(rx * x * cos(phi0), ry * y * cos(phi0), rz * sin(phi0));
            glVertex3f(rx * x * cos(phi1), ry * y * cos(phi1), rz * sin(phi1));
        }
        glEnd();
    }
}
void head(double rx, double r, int lats, int longs, ColorPattern color) {
    int i, j;
    for (i = 0; i <= lats; i++) {
        double phi0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double phi1 = M_PI * (-0.5 + (double)i / lats);
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            double theta = M_PI * (-0.5 + (double)(j) / longs);
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
            glVertex3f(rx * x * cos(phi0), r * y * cos(phi0), r * sin(phi0));
            glVertex3f(rx * x * cos(phi1), r * y * cos(phi1), r * sin(phi1));
        }
        glEnd();
    }
    glPushMatrix();
    glTranslatef(rx / 2, r / 3, r / -1.3);
    eyeBall(r / 2, r / 4, r / 5, lats, longs, color, SOLID_WHITE, false);
    glPushMatrix();
    glTranslatef(r/10,0, 0);
    eyeBall(r / 4, r / 5, r / 5, lats, longs, color, SOLID_BLACK, false);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(rx / 2, -r / 3, r / -1.3);
    eyeBall(r / 2, r / 4, r / 5, lats, longs, color, SOLID_WHITE, false);
    glPushMatrix(); 
    glTranslatef(r/10, 0, 0 );
    eyeBall(r / 4, r / 5, r / 5, lats, longs, color, SOLID_BLACK, false);
    glPopMatrix();
    glPopMatrix();
}
void torus(int lats, int longs, double fromTheta, double toTheta, double r1, double r2, double R, ColorPattern color, double r0, double tail, double tailRange, double headRange, double rx)
//if r0 == -1:
////[r1, r2] is the range of cross section radius, R is the axial radius, 
////fromTheta and toTheta specify the range of angles covered by the torus. For the complete torus use [0,2pi]
//otherwise, 
{
    int i, j, k;
    double r = r1;
    double t, thetaRange = toTheta - fromTheta, twopi = 2 * (double)M_PI, phi0, phi1, theta;

    if (tailRange > 0) {
        if (thetaRange > 0) {
            fromTheta = toTheta - tailRange;
            thetaRange = tailRange;
        }
        else {
            fromTheta = toTheta + tailRange;
            thetaRange = -tailRange;
        }
        longs = 4 * longs * tailRange / twopi;
        if (longs == 0)
            longs = 1;
    }
    if (headRange > 0) {
        if (thetaRange > 0) {
            toTheta = fromTheta + headRange;
            thetaRange = headRange;
        }
        else {
            toTheta = fromTheta - headRange;
            thetaRange = -headRange;
        }
        longs = 4 * longs * headRange / twopi;
        if (longs == 0)
            longs = 1;
    }

    double middleTheta = (r0 < 0) ? -10000 : (r1 == r2) ? toTheta : tail * thetaRange / (M_PI * R / 2) + fromTheta;
    double longs0 = longs * (middleTheta - fromTheta) / thetaRange;
    for (i = 0; i < lats; i++) {
        phi0 = ((i + 1) % lats + 0.5) * twopi / lats;
        phi1 = (i % lats + 0.5) * twopi / lats;
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs + 1; j++) {
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
            if (r0 >= 0 && (theta < middleTheta && thetaRange > 0 || theta > middleTheta && thetaRange < 0))
                r = r0 + (r1 - r0) * j / longs0;
            else
                r = r1 + (r2 - r1) * (r0 < 0 ? j : j - longs0) / (r0 < 0 ? longs : longs0 == longs ? 1 : longs - longs0);
            glVertex3f((R + r * cos(phi0)) * cos(theta), (R + r * cos(phi0)) * sin(theta), r * sin(phi0));
            glVertex3f((R + r * cos(phi1)) * cos(theta), (R + r * cos(phi1)) * sin(theta), r * sin(phi1));
        }
        glEnd();
    }
    if (headRange > 0) {
        phi0 = 0;
        double tx0 = (R + r * cos(phi0)) * cos(toTheta);
        double ty0 = (R + r * cos(phi0)) * sin(toTheta);
        double tz0 = sin(phi0);
        double theta0 = toTheta - thetaRange / longs;
        double dx0 = tx0 - (R + r * cos(phi0)) * cos(theta0);
        double dy0 = ty0 - (R + r * cos(phi0)) * sin(theta0);
        double degree,radian;
        double tx1;// = r * sin(radian);
        double ty1;// = -r * cos(radian);
        degree = 180 * thetaRange / M_PI;
        if (thetaRange < 0 && dx0 > 0) {
            tx1 = r * cos(M_PI / 2 - thetaRange);
            ty1 = -r * sin(M_PI / 2 - thetaRange);
        }
        else if (thetaRange > 0 && dx0 > 0) {
            tx1 = -r * cos(M_PI / 2 - thetaRange);
            ty1 = r * sin(M_PI / 2 - thetaRange);
        }
        else if (thetaRange < 0 && dx0 < 0) {
            tx1 = -r * cos(M_PI / 2 - thetaRange);
            ty1 = r * sin(M_PI / 2 - thetaRange);
            degree += 180;
        }
        else {
            tx1 = r * cos(M_PI / 2 - thetaRange);
            ty1 = -r * sin(M_PI / 2 - thetaRange);
            degree += 180;
        }
        glPushMatrix();
        glTranslatef(tx0, ty0, tz0);
        glTranslatef(tx1, ty1, 0);
        glRotatef(degree, 0, 0, 1);
        for (i = 0; i <= lats; i++) {
            double Phi0 = M_PI * (-0.5 + (double)(i - 1) / lats);
            double Phi1 = M_PI * (-0.5 + (double)i / lats);
            glBegin(GL_QUAD_STRIP);
            for (j = 0; j <= longs; j++) {
                double theta = M_PI * (-0.5 + (double)(j) / longs);
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
                glVertex3f(rx * x * cos(Phi0), r * y * cos(Phi0),r * sin(Phi0));
                glVertex3f(rx * x * cos(Phi1), r * y * cos(Phi1),r * sin(Phi1));
            }
            glEnd();
        }
        glPushMatrix();
        glTranslatef(rx / 2, r / 3, r / -1.3);
        eyeBall(r / 2, r / 4, r / 5, lats, longs, color, SOLID_WHITE, false);
        eyeBall(r / 4, r / 5, r / 5, lats, longs, color, SOLID_BLACK, false);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(rx / 2, -r / 3, r / -1.3);
        eyeBall(r / 2, r / 4, r / 5, lats, longs, color, SOLID_WHITE, false);
        eyeBall(r / 4, r / 5, r / 5, lats, longs, color, SOLID_BLACK, false);
        glPopMatrix();
        glPopMatrix();
    }
}
#include "openGLPart.h"
void bunny(double size, Point loc) {
    glPushMatrix();
    glTranslatef(loc.x, loc.y, loc.z);
    glScalef(size, size, size);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < faces.size();i += 3) {
        glVertex3f(vertices[3 * faces[i]], vertices[3 * faces[i] + 1], vertices[3 * faces[i] + 2]);
        glVertex3f(vertices[3 * faces[i + 1]], vertices[3 * faces[i + 1] + 1], vertices[3 * faces[i + 1] + 2]);
        glVertex3f(vertices[3 * faces[i + 2]], vertices[3 * faces[i + 2] + 1], vertices[3 * faces[i + 2] + 2]);
    }
    glEnd();
    glPopMatrix();
}

Point randomBunnyLocation(int x, int y, int z, int max, int except) {
    while (true) {
        Point loc = { (x - 2 * BUNNY_SIZE) * ((double)rand() / RAND_MAX - .5),
            (y - 2 * BUNNY_SIZE) * ((double)rand() / RAND_MAX - .5),
            (z - 2 * BUNNY_SIZE) * ((double)rand() / RAND_MAX - .5) };
        bool acceptable = true;
        for (int i = 0;i <= bodySegments - 1;i++) {
            int current = (i + firstIndex) % MAX_SEGMENTS;
            int next = (current + 1) % MAX_SEGMENTS;
            Point p = diff(snakeBody[next], snakeBody[current]);
            if (p.x != 0 && (loc.x - snakeBody[current].x) * (loc.x - snakeBody[next].x) <= 0 &&
                (loc.y - snakeBody[current].y) * (loc.y - snakeBody[current].y)
                + (loc.z - snakeBody[current].z) * (loc.z - snakeBody[current].z)
                <= (THICKNESS + BUNNY_SIZE) * (THICKNESS + BUNNY_SIZE)) {
                acceptable = false;
                break;
            }
            if (p.y != 0 && (loc.y - snakeBody[current].y) * (loc.y - snakeBody[next].y) <= 0 &&
                (loc.x - snakeBody[current].x) * (loc.x - snakeBody[current].x)
                + (loc.z - snakeBody[current].z) * (loc.z - snakeBody[current].z)
                <= (THICKNESS + BUNNY_SIZE) * (THICKNESS + BUNNY_SIZE)) {
                acceptable = false;
                break;
            }
            if (p.z != 0 && (loc.z - snakeBody[current].z) * (loc.z - snakeBody[next].z) <= 0 &&
                (loc.x - snakeBody[current].x) * (loc.x - snakeBody[current].x)
                + (loc.y - snakeBody[current].y) * (loc.y - snakeBody[current].y)
                <= (THICKNESS + BUNNY_SIZE) * (THICKNESS + BUNNY_SIZE)) {
                acceptable = false;
                break;
            }
        }
        if (!acceptable)
            continue;
        for (int i = 0;i <= bodySegments;i++) {
            int current = (i + firstIndex) % MAX_SEGMENTS;
            if ((snakeBody[current].x - loc.x) * (snakeBody[current].x - loc.x) +
                (snakeBody[current].y - loc.y) * (snakeBody[current].y - loc.y) +
                (snakeBody[current].z - loc.z) * (snakeBody[current].z - loc.z) <=
                (THICKNESS + BUNNY_SIZE) * (THICKNESS + BUNNY_SIZE)) {
                acceptable = false;
                break;
            }
        }
        if (!acceptable)
            continue;
        for (int i = 0;i < max;i++) {
            if (i == except)
                continue;
            if (euclideanDistanceSquare(loc, bunnyLocation[i]) <= 4 * BUNNY_SIZE * BUNNY_SIZE) {
                acceptable = false;
                break;
            }
        }
        if (acceptable)
            return loc;
    }
}
int eatsBunny() {
    int head = (bodySegments + firstIndex) % MAX_SEGMENTS;
    for (int i = 0; i < NUMBER_OF_BUNNIES;i++)
        if (euclideanDistanceSquare(snakeBody[head], bunnyLocation[i]) <= (THICKNESS + BUNNY_SIZE) * (THICKNESS + BUNNY_SIZE))
            return i;
    return -1;
}
bool snakeCrossesItself() {
    int head0 = (bodySegments + firstIndex - 1) % MAX_SEGMENTS;
    int head1 = (head0 + 1) % MAX_SEGMENTS;
    Point direction = diff(snakeBody[head1], snakeBody[head0]);
    for (int i = 0;i <= bodySegments - 2;i++) {
        int current = (i + firstIndex) % MAX_SEGMENTS;
        int next = (current + 1) % MAX_SEGMENTS;
        Point p = diff(snakeBody[next], snakeBody[current]);
        if (direction.x == 0 && p.x == 0) {//on y-z plane
            if (snakeBody[head0].x != snakeBody[current].x)
                continue;
            if (direction.y == 0 && p.y == 0) {
                if (snakeBody[head0].y == snakeBody[current].y &&
                    ((snakeBody[head0].z - snakeBody[current].z) * (snakeBody[head0].z - snakeBody[next].z) < 0 ||
                        (snakeBody[head1].z - snakeBody[current].z) * (snakeBody[head1].z - snakeBody[next].z) < 0))
                    return true;
            }
            if (direction.z == 0 && p.z == 0) {
                if (snakeBody[head0].z == snakeBody[current].z &&
                    ((snakeBody[head0].y - snakeBody[current].y) * (snakeBody[head0].y - snakeBody[next].y) < 0 ||
                        (snakeBody[head1].y - snakeBody[current].y) * (snakeBody[head1].y - snakeBody[next].y) < 0))
                    return true;
            }
            if ((snakeBody[head0].y - snakeBody[current].y) * (snakeBody[head1].y - snakeBody[next].y) < 0 &&
                (snakeBody[head0].z - snakeBody[current].z) * (snakeBody[head1].z - snakeBody[next].z) < 0)
                return true;
            continue;
        }
        if (direction.y == 0 && p.y == 0) {// on x-z plane
            if (snakeBody[head0].y != snakeBody[current].y)
                continue;
            if (direction.z == 0 && p.z == 0)
                if (snakeBody[head0].z == snakeBody[current].z &&
                    ((snakeBody[head0].x - snakeBody[current].x) * (snakeBody[head0].x - snakeBody[next].x) < 0 ||
                        (snakeBody[head1].x - snakeBody[current].x) * (snakeBody[head1].x - snakeBody[next].x) < 0))
                    return true;
            if ((snakeBody[head0].x - snakeBody[current].x) * (snakeBody[head1].x - snakeBody[next].x) < 0 &&
                (snakeBody[head0].z - snakeBody[current].z) * (snakeBody[head1].z - snakeBody[next].z) < 0)
                return true;
            continue;
        }
        if (snakeBody[head0].z != snakeBody[current].z)
            continue;
        if ((snakeBody[head0].x - snakeBody[current].x) * (snakeBody[head1].x - snakeBody[next].x) < 0 &&
            (snakeBody[head0].y - snakeBody[current].y) * (snakeBody[head1].y - snakeBody[next].y) < 0)
            return true;
    }
    return false;
}
double length(double r) {
    //The length of snake body except the lengths of head (2r) and tail (2r)
    double sum = 0;
    Point p, p2;
    for (int i = 0;i <= bodySegments - 1;i++) {
        int current = (i + firstIndex) % (MAX_SEGMENTS);
        int next = (current + 1) % (MAX_SEGMENTS);
        int nextnext = (next + 1) % (MAX_SEGMENTS);
        p = diff(snakeBody[next], snakeBody[current]);
        double len = (p.x < 0 ? -p.x : p.x) + (p.y < 0 ? -p.y : p.y) + (p.z < 0 ? -p.z : p.z);
        if (len >= 4 * r)
            sum += (len - 4 * r);
        else if (i == 0 && len < 2 * r) {
            sum += 2 * r * (acos(1 - len / (2 * r)) - 1);
            continue;
        }
        else if (i == 0 && len >= 2 * r) {
            sum += M_PI * r - 4 * r + len;
            continue;
        }
        else if (i == bodySegments - 1) {
            sum += 2 * r * acos(1 - len / (4 * r));
        }
        if (i < bodySegments - 1) {
            p2 = diff(snakeBody[nextnext], snakeBody[next]);
            double len2 = (p2.x < 0 ? -p2.x : p2.x) + (p2.y < 0 ? -p2.y : p2.y) + (p2.z < 0 ? -p2.z : p2.z);
            if (len2 >= 4 * r)
                sum += M_PI * r;
        }
    }
    return sum;
}

void conicSnake(double r, double tailR, ColorPattern color, Point snakeUp) {
    Point d, nextD;
    double direction;
    double l = length(r);
    double tailLength = 0;
    bool headRotation = false;
    double l0 = 0, l1 = 0, r1 = 0, r2 = 0;
    for (int i = 1; i <= bodySegments; i++) {
        int current = (i + firstIndex) % (MAX_SEGMENTS);
        int prev = (i + firstIndex - 1) % (MAX_SEGMENTS);
        if (i > 2)
            l0 = l1 + (M_PI)*r;
        else if (i == 2)
            if (abs(direction) > 4 * r)
                l0 = l1 + (M_PI)*r;
            else if (abs(direction) >= 2 * r)
                l0 = M_PI * r - 2 * r + tailLength;
            else// if (abs(direction) >= 0.9194 * r)
                l0 = 2 * r * (acos(1 - abs(direction) / (2 * r)) - 1);// positive if abs(direction) >= 0.9194 * r
        glPushMatrix();
        glTranslatef(snakeBody[prev].x, snakeBody[prev].y, snakeBody[prev].z);
        d = diff(snakeBody[current], snakeBody[prev]);
        if (d.y != 0) {
            direction = d.y;
            glRotatef(90, 0, 0, 1);
        }
        else if (d.z != 0) {
            direction = d.z;
            glRotatef(-90, 0, 1, 0);
        }
        else
            direction = d.x;


        if (i == 1) {//tail
            tailLength = abs(direction) >= 4 * r ? 2 * r : abs(direction) > 2 * r ? abs(direction) - 2 * r : 0;
            if (tailLength > 0) {
                glTranslatef(direction > 0 ? 0 : -tailLength, 0, 0);
                cone(lats, longs * tailLength, direction > 0 ? 0 : tailR * tailLength / 2,
                    direction > 0 ? tailR * tailLength / 2 : 0, tailLength, color);
                glTranslatef(direction > 0 ? tailLength : 0, 0, 0);
            }
            else {
                glTranslatef(direction > 0 ? direction - 2 * r : 2 * r + direction, 0, 0);
            }
        }
        else
            glTranslatef(direction > 0 ? 2 * r : -2 * r, 0, 0);
        if (l0 < 0) {//i = 2
            double tailRes = -l0;
            glTranslatef(direction > 0 ? 0 : -tailRes, 0, 0);
            cone(lats, longs * tailRes, direction > 0 ? r2 : tailR * r,
                direction > 0 ? tailR * r : r2, tailRes, color);
            glTranslatef(direction > 0 ? tailRes : 0, 0, 0);
            l0 = 0;
            direction = direction > 0 ? direction - tailRes : direction + tailRes;
        }
        double dir = abs(direction);

        if (dir > 4 * r) {
            double temp = dir - 4 * r;
            l1 = l0 + temp;
            r1 = tailR * r + (1 - tailR) * l1 * r / l;
            glTranslatef(direction > 0 ? 0 : -temp, 0, 0);
            if (direction < 0)
                cone(lats, longs * temp, r1, tailR * r + (1 - tailR) * r * l0 / l, temp, color);
            else
                cone(lats, longs * temp, tailR * r + (1 - tailR) * r * l0 / l, r1, temp, color);
            glTranslatef(direction > 0 ? temp : 0, 0, 0);
        }
        else if (dir == 4 * r) {
            l1 = l0;
            r1 = tailR * r + (1 - tailR) * l1 * r / l;
        }
        else if (dir > 2 * r && i == 1) {
            l1 = 0;
            r1 = tailR * r;
        }
        else if (dir <= 2 * r && i == 1) {
            l1 = 0;
            r1 = tailR * r;
        }
        else if (i == bodySegments) {
            headRotation = true;
        }
        else {//error

        }
        if (i == bodySegments && !headRotation) {
            glPushMatrix();
            //if (snakeUp.z < 0 || snakeUp.x > 0)
             //   glRotatef(180, d.x!= 0? 1: 0, d.y != 0? 1: 0, d.z != 0? 1: 0);
            //if (snakeUp.y != 0)
             //   glRotatef(snakeUp.y > 0 ? -180 : 180, d.x != 0 ? 1 : 0, d.y != 0 ? 1 : 0, d.z != 0 ? 1 : 0);
            head(direction > 0 ? 2 * r : -2 * r, r, lats, longs * M_PI * r, color);
            glPopMatrix();
        }
        else if (i == bodySegments && headRotation)
        {

        }
        else {//turn
            nextD = diff(snakeBody[(current + 1) % (MAX_SEGMENTS)], snakeBody[current]);
            double l2, tailRemainder = -1, arc = -1, headAngle = -1, len = 0;
            double r0 = -1;
            if (i == bodySegments - 1 && (len = abs(nextD.x + nextD.y + nextD.z)) < 4 * r) {
                //r1 = r2 = r;
                r2 = r;
                l2 = l;
                headAngle = acos(1 - len / (4 * r));
                r1 = tailR * r + (1 - tailR) * l1 * r / l;//(l = l + 2 * r * headAngle);
            }
            else if (i > 1 || tailLength == 2 * r) {
                l2 = l1 + M_PI * r;
                r2 = tailR * r + (1 - tailR) * l2 * r / l;
            }
            else if (tailLength > 0) {
                tailRemainder = 2 * r - tailLength;
                l2 = M_PI * r - tailRemainder;
                r0 = tailR * tailLength / 2;
                r2 = tailR * r + (1 - tailR) * l2 * r / l;
            }
            else {
                arc = 2 * r * acos(1 - dir / (2 * r));
                if (arc > 2 * r) {
                    tailRemainder = 2 * r;
                    r0 = 0;
                    l2 = arc - 2 * r;
                    r2 = tailR * r + (1 - tailR) * l2 * r / l;
                }
                else {
                    tailRemainder = arc;
                    r0 = 0;
                    r2 = r1 = tailR * arc / 2;
                }
            }


            if (d.x == 0 && nextD.x == 0) {//in y-z plane
                if (d.z == 0) {//y-->z
                    glTranslatef(0, 0, nextD.z > 0 ? 2 * r : -2 * r);
                    glRotatef(90, 1, 0, 0);
                    double torusAngle = nextD.z > 0 ? -M_PI / 2 : M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.y > 0 ? 0 : 2 * torusAngle, r1, r2, 2 * r, color, r0, tailRemainder, arc / (2 * r), headAngle, 2 * r);
                }
                else {//z-->y
                    glTranslatef(0, nextD.y > 0 ? 2 * r : -2 * r, 0);
                    glRotatef(-90, 0, 0, 1);
                    double torusAngle = d.z > 0 ? M_PI / 2 : -M_PI / 2;
                    torus(lats, longs * M_PI * r, nextD.y > 0 ? 0 : 2 * torusAngle, torusAngle, r1, r2, 2 * r, color, r0, tailRemainder, arc / (2 * r), headAngle, 2 * r);
                }
            }
            else if (d.y == 0 && nextD.y == 0) {//in z-x plane 
                if (d.x == 0) {//z-->x
                    glTranslatef(0, 0, nextD.x > 0 ? -2 * r : 2 * r);
                    glRotatef(-90, 1, 0, 0);
                    double torusAngle = nextD.x > 0 ? -M_PI / 2 : M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.z > 0 ? 0 : 2 * torusAngle, r1, r2, 2 * r, color, r0, tailRemainder, arc / (2 * r), headAngle, 2 * r);
                }
                else {//x-->z 
                    glTranslatef(0, 0, nextD.z > 0 ? 2 * r : -2 * r);
                    glRotatef(-90, 1, 0, 0);
                    double torusAngle = nextD.z > 0 ? M_PI / 2 : -M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.x > 0 ? 0 : 2 * torusAngle, r1, r2, 2 * r, color, r0, tailRemainder, arc / (2 * r), headAngle, 2 * r);
                }
            }
            else {//in x-y plane
                if (d.y == 0) {//x-->y
                    glTranslatef(0, nextD.y > 0 ? 2 * r : -2 * r, 0);
                    double torusAngle = nextD.y > 0 ? -M_PI / 2 : M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.x > 0 ? 0 : 2 * torusAngle, r1, r2, 2 * r, color, r0, tailRemainder, arc / (2 * r), headAngle, 2 * r);
                }
                else {//y-->x
                    glTranslatef(0, nextD.x > 0 ? -2 * r : 2 * r, 0);
                    double torusAngle = nextD.x > 0 ? M_PI / 2 : -M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.y > 0 ? 0 : 2 * torusAngle, r1, r2, 2 * r, color, r0, tailRemainder, arc / (2 * r), headAngle, 2 * r);
                }
            }
        }
        glPopMatrix();
    }
}
static void eatline(std::istream& input)
{
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void load_bunny_data(const char file[], std::vector<GLfloat>& vertices, std::vector<GLuint>& faces, std::vector<GLfloat>& normals)
{
    ifstream fin{};
    fin.open(file);
    if (!fin.is_open()) {
        printf("Open `%s` failed\n", file);
        exit(1);
    }
    string token;
    float x, y, z;
    GLuint i1, i2, i3;
    while (!fin.eof()) {
        fin >> token;
        if (token == "#") {
            eatline(fin);
        }
        else if (token == "v") {
            fin >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            eatline(fin);
        }
        else if (token == "vn") {
            fin >> x >> y >> z;
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
            eatline(fin);
        }
        else if (token == "f") {
            fin >> i1; fin.get(); fin.get(); fin >> i1;
            fin >> i2; fin.get(); fin.get(); fin >> i2;
            fin >> i3; fin.get(); fin.get(); fin >> i3;
            faces.push_back(i1-1);
            faces.push_back(i2-1);
            faces.push_back(i3-1);
            eatline(fin);
        }
    }
}

#include <GL/glut.h>

float alpha = 0, beta = 0, delta = 1;
float angle = 0;

void timer(int value) {
    angle += 1;
    if (angle >= 360) {
        angle = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(40, timer, 1);

}
int init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluLookAt(0.0, 1.0, 0.0,
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0);
    glOrtho(-5.2, 5.2, -5.2, 5.2, -5.2, 5.2);
return 0;
}

void drawPyramid() {

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
}

void display(void) {
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(1.0, 0.0, 0.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

glPushMatrix();
glColor3f(1.0, 0.0, 1.0);
glTranslatef(alpha, 0.0, beta);
glutSolidCube(3.0f);
glPopMatrix();

glPushMatrix();
glColor3f(0.0, 1.0, 0.0);
glTranslatef(0.0, 0.0, 0.0);
glRotatef(angle, 0.0, 0.0, 1.0);
glTranslatef(0.0, 0.0, 0.0);
glutWireSphere(3, 20, 20);
glPopMatrix();


//drawPyramid();

glFlush();
}

void keyPressed_special(int key, int x, int y) {

    switch (key) {

    case GLUT_KEY_PAGE_UP:

        delta = delta * 1.1f;
        break;

    case GLUT_KEY_PAGE_DOWN:
        delta = delta * 0.99f;
        break;

    case GLUT_KEY_LEFT:
        beta += 0.1;
        break;

    case GLUT_KEY_RIGHT:
        beta -= 0.1;
        break;

    case GLUT_KEY_UP:
        alpha += 0.1;
        break;
    case GLUT_KEY_DOWN:
        alpha -= 0.1;
        break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("AA1");

    init();
    glutSpecialFunc(keyPressed_special);
    glutTimerFunc(40, timer, 1);
    glutDisplayFunc(display);
    glutMainLoop();
return 0;
}
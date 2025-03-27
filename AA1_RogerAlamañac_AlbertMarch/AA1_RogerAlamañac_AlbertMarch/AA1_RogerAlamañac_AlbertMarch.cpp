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
    // Proyecci�n
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.2, 5.2, -5.2, 5.2, -5.2, 5.2);

    // Vista camara
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 0.0,
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0);*/
    return 0;
}

void drawPyramid() {
    glColor3f(1.0f, 0.0f, 0.0f); // Color rojo

    glBegin(GL_LINES);
    // Lados de las caras triangulares
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Base cuadrada
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();
}

void drawViewportBorders() {
    // Restaurar viewport a toda la ventana
    glViewport(0, 0, 500, 500);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1, 1, 1);  // Blanco
    glLineWidth(2.0f);

    // Borde viewport izquierdo
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 125);
    glVertex2i(250, 125);
    glVertex2i(250, 375);
    glVertex2i(0, 375);
    glEnd();

    // Borde viewport derecho
    glBegin(GL_LINE_LOOP);
    glVertex2i(250, 125);
    glVertex2i(500, 125);
    glVertex2i(500, 375);
    glVertex2i(250, 375);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawScene() {
    glPushMatrix();
    glColor3f(1, 0, 1);
    glTranslatef(alpha, 0.0, beta);
    glutSolidCube(3.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 1, 0);
    glScalef(delta, delta, delta);
    glRotatef(angle, 0, 0, 1);
    glutWireSphere(3, 20, 20);
    glPopMatrix();

    drawPyramid();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    glViewport(0, 125, 250, 250);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 100.0); // Perspectiva cl�sica

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5.0, 5.0, 5.0,  // C�mara en diagonal
              0.0, 0.0, 0.0,  // Mira al centro
              0.0, 1.0, 0.0); // Arriba = eje Y

    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(alpha, 0.0, beta);
    glutWireCube(3.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glRotatef(angle, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 3.0);
    glScalef(delta, delta, delta);
    glutWireSphere(1.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glRotatef(angle, 0.0, 1.0, 0.0);
    drawPyramid();
    glPopMatrix();

    glPopMatrix();

    glViewport(250, 125, 250, 250);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5, 5, -5, 5, -10, 10);  // Volumen ortogr�fico

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0, 0.0, 0.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0); // Vista lateral desde el eje X

    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(0.0, alpha, beta);
    glutWireCube(3.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glRotatef(angle, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 3.0);
    glScalef(delta, delta, delta);
    glutWireSphere(1.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glRotatef(angle, 0.0, 1.0, 0.0);
    drawPyramid();
    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
    drawViewportBorders();
    glPopMatrix();

    glFlush();
}

void keyPressed_special(int key, int x, int y) {

    switch (key) {
    case GLUT_KEY_LEFT:
        beta += 0.1f;  // mueve en Z (vista lateral) o X (perspectiva)
        break;
    case GLUT_KEY_RIGHT:
        beta -= 0.1f;
        break;
    case GLUT_KEY_UP:
        alpha += 0.1f;  // mueve en Y (ambas vistas)
        break;
    case GLUT_KEY_DOWN:
        alpha -= 0.1f;
        break;
    }

    glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y) {
    switch (key) {
    case '+':
        delta *= 1.01f;
        break;
    case '-':
        delta *= 0.99f;
        break;
    case 'r':
        alpha = 0;
        beta = 0;
        delta = 1;
        angle = 0;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("AA1");

    init();
    glutSpecialFunc(keyPressed_special);
    glutKeyboardFunc(keyPressed);
    glutTimerFunc(40, timer, 1);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

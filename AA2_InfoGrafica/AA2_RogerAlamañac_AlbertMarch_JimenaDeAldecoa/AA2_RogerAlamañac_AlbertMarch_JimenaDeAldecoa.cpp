#include <GL/glut.h>
#include <cmath>

#define M_PI 3.14159265358979323846

float sunAngle = 0.0f;
float dayDuration = 30.0f;
float timeStep = 360.0f / (dayDuration * 60.0f); // graus per frame (60 fps)

void lighting() {
    float globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };
    float specular[] = { 1.0f, 1.0f, 0.9f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void updateLighting() {
    float radius = 10.0f;
    float x = radius * cosf(sunAngle);
    float y = radius * sinf(sunAngle);
    float pos[] = { x, y, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    float intensity = fmax(0.2f, y / radius);
    float diffuse[] = { intensity, intensity, intensity * 0.9f, 1.0f };
    float specular[] = { intensity, intensity, intensity * 0.9f, 1.0f };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void material(float r, float g, float b) {
    float ambient[] = { r * 0.2f, g * 0.2f, b * 0.2f, 1.0f };
    float diffuse[] = { r, g, b, 1.0f };
    float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
}

void scene() {
    // Terra
    material(0.3f, 0.6f, 0.3f);
    glPushMatrix();
    glScalef(10.0f, 0.1f, 10.0f);
    glutSolidCube(1);
    glPopMatrix();

    // Edifici
    material(0.6f, 0.6f, 0.8f);
    glPushMatrix();
    glTranslatef(-3.0f, 2.0f, -2.0f);
    glScalef(2.0f, 4.0f, 2.0f);
    glutSolidCube(1);
    glPopMatrix();

    // Arbre
    material(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(3.0f, 0.5f, 0.0f);
    glScalef(0.2f, 1.0f, 0.2f);
    glutSolidCube(1);
    glPopMatrix();

    material(0.0f, 0.6f, 0.0f);
    glPushMatrix();
    glTranslatef(3.0f, 1.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(0.6, 1.5, 20, 20);
    glPopMatrix();

    // Fanal
    material(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 2.0f);
    glScalef(0.3f, 0.2f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    material(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, 0.7f, 2.0f);
    glScalef(0.1f, 1.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    material(0.4f, 0.4f, 0.4f);
    glPushMatrix();
    glTranslatef(0.0f, 1.35f, 2.0f);
    glRotatef(180, 1, 0, 0);
    glutSolidCone(0.25, 0.3, 20, 20);
    glPopMatrix();

    material(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, 2.0f);
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(5, 5, 10, 0, 0, 0, 0, 1, 0);

    updateLighting();
    scene();

    glutSwapBuffers();
}

void idle() {
    sunAngle += timeStep * (M_PI / 180.0f);
    if (sunAngle > 2 * M_PI) sunAngle -= 2 * M_PI;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, aspect, 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

int init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    lighting();
    return 0;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sol Dinàmic");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}


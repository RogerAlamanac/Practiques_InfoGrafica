#include <GL/glut.h>
#include <ctime>
#include <cmath>
#define M_PI 3.14159265358979323846

float sunAngle = 0.0f; // Angle de rotació del "sol"
float dayDuration = 30.0f; // 30 segons = 24h
float timeStep = 360.0f / (dayDuration * 60.0f); // graus per frame (60 fps)

void initLighting() {
    // Llum ambiental global
    GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    // Activar il·luminació
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Propietats inicials de la llum (sol blanc)
    GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 0.9f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void updateLighting() { //Ajusta intensitat de la llum
    // Posició del sol (cercle sobre l'escena)
    float radius = 10.0f;
    float sunX = radius * cosf(sunAngle);
    float sunY = radius * sinf(sunAngle);
    GLfloat lightPos[] = { sunX, sunY, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Intensitat de llum segons l’altura del sol
    float intensity = fmax(0.2f, sunY / radius);
    GLfloat diffuse[] = { intensity, intensity, intensity * 0.9f, 1.0f };
    GLfloat specular[] = { intensity, intensity, intensity * 0.9f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void setMaterial(float r, float g, float b) {
    GLfloat ambient[] = { r * 0.2f, g * 0.2f, b * 0.2f, 1.0f };
    GLfloat diffuse[] = { r, g, b, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat shininess = 50.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();
    gluLookAt(5, 5, 10, 0, 0, 0, 0, 1, 0);

    updateLighting();

    // Terra
    setMaterial(0.3f, 0.6f, 0.3f);
    glPushMatrix();
    glScalef(10.0f, 0.1f, 10.0f);
    glutSolidCube(1);
    glPopMatrix();

    // Cub (edifici)
    setMaterial(0.5f, 0.5f, 0.8f);
    glPushMatrix();
    glTranslatef(-2.0f, 0.5f, -2.0f);
    glScalef(1.0f, 2.0f, 1.0f);
    glutSolidCube(1);
    glPopMatrix();

    // Con (arbre)
    setMaterial(0.2f, 0.7f, 0.2f);
    glPushMatrix();
    glTranslatef(2.0f, 1.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(0.5, 2.0, 20, 20);
    glPopMatrix();

    // Esfera (fanal)
    setMaterial(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 2.0f);
    glutSolidSphere(0.3f, 20, 20);
    glPopMatrix();

    glutSwapBuffers();
}

void idle() { //ACTUALITZA L'ANGLE DEL SOL DEPENENT DEL MOMENT DEL DIA (MOVIMENT DEL SOL)
    sunAngle += timeStep * (M_PI / 180.0f);
    if (sunAngle > 2 * M_PI) sunAngle -= 2 * M_PI;
    glutPostRedisplay();
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, aspect, 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("AA2");

    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glEnable(GL_NORMALIZE);

    glutMainLoop();
    return 0;
}

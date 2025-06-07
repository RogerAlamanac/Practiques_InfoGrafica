#include <GL/freeglut.h>
#include <cmath>

// Variables del jugador
float playerX = 0.0f, playerZ = 0.0f, playerAngle = 0.0f;
float moveForward = 0.0f, rotatePlayer = 0.0f;
float speed = 0.1f;

// Sol
float sunPosition = 0.0f;

// Linterna
bool flashlightActive = false;

// Estado de la puerta
enum DoorState { CLOSED, OPENING, OPEN, CLOSING };
DoorState doorState = CLOSED;
float doorRotation = 0.0f;
const float maxDoorRotation = 90.0f;
const float doorSpeed = 1.5f;
float doorPosX = 2.0f, doorPosZ = -5.0f;

// Objeto en movimiento
float movingObjectZ = 0.0f;
bool movingForward = true;

// Función que dibuja el sol
void renderSun() {
    float radius = 30.0f;
    float radian = sunPosition * 3.14159f / 180.0f;

    float sunX = radius * cos(radian);
    float sunY = radius * sin(radian);

    float ambientLight = fmax(0.0f, sin(radian));
    if (sunY > 0) {
        GLfloat lightPos[] = { sunX, sunY, 0.0f, 0.0f };
        GLfloat diffuse[] = { ambientLight, ambientLight, 0.9f * ambientLight, 1.0f };
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    }
    else {
        glDisable(GL_LIGHT0);
    }

    glClearColor(0.2f + 0.5f * ambientLight, 0.3f + 0.4f * ambientLight, 0.5f, 1.0f);

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(sunX, sunY, 0.0f);
    glutSolidSphere(1.0f, 16, 16);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// Linterna
void updateFlashlight() {
    if (!flashlightActive) {
        glDisable(GL_LIGHT1);
        return;
    }

    if (sunPosition <= 180.0f && flashlightActive) flashlightActive = false;

    float dirX = sin(playerAngle);
    float dirZ = -cos(playerAngle);

    GLfloat pos[] = { playerX, 1.2f, playerZ, 1.0f };
    GLfloat spotDir[] = { dirX, -0.2f, dirZ };
    GLfloat diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0f);
}

// Casa alrededor de la puerta
void drawHouse() {
    glColor3f(0.5f, 0.3f, 0.2f);

    // Pared trasera
    glPushMatrix();
    glTranslatef(doorPosX, 1.0f, doorPosZ - 1.0f);
    glScalef(4.0f, 2.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pared lateral izquierda larga
    glPushMatrix();
    glTranslatef(doorPosX - 2.0f, 1.0f, doorPosZ);
    glScalef(0.2f, 2.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pared lateral derecha larga
    glPushMatrix();
    glTranslatef(doorPosX + 2.0f, 1.0f, doorPosZ);
    glScalef(0.2f, 2.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pared frontal izquierda (desde la pared lateral izquierda hasta la puerta)
    glPushMatrix();
    glTranslatef(doorPosX - 1.25f, 1.0f, doorPosZ + 1.0f);
    glScalef(1.5f, 2.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pared frontal derecha (desde la puerta hasta la pared lateral derecha)
    glPushMatrix();
    glTranslatef(doorPosX + 1.25f, 1.0f, doorPosZ + 1.0f);
    glScalef(1.5f, 2.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
}


// Escenario
void drawWorld() {
    // Suelo
    glColor3f(0.3f, 0.7f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-20, 0, -20);
    glVertex3f(-20, 0, 20);
    glVertex3f(20, 0, 20);
    glVertex3f(20, 0, -20);
    glEnd();

    // Dibuja la casa
    drawHouse();

    // Puerta interactiva (con rotación ajustada)
    glPushMatrix();
    glTranslatef(doorPosX, 1.0f, doorPosZ + 1.0f); // posición global de la puerta
    glTranslatef(-0.5f, 0.0f, 0.0f);               // trasladar origen al borde izquierdo
    glRotatef(doorRotation, 0.0f, 1.0f, 0.0f);     // rotar la puerta
    glTranslatef(0.5f, 0.0f, 0.0f);                // devolver el cubo a su posición
    glScalef(1.0f, 2.0f, 0.2f);
    glColor3f(1.0f, 0.9f, 0.7f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Objeto en movimiento
    glPushMatrix();
    glTranslatef(5.0f, 0.5f, movingObjectZ);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Minimapa
void drawMinimap() {
    glViewport(0, 500, 200, 200);  // esquina superior izquierda
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20, 20, -20, 20, -1, 50);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 30, 0, 0, 0, 0, 0, 0, -1);

    glDisable(GL_LIGHTING);
    drawWorld();

    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(playerX, 0.1f, playerZ);
    glutSolidSphere(0.5f, 8, 8);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// Display
void display() {
    glViewport(0, 0, 700, 700);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float dirX = sin(playerAngle);
    float dirZ = -cos(playerAngle);
    gluLookAt(playerX, 1.0f, playerZ, playerX + dirX, 1.0f, playerZ + dirZ, 0.0f, 1.0f, 0.0f);

    renderSun();
    updateFlashlight();
    drawWorld();
    drawMinimap();

    glutSwapBuffers();
}

// Timer
void update(int value) {
    if (moveForward != 0) {
        playerX += sin(playerAngle) * moveForward * speed;
        playerZ += -cos(playerAngle) * moveForward * speed;
    }
    if (rotatePlayer != 0) playerAngle += rotatePlayer * 0.05f;

    // Puerta
    if (doorState == OPENING) {
        doorRotation += doorSpeed;
        if (doorRotation >= maxDoorRotation) {
            doorRotation = maxDoorRotation;
            doorState = OPEN;
        }
    }
    else if (doorState == CLOSING) {
        doorRotation -= doorSpeed;
        if (doorRotation <= 0.0f) {
            doorRotation = 0.0f;
            doorState = CLOSED;
        }
    }

    // Objeto en movimiento
    if (movingForward) {
        movingObjectZ += 0.05f;
        if (movingObjectZ > 8.0f) movingForward = false;
    }
    else {
        movingObjectZ -= 0.05f;
        if (movingObjectZ < -8.0f) movingForward = true;
    }

    sunPosition += 0.1f;
    if (sunPosition > 360.0f) sunPosition -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Controles
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': moveForward = 1; break;
    case 's': moveForward = -1; break;
    case 'a': rotatePlayer = -1; break;
    case 'd': rotatePlayer = 1; break;
    case 'e': {
        float dx = playerX - doorPosX;
        float dz = playerZ - (doorPosZ + 1.0f);
        float distance = sqrt(dx * dx + dz * dz);
        if (distance < 3.0f) {
            if (doorState == CLOSED) doorState = OPENING;
            else if (doorState == OPEN) doorState = CLOSING;
        }
        break;
    }
    case 'f':
        if (sunPosition > 180.0f) flashlightActive = !flashlightActive;
        break;
    }
}

void keyUp(unsigned char key, int x, int y) {
    if (key == 'w' || key == 's') moveForward = 0;
    if (key == 'a' || key == 'd') rotatePlayer = 0;
}

// Init
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Joc d'Exploració");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyUp);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}

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
float movingObjectX = 22.0f;  // empieza a la derecha

// Función que dibuja el sol
void drawSunLight() {
    const float orbitRadius = 40.0f;
    const float angleRad = sunPosition *3.14159f / 180.0f;

    const float posX = orbitRadius * cos(angleRad);
    const float posY = orbitRadius * sin(angleRad);

    float daylightIntensity = fmax(0.0f, sin(angleRad));
    if (posY > 0) {
        GLfloat lightPosition[] = { posX, posY, 0.0f, 0.0f };
        GLfloat diffuseColor[] = { daylightIntensity, daylightIntensity, 0.8f * daylightIntensity, 1.0f };
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
    }
    else {
        glDisable(GL_LIGHT0);
    }

    glClearColor(0.25f + 0.4f * daylightIntensity, 0.35f + 0.5f * daylightIntensity, 0.5f, 1.0f);

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(posX, posY, 0.0f);
    glutSolidSphere(1.0f, 16, 16);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// Linterna
void handleFlashlight() {
    if (!flashlightActive) {
        glDisable(GL_LIGHT1);
        return;
    }

    if (sunPosition < 180.0f && flashlightActive) {
        flashlightActive = false;
    }

    float facingX = sin(playerAngle);
    float facingZ = -cos(playerAngle);

    GLfloat position[] = { playerX, 1.2f, playerZ, 1.0f };
    GLfloat direction[] = { facingX, -0.25f, facingZ };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 0.85f, 1.0f };

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0f);
}

// Casa alrededor de la puerta
void drawHouse() {
    glColor3f(0.6f, 0.6f, 0.6f);

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

    // Pared frontal izquierda
    glPushMatrix();
    glTranslatef(doorPosX - 1.25f, 1.0f, doorPosZ + 1.0f);
    glScalef(1.5f, 2.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pared frontal derecha
    glPushMatrix();
    glTranslatef(doorPosX + 1.25f, 1.0f, doorPosZ + 1.0f);
    glScalef(1.5f, 2.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Techo
    glPushMatrix();
    glTranslatef(doorPosX, 2.0f, doorPosZ);
    glScalef(4.0f, 0.1f, 2.0f);
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

    // Carretera
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-22.0f, 0.01f, 3.5f);
    glVertex3f(22.0f, 0.01f, 3.5f);
    glVertex3f(22.0f, 0.01f, 6.5f);
    glVertex3f(-22.0f, 0.01f, 6.5f);
    glEnd();

    // Dibuja la casa
    drawHouse();

    // Puerta interactiva
    glPushMatrix();
    glTranslatef(doorPosX, 1.0f, doorPosZ + 1.0f);
    glTranslatef(-0.5f, 0.0f, 0.0f);
    glRotatef(doorRotation, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.5f, 0.0f, 0.0f);
    glScalef(1.0f, 2.0f, 0.2f);
    glColor3f(0.7f, 0.4f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Objeto en movimiento
    glPushMatrix();
    glTranslatef(movingObjectX, 0.5f, 5.0f);

    // Cubo delantero
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.6f, 0.0f, 0.0f);
    glScalef(0.6f, 0.3f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cubo central
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 0.0f);
    glScalef(0.6f, 0.7f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cubo trasero
    glPushMatrix();
    glTranslatef(0.6f, 0.1f, 0.0f);
    glScalef(0.6f, 0.6f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cabina
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.6f, 0.3f, 0.0f);
    glScalef(0.6f, 0.4f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Ruedas
    glColor3f(0.1f, 0.1f, 0.1f);
    float wheelYOffset = -0.25f;

    glPushMatrix();
    glTranslatef(-0.6f, wheelYOffset, 0.5f);
    glScalef(0.3f, 0.3f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.6f, wheelYOffset, -0.5f);
    glScalef(0.3f, 0.3f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6f, wheelYOffset, 0.5f);
    glScalef(0.3f, 0.3f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6f, wheelYOffset, -0.5f);
    glScalef(0.3f, 0.3f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Alerón
    glColor3f(0.7f, 0.7f, 0.0f);
    glPushMatrix();
    glTranslatef(1.0f, 0.4f, 0.0f);
    glScalef(0.2f, 0.1f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

// Minimapa
void renderMinimap() {
    glViewport(0, 500, 200, 200);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20, 20, -20, 20, -1, 50);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 30.0f, 0, 0, 0, 0, 0, 0, -1);

    glDisable(GL_LIGHTING);
    drawWorld();

    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(playerX, 0.2f, playerZ);
    glutSolidSphere(0.4f, 8, 8);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

// Display
void display() {
    glViewport(0, 0, 700, 700);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = sin(playerAngle);
    float camZ = -cos(playerAngle);
    gluLookAt(playerX, 1.2f, playerZ, playerX + camX, 1.2f, playerZ + camZ, 0.0f, 1.0f, 0.0f);

    drawSunLight();
    handleFlashlight();
    drawWorld();
    renderMinimap();

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
    movingObjectX -= 0.05f;
    if (movingObjectX < -22.0f) {
        movingObjectX = 22.0f;
    }

    sunPosition += 0.1f;
    if (sunPosition > 360.0f) sunPosition -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Controles
void handleKeyPress(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': moveForward = 1; break; //Moverse hacia delante
    case 's': moveForward = -1; break; //Moverse hacia atrás
    case 'a': rotatePlayer = -1; break; //Mirar a la izquierda
    case 'd': rotatePlayer = 1; break; //Mirar a la derecha
    case 'e': {
        float distX = playerX - doorPosX;
        float distZ = playerZ - (doorPosZ + 1.0f);
        if (sqrt(distX * distX + distZ * distZ) < 3.5f) { //Abrir puerta
            if (doorState == CLOSED) doorState = OPENING;
            else if (doorState == OPEN) doorState = CLOSING;
        }
        break;
    }
    case 'f': //Activar linterna
        if (sunPosition > 180.0f) flashlightActive = !flashlightActive;
        break;
    }
}

void handleKeyRelease(unsigned char key, int x, int y) {
    if (key == 'w' || key == 's') moveForward = 0;
    if (key == 'a' || key == 'd') rotatePlayer = 0;
}

// Init
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat globalAmbient[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Explorador 3D");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyPress);
    glutKeyboardUpFunc(handleKeyRelease);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}

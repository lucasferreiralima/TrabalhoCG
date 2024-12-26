#include <GL/glut.h>
#include <iostream>
#include <math.h>

#define PI 3.141592654

using namespace std;

// Variáveis globais para controle do robô
float angleBody = 0.0f;       // Rotação do corpo
float angleLeftArm = 45.0f;   // Ângulo do braço esquerdo
float angleRightArm = -45.0f; // Ângulo do braço direito
float angleClampLeftZ = 30.0f; // Abertura da garra esquerda no eixo Z
float angleClampLeftY = 30.0f; // Abertura da garra esquerda no eixo Y
float angleClampRightZ = 30.0f; // Abertura da garra direita no eixo Z
float angleClampRightY = 30.0f; // Abertura da garra direita no eixo Y
float angleHead = 0.0f;       // Rotação da cabeça
float zoom = 1.0f;
float viewAngleX = 0.0f;      // Rotação da câmera no eixo X
float viewAngleY = 0.0f;      // Rotação da câmera no eixo Y

// Função de controle por teclado
void RoboHandleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // Tecla ESC
        exit(0);
    case 'a': // Girar corpo para esquerda
        angleBody -= 5.0f;
        break;
    case 'd': // Girar corpo para direita
        angleBody += 5.0f;
        break;
    case 'q': // Levantar braço esquerdo
        angleLeftArm += 5.0f;
        break;
    case 'w': // Abaixar braço esquerdo
        angleLeftArm -= 5.0f;
        break;
    case 'e': // Levantar braço direito
        angleRightArm += 5.0f;
        break;
    case 'r': // Abaixar braço direito
        angleRightArm -= 5.0f;
        break;
    case 'o': // Abrir garra esquerda no eixo Z
        angleClampLeftZ += 5.0f;
        break;
    case 'p': // Fechar garra esquerda no eixo Z
        angleClampLeftZ -= 5.0f;
        break;
    case 'u': // Abrir garra direita no eixo Z
        angleClampRightZ += 5.0f;
        break;
    case 'i': // Fechar garra direita no eixo Z
        angleClampRightZ -= 5.0f;
        break;
    case 'z': // Rotação da cabeça para a esquerda
        angleHead -= 5.0f;
        break;
    case 'x': // Rotação da cabeça para a direita
        angleHead += 5.0f;
        break;
    case '+': // Zoom in
        zoom += 0.1f;
        break;
    case '-': // Zoom out
        zoom -= 0.1f;
        break;
    }
    glutPostRedisplay();
}

// Configuração da matriz de projeção
void RoboResize(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);
}

// Função para desenhar uma garra
void RoboDrawClamp(float angleClampZ, float angleClampY) {
    glPushMatrix();

    // Parte superior da garra
    glRotatef(angleClampY + 60, 0.0f, 1.0f, 0.0f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidCone(0.3, 1.0, 20, 20);
    glTranslatef(0.0f, 0.0f, 1.0);
    glRotatef(-60, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.3, 1.0, 20, 20);

    glPopMatrix();
    glPushMatrix();

    // Parte inferior da garra
    glRotatef(-angleClampY - 60, 0.0f, 1.0f, 0.0f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidCone(0.3, 1.0, 20, 20);
    glTranslatef(0.0f, 0.0f, 1.0);
    glRotatef(60, 0.0f, 1.0f, 0.0f);
    glutSolidCone(0.3, 1.0, 20, 20);

    glPopMatrix();
}

// Função para desenhar a cabeça
void RoboDrawHead() {
    glPushMatrix();
    glTranslatef(0.0f, 3.5f, 0.0f);
    glRotatef(angleHead, 0.0f, 1.0f, 0.0f);

    // Cabeça principal
    glColor3f(0.7f, 0.7f, 0.7f);
    glutSolidSphere(1.0f, 20, 20);

    // Olhos
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.6f, 0.2f, 0.8f);
    glutSolidSphere(0.2f, 20, 20);
    glTranslatef(-1.2f, 0.0f, 0.0f);
    glutSolidSphere(0.2f, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

// Função para desenhar um braço com uma garra
void RoboDrawArm(float angleArm, float angleClampZ, float angleClampY) {
    glPushMatrix();
    glRotatef(angleArm, 1.0f, 0.0f, 0.0f);

    // Braço
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUquadric* quadric = gluNewQuadric();
    gluCylinder(quadric, 0.2, 0.2, 2.0, 20, 20);
    glTranslatef(0.0f, 0.0f, 2.0);

    // Garra
    RoboDrawClamp(angleClampZ, angleClampY);

    glPopMatrix();
    gluDeleteQuadric(quadric);
}

// Função para desenhar o corpo
void RoboDrawBody() {
    glPushMatrix();
    glRotatef(angleBody, 0.0f, 1.0f, 0.0f);

    // Corpo
    glColor3f(0.5f, 0.5f, 0.0f);
    glScalef(2.0f, 3.0f, 1.0f);
    glutSolidCube(1.0f);

    // Braços conectados ao corpo
    glPushMatrix();
    glTranslatef(-1.0f, 1.0f, 0.0f);
    RoboDrawArm(angleLeftArm, angleClampLeftZ, angleClampLeftY);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, 1.0f, 0.0f);
    RoboDrawArm(angleRightArm, angleClampRightZ, angleClampRightY);
    glPopMatrix();

    glPopMatrix();
}

// Função para desenhar as pernas e a esteira
void RoboDrawLegsAndTracks() {
    glPushMatrix();

    // Pernas
    glTranslatef(0.0f, -1.5f, 0.0f);
    glColor3f(0.4f, 0.4f, 0.4f);
    glScalef(1.5f, 0.5f, 1.0f);
    glutSolidCube(1.0f);

    // Esteira
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(-1.5f, 0.0f, -1.0f);
    glVertex3f(1.5f, 0.0f, -1.0f);
    glVertex3f(1.5f, 0.0f, 1.0f);
    glVertex3f(-1.5f, 0.0f, 1.0f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

// Função principal de desenho
void RoboDrawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 5.0, 15.0, 0.0, 1.5, 0.0, 0.0, 1.0, 0.0);

    glScalef(zoom, zoom, zoom);
    glRotatef(viewAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(viewAngleY, 0.0f, 1.0f, 0.0f);

    RoboDrawLegsAndTracks();
    RoboDrawBody();
    RoboDrawHead();

    glutSwapBuffers();

}

// Inicialização de configurações do OpenGL
void RoboInitRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Robô Inspirado em Perdidos no Espaço");
    RoboInitRendering();

    glutDisplayFunc(RoboDrawScene);
    glutKeyboardFunc(RoboHandleKeypress);
    glutReshapeFunc(RoboResize);

    glutMainLoop();
    return 0;
}

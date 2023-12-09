#include <GL/glu.h>
#include "gl/glut.h"
#include <cmath>

// Функция, представляющая поверхность второго порядка
float surfaceFunction(float x, float y) {
    return x * x * x + y * y * y;
}

// Переменная состояния для включения/выключения триангуляции
bool enableTriangulation = true;

// Углы вращения камеры
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;

// Функция для рисования поверхности
void drawSurface() {
    const int numDivisions = 50;
    const float stepSize = 2.0 / numDivisions;

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < numDivisions; ++i) {
        for (int j = 0; j < numDivisions; ++j) {
            float x = -1.0 + i * stepSize;
            float y = -1.0 + j * stepSize;

            // Триангуляция прямоугольника
            glVertex3f(x, y, surfaceFunction(x, y));
            glVertex3f(x + stepSize, y, surfaceFunction(x + stepSize, y));
            glVertex3f(x, y + stepSize, surfaceFunction(x, y + stepSize));

            glVertex3f(x + stepSize, y, surfaceFunction(x + stepSize, y));
            glVertex3f(x + stepSize, y + stepSize, surfaceFunction(x + stepSize, y + stepSize));
            glVertex3f(x, y + stepSize, surfaceFunction(x, y + stepSize));
        }
    }

    glEnd();
}

// Функция для рисования контура поверхности
void drawSurfaceContour() {
    const int numDivisions = 50;
    const float stepSize = 2.0 / numDivisions;

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Режим рисования контура

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < numDivisions; ++i) {
        for (int j = 0; j < numDivisions; ++j) {
            float x = -1.0 + i * stepSize;
            float y = -1.0 + j * stepSize;

            // Триангуляция прямоугольника
            glVertex3f(x, y, surfaceFunction(x, y));
            glVertex3f(x + stepSize, y, surfaceFunction(x + stepSize, y));
            glVertex3f(x, y + stepSize, surfaceFunction(x, y + stepSize));

            glVertex3f(x + stepSize, y, surfaceFunction(x + stepSize, y));
            glVertex3f(x + stepSize, y + stepSize, surfaceFunction(x + stepSize, y + stepSize));
            glVertex3f(x, y + stepSize, surfaceFunction(x, y + stepSize));
        }
    }

    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Восстанавливаем обычный режим рисования
}

// Функция отрисовки сцены
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightDirection[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);

    GLfloat materialColor[] = {0.8, 0.8, 0.8, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5.0 * sin(cameraAngleX), 5.0 * sin(cameraAngleY), 5.0 * cos(cameraAngleX),
              0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    drawSurface();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    if (enableTriangulation) {
        drawSurfaceContour(); // Рисуем контур поверхности
    }

    glutSwapBuffers();
}

// Функция изменения размеров окна
void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
}

// Функция обработки клавиш
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 't':
            enableTriangulation = !enableTriangulation;
            glutPostRedisplay();
            break;
        case 'a':
            cameraAngleX += 0.1f;
            glutPostRedisplay();
            break;
        case 'd':
            cameraAngleX -= 0.1f;
            glutPostRedisplay();
            break;
        case 'w':
            cameraAngleY += 0.1f;
            glutPostRedisplay();
            break;
        case 's':
            cameraAngleY -= 0.1f;
            glutPostRedisplay();
            break;
        case 27: // ESC key
            exit(0);
            break;
    }
}

// Точка входа
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Triangulated Surface with Contour");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}

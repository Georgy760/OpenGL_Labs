#include <GL/glut.h>
#include <cmath>

// Угол вращения камеры
float cameraAngle = 0.0f;

// Параметры призмы
float baseVertices[][2] = {
        {3.0f, 0.0f},
        {0.0f, 3.0f},
        {-3.0f, 0.0f},
        {-1.0f, -3.0f},
        {1.0f, -3.0f}
};
float prismHeight = 5.0f;

// Параметры направленного источника света
GLfloat lightDirection[] = {1.0, 1.0, -1.0, 0.0}; // Направление света

// Функция для инициализации OpenGL
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Цвет фона: черный
    glEnable(GL_DEPTH_TEST); // Включаем буфер глубины
    glEnable(GL_LIGHTING); // Включаем освещение
    glEnable(GL_LIGHT0); // Включаем источник света 0

    // Направление источника света (параллельно вектору {1, 1, -1})
    GLfloat lightDirection[] = {1.0, 1.0, -1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0); // Настройка перспективной проекции
}

// Функция для отрисовки призмы
void drawPrism() {
    glBegin(GL_QUAD_STRIP);

    for (int i = 0; i < 5; ++i) {
        // Вычисляем нормаль для каждой вершины
        float x0 = baseVertices[i][0];
        float y0 = baseVertices[i][1];
        float x1 = baseVertices[(i + 1) % 5][0];
        float y1 = baseVertices[(i + 1) % 5][1];
        float normalX = -(y1 - y0);
        float normalY = x1 - x0;
        float normalLength = sqrt(normalX * normalX + normalY * normalY);
        normalX /= normalLength;
        normalY /= normalLength;

        // Устанавливаем нормаль
        glNormal3f(normalX, normalY, 0.0);

        // Устанавливаем цвет вершины
        glColor3f(static_cast<float>(i) / 4.0f, 0.0f, 1.0f - static_cast<float>(i) / 4.0f);

        // Вершина на первом основании
        glVertex3f(x0, y0, 0.0);
        // Вершина на втором основании (с учетом трансляции)
        glVertex3f(x0 + 1.0f, y0 + 1.0f, prismHeight);
    }

    // Замыкаем призму
    glVertex3f(baseVertices[0][0], baseVertices[0][1], 0.0);
    glVertex3f(baseVertices[0][0] + 1.0f, baseVertices[0][1] + 1.0f, prismHeight);

    glEnd();

    // Рисуем основания призмы
    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; ++i) {
        glNormal3f(0.0, 0.0, -1.0); // Нормаль направлена внутрь призмы
        glColor3f(static_cast<float>(i) / 4.0f, 0.0f, 1.0f - static_cast<float>(i) / 4.0f);
        glVertex3f(baseVertices[i][0], baseVertices[i][1], 0.0);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; ++i) {
        glNormal3f(0.0, 0.0, 1.0); // Нормаль направлена наружу призмы
        glColor3f(static_cast<float>(i) / 4.0f, 0.0f, 1.0f - static_cast<float>(i) / 4.0f);
        glVertex3f(baseVertices[i][0] + 1.0f, baseVertices[i][1] + 1.0f, prismHeight);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Включаем цвет вершин
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Рисуем призму
    drawPrism();

    // Отключаем цвет вершин
    glDisable(GL_COLOR_MATERIAL);

    // Позиция камеры
    float cameraX = 20.0f * sin(cameraAngle);
    float cameraY = 20.0f * cos(cameraAngle);

    // Устанавливаем камеру
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, 20.0f, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Colored Prism");

    init();

    glutDisplayFunc(display);

    glutIdleFunc([]() {
        // Вращаем камеру
        cameraAngle += 0.01f;
        if (cameraAngle > 2 * M_PI) { //3.14159265f
            cameraAngle -= 2 * M_PI;
        }
        glutPostRedisplay();
    });

    glutMainLoop();

    return 0;
}

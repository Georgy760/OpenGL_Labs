#include <GL/glut.h>
#include <cmath>

// Параметры для движения камеры и источника света
float t = 0.0f;      // Параметр для траектории
float dt = 0.01f;   // Скорость изменения параметра
float cameraX, cameraY, lightX, lightY; // Позиции камеры и источника света

float rotationAngle = 0.0f;

// Параметры призмы
float baseVertices[][2] = {
        {3.0f, 0.0f},
        {0.0f, 3.0f},
        {-3.0f, 0.0f},
        {-1.0f, -3.0f},
        {1.0f, -3.0f}
};
float prismHeight = 5.0f;

// Функция для расчета позиций на основе уравнения
void calculatePositions() {
    // Рассчитываем позиции на основе уравнения x^2 - 2xy + 5y^2 - 4 = 0
    float a = 2.0f;
    float b = sqrt(5.0 - 2.0 * t);
    float x = a * cos(t * M_PI * 2.0);
    float y = b * sin(t * M_PI * 2.0);

    // Обновляем позиции камеры и источника света
    cameraX = x;
    cameraY = y;
    lightX = x;
    lightY = y;
}

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
void calculateRotationMatrix(float angle, float (&rotationMatrix)[2][2]) {
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    rotationMatrix[0][0] = cosTheta;
    rotationMatrix[0][1] = -sinTheta;
    rotationMatrix[1][0] = sinTheta;
    rotationMatrix[1][1] = cosTheta;
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

        rotationAngle = 1.5708f;

        // Вычисляем матрицу поворота
        float rotationMatrix[16];
        glLoadIdentity();
        glRotatef(rotationAngle * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
        glGetFloatv(GL_MODELVIEW_MATRIX, rotationMatrix);

        // Применяем матрицу поворота к объекту
        glMultMatrixf(rotationMatrix);

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

// Функция для обновления сцены
void updateScene() {
    // Обновляем параметр t и пересчитываем позиции на каждом кадре
    t += dt;
    if (t > 1.0f) {
        t = 0.0f;  // Сбрасываем параметр, когда он достигает конца
    }
    calculatePositions();
}

// Функция для отображения сцены
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Обновляем сцену с новыми позициями
    updateScene();

    // Включаем цвет вершин
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Рисуем призму
    drawPrism();

    // Отключаем цвет вершин
    glDisable(GL_COLOR_MATERIAL);

    // Устанавливаем позицию камеры
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
        glutPostRedisplay();
    });

    glutMainLoop();

    return 0;
}

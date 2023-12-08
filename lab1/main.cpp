#include <GL/glut.h>
#include <cmath>

// Угол вращения источника света
float lightAngle = 0.0f;

// Радиус вращения камеры
float cameraRadius = 20.0f;

// Угол вращения камеры
float cameraAngle = 0.0f;

// Функция для инициализации OpenGL
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Цвет фона: черный
    glEnable(GL_DEPTH_TEST); // Включаем буфер глубины
    glEnable(GL_LIGHTING); // Включаем освещение
    glEnable(GL_LIGHT0); // Включаем источник света 0

    // Направление источника света (параллельно вектору {1, 1, 1})
    GLfloat lightDirection[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0); // Настройка перспективной проекции
}

// Функция для отрисовки пятиугольника
void drawPolygon() {
    glBegin(GL_POLYGON);

    // Первая вершина (3, 0)
    glNormal3f(0.0, 0.0, 1.0); // Нормаль направлена наружу
    glColor3f(1.0, 0.0, 0.0); // Красный цвет материала
    glVertex3f(3.0, 0.0, 0.0);

    // Вторая вершина (0, 3)
    glNormal3f(0.0, 0.0, 1.0); // Нормаль также направлена наружу
    glColor3f(0.0, 1.0, 0.0); // Зеленый цвет материала
    glVertex3f(0.0, 3.0, 0.0);

    // Остальные вершины аналогично с указанием цвета и нормали
    // (-3, 0)
    glNormal3f(0.0, 0.0, 1.0);
    glColor3f(0.0, 0.0, 1.0); // Синий цвет материала
    glVertex3f(-3.0, 0.0, 0.0);

    // (-1, -3)
    glNormal3f(0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 0.0); // Желтый цвет материала
    glVertex3f(-1.0, -3.0, 0.0);

    // (1, -3)
    glNormal3f(0.0, 0.0, 1.0);
    glColor3f(1.0, 0.0, 1.0); // Фиолетовый цвет материала
    glVertex3f(1.0, -3.0, 0.0);

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Включаем цвет вершин
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Рисуем пятиугольник
    drawPolygon();

    // Отключаем цвет вершин
    glDisable(GL_COLOR_MATERIAL);

    // Позиция камеры
    float cameraX = cameraRadius * sin(cameraAngle);
    float cameraY = cameraRadius * cos(cameraAngle);

    // Устанавливаем камеру
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0); // Камера

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rotating Camera");

    init();

    glutDisplayFunc(display);

    glutIdleFunc([]() {
        // Вращаем камеру
        cameraAngle += 0.01f;
        if (cameraAngle > 2 * 3.14159265f) {
            cameraAngle -= 2 * 3.14159265f;
        }
        glutPostRedisplay();
    });

    glutMainLoop();

    return 0;
}

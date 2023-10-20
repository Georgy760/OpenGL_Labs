#include <GL/glut.h>
#include <IL/il.h>
#include <cmath>

GLuint textureID; // Идентификатор текстуры

// Угол вращения камеры
float cameraAngle = 0.0f;

// Угол вращения для 2х фигур
float rotationAngle_1 = 0.0f;
float rotationAngle_2 = 0.0f;


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

// Функция для загрузки текстуры
void loadTexture() {
    // Инициализация DevIL
    ilInit();


    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    ILboolean success = ilLoadImage("/home/axelsingleton/Pictures/ava (copy).jpg");
    if (success) {
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (!success) {
            // Обработка ошибки конвертации
            ILenum error = ilGetError();
            printf("DevIL Error: %s\n",  ilGetString(error));
            ilDeleteImages(1, &imageID);
            return;
        }
    } else {
        // Обработка ошибки загрузки изображения
        ILenum error = ilGetError();
        printf("DevIL Error: %s\n", ilGetString(error));
        ilDeleteImages(1, &imageID);
        return;
    }

    // Генерация и привязка текстуры OpenGL
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Установка параметров фильтрации
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Загрузка данных изображения в текстуру
    glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
                 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

    // Освобождение памяти DevIL и разбинд текстуры
    ilDeleteImages(1, &imageID);
    glBindTexture(GL_TEXTURE_2D, 0);
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

    loadTexture(); // Загрузка текстуры
}

// Функция для отрисовки призмы
void drawPrism() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

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

        glBegin(GL_QUADS);
        // Отрисовка каждой грани
        glTexCoord2f(0.0, i * 0.2);
        glVertex3f(x0, y0, 0.0);

        glTexCoord2f(1.0, i * 0.2);
        glVertex3f(x0 + 1.0f, y0 + 1.0f, prismHeight);

        glTexCoord2f(1.0, (i + 1) * 0.2);
        glVertex3f(x1 + 1.0f, y1 + 1.0f, prismHeight);

        glTexCoord2f(0.0, (i + 1) * 0.2);
        glVertex3f(x1, y1, 0.0);

        glEnd();
    }

    glDisable(GL_TEXTURE_2D);

    // Рисуем основания призмы
    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; ++i) {
        glNormal3f(0.0, 0.0, -1.0); // Нормаль направлена внутрь призмы
        glVertex3f(baseVertices[i][0], baseVertices[i][1], 0.0);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; ++i) {
        glNormal3f(0.0, 0.0, 1.0); // Нормаль направлена наружу призмы
        glVertex3f(baseVertices[i][0] + 1.0f, baseVertices[i][1] + 1.0f, prismHeight);
    }
    glEnd();
}


// Функция для вращения фигур
void rotate(float rotationAngle, int polar) {
    //if(polar < 0) rotationAngle * -1;
    rotationAngle += 2.0f; //
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
    glutPostRedisplay(); // Запрос на перерисовку
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Вращаем фигуры
    rotate(rotationAngle_1, -1);
    rotate(rotationAngle_2, 1);
    // Включаем цвет вершин
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Позиция камеры
    float cameraX = 20.0f * sin(cameraAngle);
    float cameraY = 20.0f * cos(cameraAngle);

    // Устанавливаем камеру
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, 20.0f, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    // Отрисовка фигур
    glPushMatrix();
    glRotatef(rotationAngle_1, 0.0f, 0.0f, 1.0f); // Вращение вокруг Z-оси
    glTranslatef(0.0, 0.0, 0.0); // Первая призма в начале координат
    drawPrism();
    glPopMatrix();

    glPushMatrix();
    glRotatef(rotationAngle_2, 0.0f, 0.0f, 1.0f); // Вращение вокруг Z-оси
    glTranslatef(3.0, 3.0, 0.0); // Перемещаем вторую призму
    drawPrism(); // Можете использовать другую функцию для второй фигуры
    glPopMatrix();

    // Отключаем цвет вершин
    glDisable(GL_COLOR_MATERIAL);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Textured Prism"); // Изменим название окна

    init();

    glutDisplayFunc(display);

    glutIdleFunc([]() {
        // Вращаем камеру
        cameraAngle += 0.01f;
        if (cameraAngle > 2 * M_PI) {
            cameraAngle -= 2 * M_PI;
        }
        glutPostRedisplay();
    });

    glutMainLoop();

    return 0;
}

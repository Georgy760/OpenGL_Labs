#include <GL/glut.h>
#include <IL/il.h>
#include <cmath>

GLuint textureID; // Texture identifier

// Camera angles and position
float cameraAngle = 0.0f;
float cameraX = 0.0f;
float cameraY = 0.0f;

// Octahedron parameters
GLfloat octahedronVertices[][3] = {
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f}
};
GLubyte octahedronIndices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        5, 2, 1,
        5, 3, 2,
        5, 4, 3,
        5, 1, 4
};
float octahedronColors[][3] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 1.0f}
};

// Light source parameters
GLfloat lightDirection[] = {1.0, 1.0, -1.0, 0.0}; // Light direction

// Function to load a texture
void loadTexture() {
    // Initialize DevIL
    ilInit();

    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    ILboolean success = ilLoadImage("/home/axelsingleton/Pictures/ava (copy).jpg");
    if (success) {
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (!success) {
            // Handle conversion error
            ILenum error = ilGetError();
            printf("DevIL Error: %s\n",  ilGetString(error));
            ilDeleteImages(1, &imageID);
            return;
        }
    } else {
        // Handle image loading error
        ILenum error = ilGetError();
        printf("DevIL Error: %s\n", ilGetString(error));
        ilDeleteImages(1, &imageID);
        return;
    }

    // Generate and bind an OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image data into the texture
    glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
                 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

    // Free DevIL memory and unbind the texture
    ilDeleteImages(1, &imageID);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Function to initialize OpenGL
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Background color: black
    glEnable(GL_DEPTH_TEST); // Enable depth buffer
    glEnable(GL_LIGHTING); // Enable lighting
    glEnable(GL_LIGHT0); // Enable light source 0

    // Light direction (parallel to {1, 1, -1})
    GLfloat lightDirection[] = {1.0, 1.0, -1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0); // Set up perspective projection

    loadTexture(); // Load the texture
}

// Function to draw the octahedron
void drawOctahedron() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    for (int i = 0; i < 8; i++) {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, octahedronColors[i]);
        glBegin(GL_TRIANGLES);
        glNormal3fv(octahedronVertices[octahedronIndices[3 * i]]);
        glTexCoord2f(0.5, 0.5);
        glVertex3fv(octahedronVertices[octahedronIndices[3 * i]]);
        glNormal3fv(octahedronVertices[octahedronIndices[3 * i + 1]]);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(octahedronVertices[octahedronIndices[3 * i + 1]]);
        glNormal3fv(octahedronVertices[octahedronIndices[3 * i + 2]]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(octahedronVertices[octahedronIndices[3 * i + 2]]);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable vertex colors
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Draw the octahedron
    drawOctahedron();

    // Disable vertex colors
    glDisable(GL_COLOR_MATERIAL);

    // Camera position
    float cameraZ = 20.0f * sin(cameraAngle);

    // Set up the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            cameraY += 0.1f;
            break;
        case 's':
            cameraY -= 0.1f;
            break;
        case 'a':
            cameraX -= 0.1f;
            break;
        case 'd':
            cameraX += 0.1f;
            break;
        case 'q':
            cameraAngle -= 0.05f;
            break;
        case 'e':
            cameraAngle += 0.05f;
            break;
        case 27: // Escape key
            exit(0);
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Textured Octahedron");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}

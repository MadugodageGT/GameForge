#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <SOIL2.h>
#include <tuple>
#include <cmath>

constexpr float PI = 3.14159265358979323846;
float an = 0;
float increment = 0.4;
int width;
int height;
bool isl1On = false;
bool isl2On = false;
bool isl3On = false;

//timer for textures
int flaame_animation_timer = 0;
int frameDelayCounter = 0;

//To on/off grids and axes
int gridOn = 0;
int axesOn = 0;

//variables to move the camera
GLfloat camX = 0.0; GLfloat camY = 0.0; GLfloat camZ = 0.0;

//variables to move the scene
GLfloat sceRX = 0.0; GLfloat sceRY = 0.0; GLfloat sceRZ = 0.0;
GLfloat sceTX = 0.0; GLfloat sceTY = 0.0; GLfloat sceTZ = 0.0;

//variables to move the objects
GLfloat objRX = 0.0; GLfloat objRY = 0.0; GLfloat objRZ = 0.0;
GLfloat objTX = 0.0; GLfloat objTY = 0.0; GLfloat objTZ = 0.0;

//loading textures
GLuint texture_flame[4];
GLuint texture_ground_block_1[4];
GLuint textures_blocks[5];

unsigned char* char_textures_block[5];

void init(void);

void loadTexture();

std::tuple<float, float, float> calculateNormal(
    const std::tuple<float, float, float>& v1,
    const std::tuple<float, float, float>& v2,
    const std::tuple<float, float, float>& v3);

void drawGrid();

void drawAxes();


void drawCubeWithTexture(GLuint front_texture,
    GLuint back_texture,
    GLuint top_texture,
    GLuint bottom_texture,
    GLuint right_texture,
    GLuint left_texture,
    float height,
    float width);



void setLightingAndShading();
void setFlameLight();
void setFlameMaterial();

void flames_animated(float x, float y, float z);
void ground_block_1();


void block_type_1(GLuint texture);
void block_type_2(GLuint texture);
void block_type_3(GLuint texture);
void Block_01(float x, float y, float z, float rx, float ry, float rz, float angle);
void Block_02(float x, float y, float z, float rx, float ry, float rz, float angle);
void Block_03(float x, float y, float z, float rx, float ry, float rz, float angle);
void Block_04(float x, float y, float z, float rx, float ry, float rz, float angle);
void flame_base(float x, float y, float z, float rx, float ry, float rz, float angle);
void displayUnits();

void display(void);

void reshape(GLsizei w, GLsizei h);

void keyboardSpecial(int key, int x, int y);

void keyboard(unsigned char key, int x, int y);

void Timer(int x);

int main(void) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Textured Cube");
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(60.0, Timer, 1);
    init();
    glutMainLoop();
    return 0;
}


///////////////////////////////////////////////////////////////////

////cube////

void drawCubeWithTexture(GLuint front_texture, GLuint back_texture, GLuint top_texture,
    GLuint bottom_texture, GLuint right_texture, GLuint left_texture,
    float height, float width) {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glPushMatrix();

    float halfHeight = height / 2.0f;
    float halfWidth = width / 2.0f;

    // Front Face
    glBindTexture(GL_TEXTURE_2D, front_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, halfWidth },
            { halfWidth, -halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, halfWidth);
    }
    glEnd();

    // Back Face
    glBindTexture(GL_TEXTURE_2D, back_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, halfHeight, -halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
    }
    glEnd();

    // Top Face
    glBindTexture(GL_TEXTURE_2D, top_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, halfHeight, -halfWidth },
            { -halfWidth, halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, halfHeight, halfWidth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfWidth);
    }
    glEnd();

    // Bottom Face
    glBindTexture(GL_TEXTURE_2D, bottom_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
    }
    glEnd();

    // Right Face
    glBindTexture(GL_TEXTURE_2D, right_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfWidth);
    }
    glEnd();

    // Left Face
    glBindTexture(GL_TEXTURE_2D, left_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { -halfWidth, -halfHeight, halfWidth },
            { -halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
    }
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void block_type_1(GLuint texture) {
    glPushMatrix();

    float halfHeight = 1;
    float halfWidth = 1;

    // Front Face
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, halfWidth },
            { halfWidth, -halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.0f, 0.5f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(0.25f, 0.5f); glVertex3f(halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(0.25f, 1.0f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, halfWidth);
    }
    glEnd();

    // Back Face
    //glBindTexture(GL_TEXTURE_2D, back_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, halfHeight, -halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.5f, 0.5f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.25f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.25f, 0.5f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
    }
    glEnd();

    // Top Face
   // glBindTexture(GL_TEXTURE_2D, top_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, halfHeight, -halfWidth },
            { -halfWidth, halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.5f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.5f, 0.5f); glVertex3f(-halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.75f, 0.5f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.75f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfWidth);
    }
    glEnd();

    // Bottom Face
    //glBindTexture(GL_TEXTURE_2D, bottom_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.75f, 1.0f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.75f, 0.5f); glVertex3f(halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(1.0f, 0.5f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
    }
    glEnd();

    // Right Face
   // glBindTexture(GL_TEXTURE_2D, right_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.25f, 0.0f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.25f, 0.5f); glVertex3f(halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.5f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfWidth);
    }
    glEnd();

    // Left Face
   // glBindTexture(GL_TEXTURE_2D, left_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { -halfWidth, -halfHeight, halfWidth },
            { -halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.25f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.5f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(0.5f, 0.5f); glVertex3f(-halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.25f, 0.5f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
    }
    glEnd();
    glPopMatrix();
}
void block_type_2(GLuint texture) {
    glPushMatrix();

    float halfHeight = 2;
    float halfWidth = 1;

    // Front Face
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, halfWidth },
            { halfWidth, -halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.2f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(0.4f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(0.4f, 1.0f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.2f, 1.0f); glVertex3f(-halfWidth, halfHeight, halfWidth);
    }
    glEnd();

    // Back Face
    //glBindTexture(GL_TEXTURE_2D, back_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, halfHeight, -halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.6f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.6f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.4f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.4f, 0.0f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
    }
    glEnd();

    // Top Face
   // glBindTexture(GL_TEXTURE_2D, top_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, halfHeight, -halfWidth },
            { -halfWidth, halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.5f); glVertex3f(-halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.2f, 0.5f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.2f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfWidth);
    }
    glEnd();

    // Bottom Face
    //glBindTexture(GL_TEXTURE_2D, bottom_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.2f, 0.5f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.5f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(0.2f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
    }
    glEnd();

    // Right Face
   // glBindTexture(GL_TEXTURE_2D, right_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.8f, 0.0f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.8f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.6f, 1.0f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.6f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfWidth);
    }
    glEnd();

    // Left Face
   // glBindTexture(GL_TEXTURE_2D, left_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { -halfWidth, -halfHeight, halfWidth },
            { -halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.8f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.8f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfWidth);

    }
    glEnd();
    glPopMatrix();
}
void block_type_3(GLuint texture) {
    glPushMatrix();

    float halfHeight = 0.25;
    float halfWidth = 0.75;

    glEnable(GL_TEXTURE_2D);
    // Front Face
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, halfWidth },
            { halfWidth, -halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.0f, 0.5f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(0.5f, 0.5f); glVertex3f(halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(0.5f, 0.5545f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.0f, 0.5545f); glVertex3f(-halfWidth, halfHeight, halfWidth);

    }
    glEnd();

    // Back Face
    //glBindTexture(GL_TEXTURE_2D, back_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, halfHeight, -halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.5f, 0.5f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.5f, 0.5545f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.5545f); glVertex3f(halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.5f); glVertex3f(halfWidth, -halfHeight, -halfWidth);

    }
    glEnd();

    // Top Face
   // glBindTexture(GL_TEXTURE_2D, top_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, halfHeight, -halfWidth },
            { -halfWidth, halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.0f, 0.5f); glVertex3f(-halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.5f, 0.0f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.5f, 0.5f); glVertex3f(halfWidth, halfHeight, -halfWidth);
    }
    glEnd();

    // Bottom Face
    //glBindTexture(GL_TEXTURE_2D, bottom_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(1.0f, 0.5f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.5f, 0.5f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.5f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
    }
    glEnd();

    // Right Face
   // glBindTexture(GL_TEXTURE_2D, right_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { halfWidth, -halfHeight, -halfWidth },
            { halfWidth, -halfHeight, halfWidth },
            { halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.5f, 0.5f); glVertex3f(halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.5f, 0.5545f); glVertex3f(halfWidth, halfHeight, -halfWidth);
        glTexCoord2f(0.0f, 0.5545f); glVertex3f(halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.0f, 0.5f); glVertex3f(halfWidth, -halfHeight, halfWidth);
    }
    glEnd();

    // Left Face
   // glBindTexture(GL_TEXTURE_2D, left_texture);
    glBegin(GL_QUADS);
    {
        auto normal = calculateNormal(
            { -halfWidth, -halfHeight, -halfWidth },
            { -halfWidth, -halfHeight, halfWidth },
            { -halfWidth, halfHeight, halfWidth });
        glNormal3f(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal));

        glTexCoord2f(0.0f, 0.5f); glVertex3f(-halfWidth, -halfHeight, -halfWidth);
        glTexCoord2f(0.5f, 0.5f); glVertex3f(-halfWidth, -halfHeight, halfWidth);
        glTexCoord2f(0.5f, 0.5545f); glVertex3f(-halfWidth, halfHeight, halfWidth);
        glTexCoord2f(0.0f, 0.5545f); glVertex3f(-halfWidth, halfHeight, -halfWidth);

    }
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void Block_01(float x, float y, float z, float rx, float ry, float rz, float angle) {
    // position x y z, axis x, y, z, angle
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, rx, ry, rz);
    block_type_1(textures_blocks[0]);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void Block_02(float x, float y, float z, float rx, float ry, float rz, float angle) {
    // position x y z, axis x, y, z, angle;
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, rx, ry, rz);
    block_type_1(textures_blocks[2]);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0, 0, 1.1f);
    setFlameLight();
    glPopMatrix();

}
void Block_03(float x, float y, float z, float rx, float ry, float rz, float angle) {
    // position x y z, axis x, y, z, angle
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, rx, ry, rz);
    block_type_2(textures_blocks[1]);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void Block_04(float x, float y, float z, float rx, float ry, float rz, float angle) {
    // position x y z, axis x, y, z, angle
    //setFlameLight();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, rx, ry, rz);
    block_type_2(textures_blocks[4]);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
}


//ground//
void ground_block_1() {

    glPushMatrix();
    drawCubeWithTexture(texture_ground_block_1[2],
        texture_ground_block_1[2],
        texture_ground_block_1[0],
        texture_ground_block_1[1],
        texture_ground_block_1[2],
        texture_ground_block_1[2],
        0.25f, 12);

    glPopMatrix();

}
void ground_block_2() {

    glPushMatrix();
    drawCubeWithTexture(texture_ground_block_1[2],
        texture_ground_block_1[2],
        texture_ground_block_1[3],
        texture_ground_block_1[1],
        texture_ground_block_1[2],
        texture_ground_block_1[2],
        0.25f, 4);

    glPopMatrix();

}
void flame_base(float x, float y, float z, float rx, float ry, float rz, float angle) {
    // position x y z, axis x, y, z, angle

    //float height = 2;
    //float width = 2;
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, rx, ry, rz);
    block_type_3(textures_blocks[3]);
    flames_animated(0, 1.25, -1);
    glPopMatrix();



}

//displaying units collection
void displayUnits() {
    //GROUND
    glPushMatrix();
    glTranslatef(0, -1.125f, 0);
    ground_block_1();
    glTranslatef(-8, 0, 2);
    ground_block_2();
    glPopMatrix();

    //blocks
    glPushMatrix();
    Block_01(-5, 0, -7, 0, 0, 0, 0);
    Block_03(-3, 0, -7, 0, 0, 0, 0);
    Block_02(-1, 0, -6, 0, 0, 0, 0);
    Block_03(-1, 0, -7, 0, 1, 0, 90);
    Block_03(-3, 1, -9, 0, 0, 0, 0);
    Block_01(-1, 2, -9, 0, 1, 0, 90);
    Block_01(-7, 0, -5, 0, 1, 0, 90);
    Block_01(-7, 0, -3, 0, 0, 0, 0);
    Block_02(-7, 0, -1, 0, 0, 0, 0);
    Block_04( 7, 1, -3, 0, 1, 0, -90);
    Block_02(7, 0, -5, 0, 0, 0, -90);
    Block_03(9, 1, -5, 0, 0, 0, -90);
    Block_03(9, 1, -3, 0, 0, 0, -90);
    Block_02(7, 0, -1, 0, 1, 0, -90);
    Block_03(9, 1, -1, 0, 0, 0, -90);

    glPushMatrix();
    glTranslatef(9, 0, 0.5);
    glScalef(0.5, 2, 0.5);
    Block_01(0, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    flame_base(-9, -1, 3, 0, 0, 0, 0);

    glPopMatrix();

}

//texture loading funtion
void loadTexture() {

    const char* flame_file[4] = {
        "textures/flames/frame0000.png",
        "textures/flames/frame0001.png",
        "textures/flames/frame0002.png",
        "textures/flames/frame0003.png",
    };

    const char* block_texture_files[5] = {
        "textures/block_textures/block_1.png",
        "textures/block_textures/block_2.png",
        "textures/block_textures/block_3.png",
        "textures/block_textures/block_4.png",
        "textures/block_textures/block_5.png",
    };

    const char* ground_block_1_files[4] = {
        "textures/ground/top.jpg",
        "textures/ground/bottom.jpg",
        "textures/ground/sides.jpg",
        "textures/ground/ground_1_top.png"
    };

    //flame texture
    for (int i = 0; i < 4; i++) {
        texture_flame[i] = SOIL_load_OGL_texture(flame_file[i], SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_GL_MIPMAPS | SOIL_FLAG_INVERT_Y);
        if (!texture_flame[i]) {
            printf("Texture loading failed: %s\n", SOIL_last_result());
        }
    }



    //block textures
    //for (int i = 0; i < 5; i++) {
    //    textures_blocks[i] = SOIL_load_OGL_texture(block_texture_files[i], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_GL_MIPMAPS | SOIL_FLAG_INVERT_Y);
    //    if (!textures_blocks[i]) {
    //        printf("Texture loading failed: %s\n", SOIL_last_result());
    //    }
    //}

    for (int i = 0; i < 5; i++) {

        glGenTextures(1, &textures_blocks[i]);
        glBindTexture(GL_TEXTURE_2D, textures_blocks[i]);

        char_textures_block[i] = SOIL_load_image("texture.jpg", &width, &height, 0, SOIL_LOAD_RGB); //loads an image into memory without creating an OpenGL texture object.
        if (char_textures_block[i] == NULL) {
            printf("Error : %s", SOIL_last_result());
        }

     
      
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, char_textures_block[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    }

    //ground block1 texture
    for (int i = 0; i < 4; i++) {
        texture_ground_block_1[i] = SOIL_load_OGL_texture(ground_block_1_files[i], SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_GL_MIPMAPS | SOIL_FLAG_INVERT_Y);
        if (!texture_ground_block_1[i]) {
            printf("Texture loading failed: %s\n", SOIL_last_result());
        }
    }

}

// animated flames
void flames_animated(float x, float y, float z) {


    glEnable(GL_TEXTURE_2D);
    setFlameLight();
    setFlameMaterial();

    glPushMatrix();
    glTranslatef(x, y, z);

    glBindTexture(GL_TEXTURE_2D, texture_flame[flaame_animation_timer]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    glPopMatrix();


    glDisable(GL_TEXTURE_2D);
}

// initialization funtion
void init(void) {
    // glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    loadTexture();
}

// draw axis and grids
void drawGrid() {
    GLfloat step = 1.0f;
    GLint line;

    glBegin(GL_LINES);
    for (line = -20; line <= 20; line += step) {
        glVertex3f(line, -0.4, 20);
        glVertex3f(line, -0.4, -20);

        glVertex3f(20, -0.4, line);
        glVertex3f(-20, -0.4, line);
    }
    glEnd();
}
void drawAxes() {
    glBegin(GL_LINES);
    glLineWidth(1.5);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-200, 0, 0);
    glVertex3f(200, 0, 0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, -200, 0);
    glVertex3f(0, 200, 0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, -200);
    glVertex3f(0, 0, 200);

    glEnd();
}

//lighting
void setLightingAndShading() {
    glEnable(GL_LIGHTING);

    // Light 0
    GLfloat l0_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat l0_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat l0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat l0_position[] = { 0.0f, 10.0f, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, l0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, l0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, l0_position);

    // Light 1
    GLfloat l1_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat l1_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat l1_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat l1_position[] = { 0.0f + objTX, 1.0f + objTZ, -0.5f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, l1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, l1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, l1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, l1_position);


    // Material properties
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat specRef[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specRef);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);



}
void setFlameLight() {
    //glEnable(GL_LIGHT2);  // Enable a new light source

    // Position the light near the flame plane
    GLfloat lightPosition[] = { 0.0f, 0.0f,0.0f, 1.0f };  // Adjust coordinates as needed
    GLfloat lightColor[] = { 1.0f, 0.5f, 0.0f, 1.0f };     // Warm orange color for flame effect

    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightColor);

    // Set attenuation values
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1);  // Constant factor
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 1);    // Linear factor
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.001); // Quadratic factor

    // Reduce the range of light
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.5);  // Base intensity near light source
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 1.5);    // Moderate fall-off
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.001); // Sharper fall-off at distance


    // Optionally, set specular for a shinier look
    GLfloat specularColor[] = { 1.0f, 0.6f, 0.3f, 1.0f };
    glLightfv(GL_LIGHT2, GL_SPECULAR, specularColor);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

}
void setFlameMaterial() {
    GLfloat flameDiffuse[] = { 1.0f, 0.5f, 0.3f, 1.0f }; // Slightly orange diffuse
    GLfloat flameSpecular[] = { 0.8f, 0.4f, 0.2f, 1.0f }; // Specular reflection
    GLfloat flameShininess = 50.0f;  // Shininess for highlights

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, flameDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, flameSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, flameShininess);
}

// display and reshape funtions
void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;


    glMatrixMode(GL_MODELVIEW);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Define the Perspective projection frustum 
    gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    // camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    gluLookAt(-9.0 + camX, 12.0 + camY, 9.0 + camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // move the scene (all the rendered environment) using keyboard keys
    glTranslatef(sceTX, sceTY, sceTZ);
    glRotatef(sceRY, 0.0, 1.0, 0.0);

    //Draw a grid on the xz plane
    glColor3f(1, 1, 1);
    if (gridOn == 1)
        drawGrid();
    //draw the three axes
    if (axesOn == 1)
        drawAxes();

    setLightingAndShading();
    displayUnits();

    glPopMatrix();
    glutSwapBuffers();
}

// keyboard inputs
void keyboardSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        camY += 1;
    if (key == GLUT_KEY_DOWN)
        camY -= 1;
    if (key == GLUT_KEY_RIGHT)
        sceRY += 1;
    if (key == GLUT_KEY_LEFT)
        sceRY -= 1;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'l')
        objRY += 1;
    if (key == 'r')
        objRY -= 1;
    if (key == 'Z')
        sceTZ += 1;
    if (key == 'z')
        sceTZ -= 1;
    if (key == 'w')
        sceTZ -= 1;
    if (key == 's')
        sceTZ += 1;
    if (key == 'd')
        sceTX -= 1;
    if (key == 'a')
        sceTX += 1;

    //Grids and axes
    if (key == 'G')
        gridOn = 1;
    if (key == 'g')
        gridOn = 0;
    if (key == 'A')
        axesOn = 1;
    if (key == 'a')
        axesOn = 0;
    if (key == '1')
        if (!isl1On) {
            glEnable(GL_LIGHT0);
            isl1On = true;
        }
        else {
            glDisable(GL_LIGHT0);
            isl1On = false;
        }
    if (key == '2')
        if (!isl2On) {
            glEnable(GL_LIGHT1);
            isl2On = true;
        }
        else {
            glDisable(GL_LIGHT1);
            isl2On = false;
        }
    if (key == '3')
        if (!isl3On) {
            glEnable(GL_LIGHT2);
            isl3On = true;
        }
        else {
            glDisable(GL_LIGHT2);
            isl3On = false;
        }

    glutPostRedisplay();
}

// timer funtion

void Timer(int x) {
    if (frameDelayCounter % 2 == 0) {  // Change every second timer call
        flaame_animation_timer = (flaame_animation_timer + 1) % 4;
    }
    frameDelayCounter++;
    glutPostRedisplay();
    glutTimerFunc(60, Timer, 1);  // Keep original timer interval
}

// Function to calculate normal
std::tuple<float, float, float> calculateNormal(
    const std::tuple<float, float, float>& v1,
    const std::tuple<float, float, float>& v2,
    const std::tuple<float, float, float>& v3) {

    float x1 = std::get<0>(v1), y1 = std::get<1>(v1), z1 = std::get<2>(v1);
    float x2 = std::get<0>(v2), y2 = std::get<1>(v2), z2 = std::get<2>(v2);
    float x3 = std::get<0>(v3), y3 = std::get<1>(v3), z3 = std::get<2>(v3);

    float ux = x2 - x1, uy = y2 - y1, uz = z2 - z1;
    float vx = x3 - x1, vy = y3 - y1, vz = z3 - z1;

    float nx = uy * vz - uz * vy;
    float ny = uz * vx - ux * vz;
    float nz = ux * vy - uy * vx;

    float length = std::sqrt(nx * nx + ny * ny + nz * nz);
    if (length != 0) {
        nx /= length;
        ny /= length;
        nz /= length;
    }

    return std::make_tuple(nx, ny, nz);
}


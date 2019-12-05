#ifndef UTILITIES2D_H
#define UTILITIES2D_H

#include <QOpenGLFunctions_3_0>
#include <QOpenGLContext>
#include <vector>

class Utilities2D
{
public:
    static QOpenGLFunctions_3_0* getGLFunctions30();
    static GLuint getDefaultFBO();
    static void setContext(QOpenGLContext* context);
    static void createQuad();
    static void drawQuad();
    static GLuint wire_circle(const int number_of_points, GLfloat radius);
    static GLuint wire_quad(GLfloat size);
    static GLuint wire_sphere(const int number_of_points, GLfloat radius);
    static GLuint wire_cylinder(const int number_of_points, GLfloat radius, GLfloat height);
    static GLuint wire_cone(const int number_of_points, GLfloat radius, GLfloat height);
    static int maxInt(int a, int b);
    static void delList(GLuint* lst);

private:
    static void circle_helper(std::vector<QVector3D>* v, const int number_of_points, GLfloat z, GLfloat radius);

private:
    static QOpenGLContext* ms_context;
    static GLuint ms_texture_quad;

    const static float PI;
    const static float PI_TIMES_2;
};

#endif // UTILITIES2D_H

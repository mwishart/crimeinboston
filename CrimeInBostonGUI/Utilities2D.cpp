#include "Utilities2D.h"
#include <QVector3D>
#include <cmath>
#include <vector>

using namespace std;

QOpenGLContext* Utilities2D::ms_context = nullptr;
GLuint Utilities2D::ms_texture_quad = 0;
const float Utilities2D::PI = 3.1415926f;
const float Utilities2D::PI_TIMES_2 = (Utilities2D::PI * 2.0f);

QOpenGLFunctions_3_0* Utilities2D::getGLFunctions30()
{
    QOpenGLFunctions_3_0* funcs = nullptr;
    if (ms_context != nullptr)
        funcs = ms_context->versionFunctions<QOpenGLFunctions_3_0>();
    return funcs;
}
GLuint Utilities2D::getDefaultFBO()
{
    GLuint fbo = 0u;
    if (ms_context != nullptr)
        fbo = ms_context->defaultFramebufferObject();
    return fbo;
}
void Utilities2D::setContext(QOpenGLContext* context)
{
    ms_context = context;
}
void Utilities2D::createQuad()
{
    QOpenGLFunctions_3_0* f = getGLFunctions30();
    if (f != nullptr)
    {
        ms_texture_quad = f->glGenLists(1);
        f->glNewList(ms_texture_quad, GL_COMPILE);
        f->glBegin(GL_TRIANGLE_STRIP);
        f->glTexCoord2i(0, 0);
        f->glVertex2f(0.0f, 0.0f);
        f->glTexCoord2i(0, 1);
        f->glVertex2f(0.0f, 1.0f);
        f->glTexCoord2i(1, 0);
        f->glVertex2f(1.0f, 0.0f);
        f->glTexCoord2i(1, 1);
        f->glVertex2f(1.0f, 1.0f);
        f->glEnd();
        f->glEndList();
    }
}
void Utilities2D::drawQuad()
{
    getGLFunctions30()->glCallList(ms_texture_quad);
}

GLuint Utilities2D::wire_circle(const int number_of_points, GLfloat radius)
{
    GLuint circle = 0u;
    QOpenGLFunctions_3_0* f = getGLFunctions30();
    if (f != nullptr)
    {
        GLfloat x, y, z = 0.0f;
        float angle_iter, angle_incr;
        circle = f->glGenLists(1);
        f->glNewList(circle, GL_COMPILE);
        f->glBegin(GL_TRIANGLE_FAN);
        f->glVertex3f(0.0f, 0.0f, z);
        angle_iter = 0.0f;
        angle_incr = PI_TIMES_2 / static_cast<float>(number_of_points);
        while (angle_iter <= PI_TIMES_2)
        {
            x = cosf(angle_iter) * radius;
            y = sinf(angle_iter) * radius;
            f->glVertex3f(x, y, z);
            angle_iter += angle_incr;
        }
        f->glVertex3f(radius, 0.0f, z);

        f->glEnd();
        f->glEndList();
    }

    return circle;
}
GLuint Utilities2D::wire_quad(GLfloat size)
{
    GLuint quad = 0u;
    QOpenGLFunctions_3_0* f = getGLFunctions30();
    if (f != nullptr)
    {
        GLfloat hs = size * 0.5f, z = 0.0f;
        quad = f->glGenLists(1);
        f->glNewList(quad, GL_COMPILE);
        f->glBegin(GL_TRIANGLE_STRIP);
        f->glVertex3f(hs, -hs, z);
        f->glVertex3f(hs, hs, z);
        f->glVertex3f(-hs, -hs, z);
        f->glVertex3f(-hs, hs, z);
        f->glEnd();
        f->glEndList();
    }
    return quad;
}

GLuint Utilities2D::wire_sphere(const int number_of_points, GLfloat radius)
{
    GLuint sphere = 0u;
    QOpenGLFunctions_3_0* f = getGLFunctions30();
    if (f != nullptr)
    {
        GLfloat x, y, z = 0.0f;
        vector<vector<QVector3D>> p_list;
        float azimuth, azimuth_incr, polar, polar_incr;

        //determine the points
        azimuth = 0.0f;
        azimuth_incr = PI_TIMES_2 / static_cast<float>(number_of_points);
        polar = 0.0f;
        polar_incr = PI / static_cast<float>(number_of_points);
        while (polar <= PI + 0.0001f)
        {
            vector<QVector3D> row;
            azimuth = 0.0f;
            while (azimuth <= PI_TIMES_2 + 0.0001f)
            {
                x = cosf(azimuth) * sinf(polar) * radius;
                y = sinf(azimuth) * sinf(polar) * radius;
                z = cosf(polar) * radius;
                row.push_back(QVector3D(x, y, z));
                azimuth += azimuth_incr;
            }
            p_list.push_back(row);
            polar += polar_incr;
        }

        //make the gl call list
        sphere = f->glGenLists(1);
        f->glNewList(sphere, GL_COMPILE);
        for (unsigned int i = 1; i < p_list.size(); i++)
        {
            const vector<QVector3D>& row_01 = p_list.at(i - 1);
            const vector<QVector3D>& row_02 = p_list.at(i);
            f->glBegin(GL_TRIANGLE_STRIP);
            for (unsigned int j = 0; j < row_01.size(); j++)
            {
                QVector3D v1 = row_01.at(j);
                QVector3D v2 = row_02.at(j);

                f->glVertex3f(v1.x(), v1.y(), v1.z());
                f->glVertex3f(v2.x(), v2.y(), v2.z());
            }
            f->glEnd();
        }
        f->glEndList();
    }

    return sphere;
}

void Utilities2D::circle_helper(std::vector<QVector3D>* v, const int number_of_points, GLfloat z, GLfloat radius)
{
    GLfloat x, y;
    float angle_iter, angle_incr;

    angle_iter = 0.0f;
    angle_incr = PI_TIMES_2 / static_cast<float>(number_of_points);
    while (angle_iter <= PI_TIMES_2 + 0.00001f)
    {
        x = cosf(angle_iter) * radius;
        y = sinf(angle_iter) * radius;
        v->push_back(QVector3D(x, y, z));
        angle_iter += angle_incr;
    }
}

GLuint Utilities2D::wire_cylinder(const int number_of_points, GLfloat radius, GLfloat height)
{
    GLuint cylinder = 0u;
    QOpenGLFunctions_3_0* f = getGLFunctions30();
    if (f != nullptr)
    {
        //determine the vertex positions
        std::vector<QVector3D> circles[2];
        for (int i = 0; i < 2; i++)
        {
            GLfloat z = 0.0f;
            if (i == 0)
                z = -height * 0.5f;
            else
                z = height * 0.5f;
            circle_helper(&circles[i], number_of_points, z, radius);
        }

        //construct the geometry
        cylinder = f->glGenLists(1);
        f->glNewList(cylinder, GL_COMPILE);
        f->glBegin(GL_TRIANGLE_FAN);
        f->glVertex3f(0.0f, 0.0f, circles[0].at(0).z()); //back face
        for (unsigned int i = 0; i < circles[0].size(); i++)
        {
            const QVector3D &v = circles[0].at(i);
            f->glVertex3f(v.x(), v.y(), v.z());
        }
        f->glEnd();
        f->glBegin(GL_TRIANGLE_STRIP); //connection of faces
        for (unsigned int i = 0; i < circles[0].size(); i++)
        {
            const QVector3D &v1 = circles[0].at(i);
            const QVector3D &v2 = circles[1].at(i);
            f->glVertex3f(v1.x(), v1.y(), v1.z());
            f->glVertex3f(v2.x(), v2.y(), v2.z());
        }
        f->glEnd();
        f->glBegin(GL_TRIANGLE_FAN);
        f->glVertex3f(0.0f, 0.0f, circles[1].at(0).z()); //front face
        for (unsigned int i = 0; i < circles[1].size(); i++)
        {
            const QVector3D &v = circles[1].at(i);
            f->glVertex3f(v.x(), v.y(), v.z());
        }
        f->glEnd();
        f->glEndList();
    }

    return cylinder;
}
GLuint Utilities2D::wire_cone(const int number_of_points, GLfloat radius, GLfloat height)
{
    GLuint cone = 0u;
    QOpenGLFunctions_3_0* f = getGLFunctions30();
    if (f != nullptr)
    {
        std::vector<QVector3D> circle;
        const GLfloat z_array[2u] = {height * 0.5f, height * -0.5f};

        //determine the vertex positions
        circle_helper(&circle, number_of_points, z_array[0u], radius);

        cone = f->glGenLists(1);
        f->glNewList(cone, GL_COMPILE);
        for (unsigned int fans = 0u; fans < 2u; fans++)
        {
            f->glBegin(GL_TRIANGLE_FAN);
            f->glVertex3f(0.0f, 0.0f, z_array[fans]); //front face
            for (unsigned int i = 0u; i < circle.size(); i++)
            {
                const QVector3D &v = circle.at(i);
                f->glVertex3f(v.x(), v.y(), v.z());
            }
            f->glEnd();
        }
        f->glEndList();
    }

    return cone;
}

int Utilities2D::maxInt(int a, int b)
{
    int c = b;
    if (a >= b)
        c = a;
    return c;
}

void Utilities2D::delList(GLuint *lst)
{
    if (lst != nullptr && *lst != 0u)
    {
        QOpenGLFunctions_3_0* f = getGLFunctions30();
        if (f != nullptr)
        {
            f->glDeleteLists(*lst, 1);
            *lst = 0u;
        }
    }
}

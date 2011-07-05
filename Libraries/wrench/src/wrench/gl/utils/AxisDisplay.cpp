/*
 *  gl::wrench::VBO.cpp
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "AxisDisplay.h"

wrench::gl::utils::AxisDisplay::AxisDisplay(void)
{
}

wrench::gl::utils::AxisDisplay::~AxisDisplay()
{
}

bool wrench::gl::utils::AxisDisplay::init()
{
    OGLStatus::logOGLErrors("wrench::gl::AxisDisplay - init()");

    float* vertex   = new float[18];	// vertex array
    float* color    = new float[18];     // texture coord array

    vertex[0]   = 0.0f; vertex[1]   = 0.0f; vertex[2]  = 0.0f;  //x
    vertex[3]   = 1.0f; vertex[4]   = 0.0f; vertex[5]  = 0.0f;
    vertex[6]   = 0.0f; vertex[7]   = 0.0f; vertex[8]  = 0.0f;  //y
    vertex[9]   = 0.0f; vertex[10]  = 1.0f; vertex[11] = 0.0f;
    vertex[12]  = 0.0f; vertex[13]  = 0.0f; vertex[14] = 0.0f;  //z
    vertex[15]  = 0.0f; vertex[16]  = 0.0f; vertex[17] = 1.0f;

    color[0]   = 1.0f; color[1]   = 0.0f; color[2]  = 0.0f;  //x
    color[3]   = 1.0f; color[4]   = 0.0f; color[5]  = 0.0f;
    color[6]   = 0.0f; color[7]   = 1.0f; color[8]  = 0.0f;  //y
    color[9]   = 0.0f; color[10]  = 1.0f; color[11] = 0.0f;
    color[12]  = 0.0f; color[13]  = 0.0f; color[14] = 1.0f;  //z
    color[15]  = 0.0f; color[16]  = 0.0f; color[17] = 1.0f;

    m_axis.init(GL_LINES, 6);
    m_vertex.init(3, GL_FLOAT, GL_ARRAY_BUFFER);
    m_vertex.bufferData(6, vertex, GL_STATIC_DRAW);
    m_axis.addVBO(m_vertex, "vert");

    m_color.init(3, GL_FLOAT, GL_ARRAY_BUFFER);
    m_color.bufferData(6, color, GL_STATIC_DRAW);
    m_axis.addVBO(m_color, "vertColor");

    delete [] vertex;
    delete [] color;

    m_axisShader.init();
    m_axisShader.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/AxisDisplay.vert"));
    m_axisShader.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/AxisDisplay.frag"));
    m_axisShader.bindAttributeLocation("vert", 0);
    m_axisShader.bindAttributeLocation("vertColor", 1);

    m_axisShader.link();

    OGLStatus::logOGLErrors("wrench::gl::AxisDisplay - init()");
    return true;
}

void wrench::gl::utils::AxisDisplay::draw(glm::mat4 modelViewMatrix)
{
    glPushAttrib(GL_VIEWPORT_BIT);
    {
        glMatrixMode (GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity ();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

        glMatrixMode (GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glViewport (0, 0, 50, 50);

        glm::mat3 modelViewRotations = glm::mat3(modelViewMatrix);

        m_axisShader.bind();
        {
            m_axisShader.uniform("modelViewMatrix", modelViewRotations);

            //  Need to get the MV matrix
            m_axis.draw();
        }
        m_axisShader.unbind();

        //	Pop back matricies as if nothing happened
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
    glPopAttrib();

    OGLStatus::logOGLErrors("wrench::gl::AxisDisplay - draw()");
}

#include "GradientBackground.h"

wrench::gl::utils::GradientBackground::GradientBackground(void)
{
  for(int i = 0; i < 3; ++i)
  {
    m_color1[i] = 1.0f;
    m_color2[i] = 0.0f;
  }
}

wrench::gl::utils::GradientBackground::GradientBackground(GLfloat color1[3], GLfloat color2[3])
{
  for(int i = 0; i < 3; ++i)
  {
    m_color1[i] = color1[i];
    m_color2[i] = color2[i];
  }
}

wrench::gl::utils::GradientBackground::~GradientBackground()
{

}

void wrench::gl::utils::GradientBackground::draw(void)
{
  _setMatrices();                     //  Sets the projection and model view matrix

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  glBegin (GL_QUADS);                 //  Draw a screen aligned quad
  {
    glColor3fv(m_color2);             //  Color for the bottom
    glVertex3f (-1.0f, -1.0f, -1.0f);
    glVertex3f (1.0f, -1.0f, -1.0f);

    glColor3fv(m_color1);             //  Color for the top
    glVertex3f (1.0f, 1.0f, -1.0f);
    glVertex3f (-1.0f, 1.0f, -1.0f);
  }
  glEnd ();

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

  _restoreMatrices();                // Restores the projection and model view matrix
}

void wrench::gl::utils::GradientBackground::setColors(GLfloat color1[3], GLfloat color2[3])
{
  //  Simply assign the background colors to the colors passed in
  for(int i = 0; i < 3; ++i)
  {
    m_color1[i] = color1[i];
    m_color2[i] = color2[i];
  }
}

void wrench::gl::utils::GradientBackground::_setMatrices()
{
  glMatrixMode(GL_PROJECTION);  //  Switch to the projection matrix stack
  glPushMatrix();               //  Save the current projection matrix
  glLoadIdentity();             //  Load an identity

  glMatrixMode(GL_MODELVIEW);   //  Switch to the Model view matrix stack
  glPushMatrix();               //  Save the current model view matrix
  glLoadIdentity();             //  Load an identity
}

void wrench::gl::utils::GradientBackground::_restoreMatrices()
{
  glPopMatrix();                //  Pop back the matrix. This should be the model view

  glMatrixMode(GL_PROJECTION);  //  Switch back to the projection matrix stack
  glPopMatrix();                //  Pop back the projection matrix

  glMatrixMode(GL_MODELVIEW);   //  Put the matrix back to the model view matrix
}

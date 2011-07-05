#include "GradientBackground.h"

wrench::gl::utils::GradientBackground::GradientBackground(void)
{
  m_width = 0;
  m_height = 0;

   //  Simply assign the background colors to be white and black
  for(int i = 0; i < 3; ++i)
  {
    m_topColor[i] = 1.0f;			// Top color should be white
    m_bottomColor[i] = 0.0f;		// Bottom color should be black
  }
}

wrench::gl::utils::GradientBackground::GradientBackground(GLfloat topColor[3], GLfloat bottomColor[3])
{
  m_width = 0;
  m_height = 0;

  //  Simply assign the background colors to the colors passed in
  for(int i = 0; i < 3; ++i)
  {
    m_topColor[i] 		= topColor[i];
    m_bottomColor[i] 	= bottomColor[i];
  }
}

wrench::gl::utils::GradientBackground::~GradientBackground()
{

}

void wrench::gl::utils::GradientBackground::draw(void)
{
  _setMatrices();                                 //  Sets the projection and model view matrix

  glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT);  //  Save the current state for the lighting and depth
  {
      glDisable(GL_LIGHTING);                     //  Disable lighting since it should not be lighting the background
      glDisable(GL_DEPTH_TEST);                   //  Disable depth so that it is the farthest object in the scene
	
      glBegin (GL_QUADS);                         //  Draw a screen aligned quad
	  {
        glColor3fv(m_bottomColor);                //  Color for the bottom
		glVertex3f (-1.0f, -1.0f, -1.0f);
		glVertex3f (1.0f, -1.0f, -1.0f);
	
        glColor3fv(m_topColor);                   //  Color for the top
		glVertex3f (1.0f, 1.0f, -1.0f);
		glVertex3f (-1.0f, 1.0f, -1.0f);
	  }
	  glEnd ();
  }
  glPopAttrib();					 // Restore the enables that we disabled

  _restoreMatrices();                // Restores the projection and model view matrix
}

void wrench::gl::utils::GradientBackground::setColors(GLfloat topColor[3], GLfloat bottomColor[3])
{
  //  Simply assign the background colors to the colors passed in
  for(int i = 0; i < 3; ++i)
  {
    m_topColor[i] 		= topColor[i];
    m_bottomColor[i] 	= bottomColor[i];
  }
}

void wrench::gl::utils::GradientBackground::resize(int width, int height)
{
  //  Set the width and the height based on the parameters that are passed in
  m_width   = width;
  m_height  = height;
}

void wrench::gl::utils::GradientBackground::_setMatrices()
{
  glMatrixMode(GL_PROJECTION);  //  Switch to the projection matrix stack
  glPushMatrix();               //  Save the current projection matrix
  glLoadIdentity();             //  Load an identity
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);   //  Switch to the Model view matrix stack
  glPushMatrix();               //  Save the current model view matrix
  glLoadIdentity();             //  Load an identity
  glViewport(0, 0, m_width, m_height);
}

void wrench::gl::utils::GradientBackground::_restoreMatrices()
{
  glPopMatrix();                //  Pop back the matrix. This should be the model view

  glMatrixMode(GL_PROJECTION);  //  Switch back to the projection matrix stack
  glPopMatrix();                //  Pop back the projection matrix

  glMatrixMode(GL_MODELVIEW);   //  Put the matrix back to the model view matrix
}

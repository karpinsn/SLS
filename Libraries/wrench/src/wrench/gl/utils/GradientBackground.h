/**
 *  @file
 *  @author   Nikolaus Karpinsky
 *
 *  Class which renders a nice gradient background on an OpenGL scene. To use this
 *	class simply create an instance and then call GradientBackground::draw(void) in
 *	your OpenGL draw loop to render the background. 
 */

#ifndef _GRADIENT_BACKGROUND_H_
#define _GRADIENT_BACKGROUND_H_

#ifdef __APPLE__
#include <glew.h>
#include <OpenGL/gl.h>
#elif _WIN32
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

namespace wrench
{
  namespace gl
  {
    namespace utils
    {
      class GradientBackground
      {
      private:
        GLfloat m_topColor[3];    	//  Top color
        GLfloat m_bottomColor[3];   //  Bottom color

        int m_width;                //  Width of the OpenGL viewport
        int m_height;               //  Height of the OpenGL viewport

      public:
      
      	/**
      	*	Creates a default gradient background
      	*
      	*	Default constructor which creates a gradient background. The top color
      	*	is white and the bottom color is black
      	*/
        GradientBackground(void);
        
        /**
        *	Creates a gradient background with the specified colors
        *
        *	Creates a gradient background with the specified colors. The colors
        *	that are specified should be 3 component vectors that range from (0, 1)
        *
        *	@param topColor[3]		A 3 component vector which specifies the top color of the gradient
        *	@param bottomColor[3]	A 3 component vector which specifies the bottom color of the gradient
        */
        GradientBackground(GLfloat topColor[3], GLfloat bottomColor[3]);
        ~GradientBackground();

		/**
		*	Draws the gradient background for the scene
		*
		*	This function draws a smooth gradient background by rendering a
		*	quad in the background with the current top and bottom colors.
		*	These colors can be changed by using the 
		*	GradientBackground::setColors(GLfloat topColor[3], GLfloat bottomColor[3])
		*	method
		*/
        void draw(void);
        
        /**
        *	Sets the background colors for the gradient background
        *
        *	This function sets the background colors for the gradient background.
        *	The colors that are specified should be 3 component vectors that range
        *	from (0, 1).
        *
        *	@param topColor[3] 		A 3 component vector which specifies the top color of the gradient
        *	@param bottomColor[3] 	A 3 component vector which specifies the bottom color of the gradient
        */
        void setColors(GLfloat topColor[3], GLfloat bottomColor[3]);

        /**
          * Called when the window is resized
          *
          * This method should be called when the window OpenGL is in is resized. This is
          * the viewport can be set in the draw function to be the whole size of the window
          * and then have the background span the entire size.
          *
          * @param width    New width of the OpenGL window
          * @param height   New height of the OpenGL window
          */
        void resize(int width, int height);

      private:
        void _setMatrices(void);
        void _restoreMatrices(void);

      };
    }
  }
}

#endif  //_GRADIENT_BACKGROUND_H_

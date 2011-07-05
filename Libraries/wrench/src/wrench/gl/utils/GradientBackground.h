/**
 *  @filename GradientBackground
 *  @author   Nikolaus Karpinsky
 *
 *  Class which renders a nice gradient background on an OpenGL scene
 */

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
        GLfloat m_color1[3];    //  Top color
        GLfloat m_color2[3];    //  Bottom color

      public:
        GradientBackground(void);
        GradientBackground(GLfloat color1[3], GLfloat color2[3]);
        ~GradientBackground();

        void draw(void);
        void setColors(GLfloat color1[3], GLfloat color2[3]);

      private:
        void _setMatrices(void);
        void _restoreMatrices(void);

      };
    }
  }
}

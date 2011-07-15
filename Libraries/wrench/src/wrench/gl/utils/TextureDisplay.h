/*
 Filename:	CameraGLContext.h
 Author:	Nikolaus Karpinsky
 Date Created:	01/30/11
 Last Edited:	01/30/11
 
 Revision Log:
 01/30/11 - Nik Karpinsky - Original creation.
 */
#ifndef _TEXTURE_DISPLAY_H_
#define _TEXTURE_DISPLAY_H_

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

#include <wrench/gl/ShaderProgram.h>
#include <wrench/gl/Shader.h>
#include <wrench/gl/Texture.h>
#include <wrench/gl/VBO.h>
#include <wrench/gl/VAO.h>
#include <wrench/gl/Camera.h>

namespace wrench
{
  namespace gl
  {
    namespace utils
    {
      class TextureDisplay
      {
      private:
        Camera m_camera;

        ShaderProgram m_textureDisplay;

        bool m_hasBeenInit;

        VAO m_screen;
        VBO m_vertex;
        VBO m_texCoords;

        int m_width;
        int m_height;

      public:
        TextureDisplay(void);

        void init(void);
        void draw(Texture* texture);
        void resize(int width, int height);

      private:
        void _initShaders(void);
        void _cacheQuad(void);
        void _setMatrices(int textureWidth, int textureHeight);
        void _restoreMatrices(void);
      };
    }
  }
}

#endif	// _TEXTURE_DISPLAY_H_

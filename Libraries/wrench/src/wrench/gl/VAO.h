/**
 @filename		FBO.h
 @author		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	10/27/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 10/27/10 - Nik Karpinsky - Allows for VRJ context specific data
 12/09/10 - Nik Karpinsky - Moved file into the Wrench library
 */

#ifndef _WRENCH_GL_VBA_H_
#define _WRENCH_GL_VBA_H_

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

#include "map"

#include "../Logger.h"
#include "OGLStatus.h"
#include "VBO.h"

#ifdef USE_VRJ
#include <vrj/Draw/OpenGL/ContextData.h>
#endif

#ifdef __APPLE__
#define glGenVertexArrays 		glGenVertexArraysAPPLE
#define glDeleteVertexArrays  	glDeleteVertexArraysAPPLE
#define glBindVertexArray		glBindVertexArrayAPPLE
#endif

namespace wrench 
{
  namespace gl
  {
    class VAO
    {
    private:
    
    //	Used for VR Juggler 3.0 compatiability
#ifdef USE_VRJ
      vrj::opengl::ContextData<GLuint>  vrjVAOID;
#define m_vaoID             (*vrjVAOID)
#else
      GLuint m_vaoID;
#endif

      GLenum  m_mode;     // Defined by glDrawArrays: GL_TRIANGLE_STRIP, GL_QUAD, etc...
      GLsizei m_count;    // Defined by glDrawArrays: Number of things to draw

      std::map<std::string, int> m_vertexAttributes;
    public:
    
      /**
      *	Constructer for the VAO. Before it can be drawn using the draw method
      * init must be called, and VBOs must be added using addVBO.
      */
      VAO(void);
      
      /**
      * Destructor for the VAO. If the VAO has been initialized, this method will
      * release the VAO on the graphics card.
      */
      ~VAO();

	  /**
	  *	Initializes the VAO using the specified mode, and the specified count of VBOs.
	  *
	  *	@param mode - Mode of the VAO defined by glDrawArrays: GL_TRIANGLE_STRIP, GL_QUAD, etc...
	  *	@param count - Number of things to draw defined by glDrawArrays
	  */
      bool init(GLenum mode, GLsizei count);
      
      /**
      *	Adds a VBO to this VAO using the specified attribute name.
      *
      *	@param vbo - VBO to add
      *	@param attributeName - name of the attribute specified by the VBO
      */
      void addVBO(VBO& vbo, std::string attributeName);
      
      /**
      *	Returns the address of the specified attribute.
      *
      *	@param attributeName - name of the attribute to retrieve the address for.
      *
      *	@return address of the specified attribute.
      */
      GLuint getVertexAttributeAddress(std::string attributeName);

	  /**
	  *	Draws the VAO using all of the VBOs it contains.
	  */
      void draw();

    private:
      void _bind(void);
      void _unbind(void);
    };
  }
}

#endif	// _WRENCH_GL_VAO_H_

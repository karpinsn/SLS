/**
 @file          IBO.h
 @author		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	10/27/10

 */

#ifndef _WRENCH_GL_IBO_H_
#define _WRENCH_GL_IBO_H_

//  Used to calculate buffer offsets for the buffer object pointers
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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

#include "../Logger.h"
#include "OGLStatus.h"
#include "Converter.h"

#ifdef USE_VRJ
#include <vrj/Draw/OpenGL/ContextData.h>
#endif

namespace wrench 
{
  namespace gl
  {
    class IBO
    {
    private:
#ifdef USE_VRJ
      vrj::opengl::ContextData<GLuint>  vrjIBOID;
#define m_vboID             (*vrjIBOID)
#else
      GLuint m_iboID;
#endif

      GLint   m_componentSize;    // Defined in glVertexAttribPointer as size
      GLenum  m_componentType;    // Defined in glVertexAttribPointer as type
      GLsizei m_componentCount;   // Number of indices stored by this IBO
    public:
      IBO(void);
      ~IBO();

	  /**
      *	Initalizes the IBO.
	  * 
	  *	@param compSize - Number of components per vertex. Defined in glVertexAttribPointer as size.
	  *	@param type - Component type. Defined in glVertexAttribPointer as type.
	  *
	  * @return True or false based on whether or not initalization was successful.
	  */
      bool init(GLint compSize, GLenum type);  // Here compSize is the number of components per vertex

	  /**
	  * Buffers the data to the GPU.
	  * 
	  *	@param size - Size of the data to buffer over.
	  * @param data - Pointer to the first element of the data.
	  * @param usage - Defined in glBufferData as usage.
	  */
      void bufferData(GLsizei size, const GLvoid* data, GLenum usage);

      GLsizei getComponentCount(void);

	  /**
	  *	Returns the number of components per vertex. Defined in glVertexAttribPointer as size.
	  */
      GLint getComponentSize(void);
      
      /**
      *	Returns the type of the components. Defined in glVertexAttribPointer as type.
      *
      *	@return Type of component used by this VBO.
      */
      GLenum getComponentType(void);

      void bind(void);
      void unbind(void);
    };
  }
}

#endif	// _WRENCH_GL_IBO_H_

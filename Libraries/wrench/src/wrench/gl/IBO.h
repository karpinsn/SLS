/**
 @file          IBO.h
 @author		Nikolaus Karpinsky

 Index Buffer Object (IBO) which holds the index values for a
 Vertex Array Object (VAO).
 */

#ifndef _WRENCH_GL_IBO_H_
#define _WRENCH_GL_IBO_H_

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

      /**
        * Creates a new Index Buffer Object which can be used to store indicies for a
        * Vertex Array Object (VAO)
        */
      IBO(void);

      /**
        * Default destructor which releases the GPU memory used to hold the IBO
        */
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

      /**
        * Returns the number of components (indicies) held in this IBO.
        *
        * @return Number of components (indicies) held in this IBO.
        */
      GLsizei getComponentCount(void);

	  /**
      *	Returns the number of components per index. Defined in glVertexAttribPointer as size.
      *
      * @return Number of components per index. This is typically 1, but can differ under certain
      *         circumstances
	  */
      GLint getComponentSize(void);
      
      /**
      *	Returns the type of the components. Defined in glVertexAttribPointer as type.
      *
      *	@return Type of component used by this VBO.
      */
      GLenum getComponentType(void);

      /**
        * Binds the IBO as the current GL_ELEMENT_ARRAY_BUFFER.
        */
      void bind(void);

      /**
        * Unbinds the IBO as the current GL_ELEMENT_ARRAY_BUFFER, setting
        * the currently bound to 0.
        */
      void unbind(void);
    };
  }
}

#endif	// _WRENCH_GL_IBO_H_

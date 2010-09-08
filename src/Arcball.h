#ifndef _Arcball_h
#define _Arcball_h

#include <QtOpenGL/QGLWidget>

#include <glm/glm.hpp>
#include <glm/gtc/Quaternion.hpp>
#include <glm/gtx/type_ptr.hpp>

# define Epsilon 1.0e-5

class Arcball
{	
public:
	//Create/Destroy
	Arcball(GLfloat NewWidth, GLfloat NewHeight);
	~Arcball() { /* nothing to do */ };
	
	//Set new bounds
	inline void setBounds(GLfloat NewWidth, GLfloat NewHeight)
	{
		assert((NewWidth > 1.0f) && (NewHeight > 1.0f));
		
		//Set adjustment factor for width/height
		this->AdjustWidth  = 1.0f / ((NewWidth  - 1.0f) * 0.5f);
		this->AdjustHeight = 1.0f / ((NewHeight - 1.0f) * 0.5f);
	}
	
	//Mouse down
	void    click(const glm::vec2& NewPt);
	
	//Mouse drag, calculate rotation
	void    drag(const glm::vec2& NewPt, glm::quat& NewRot);
	
protected:
	glm::vec3   m_startVector;          //Saved click vector
	glm::vec3   m_endVector;          //Saved drag vector
	GLfloat     AdjustWidth;    //Mouse bounds width
	GLfloat     AdjustHeight;   //Mouse bounds height
	
private:
	inline void mapToSphere(const glm::vec2& NewPt, glm::vec3& NewVec) const;
};

#endif


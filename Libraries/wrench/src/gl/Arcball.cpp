#include "Arcball.h"                                   

wrench::gl::Arcball::Arcball()
{	
}

void wrench::gl::Arcball::init(GLfloat width, GLfloat height)
{
	//	Clear initial values
	m_startVector = glm::vec3(0.0f);
	m_endVector = glm::vec3(0.0f);
	
	//	Set initial bounds
    this->setBounds(width, height);
}

void wrench::gl::Arcball::mousePressEvent(const GLint mouseX, const GLint mouseY)
{
	m_lastRotation = m_thisRotation;
    
	//	Map the point to the sphere
    m_startVector = this->mapPointToSphere(glm::vec2(mouseX, mouseY));
}

void wrench::gl::Arcball::mouseMoveEvent(const GLint mouseX, const GLint mouseY)
{
	glm::quat drawQuat;
	
    //Map the point to the sphere
    m_endVector = this->mapPointToSphere(glm::vec2(mouseX, mouseY));
	
	//Compute the vector perpendicular to the begin and end vectors
	glm::vec3 perpendicular = glm::cross(m_startVector, m_endVector);
	
	//Compute the length of the perpendicular vector
	if (glm::length(perpendicular) > Epsilon)    //	if its non-zero
	{
		//We're ok, so return the perpendicular vector as the transform after all
		drawQuat.x = perpendicular.x;
		drawQuat.y = perpendicular.y;
		drawQuat.z = perpendicular.z;
		//In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
		drawQuat.w = glm::dot(this->m_startVector, m_endVector);
	}
	else                               //	if its zero
	{
		//The begin and end vectors coincide, so return an identity transform
		drawQuat = glm::quat();
	}
	
	drawQuat = glm::normalize(drawQuat);						//	Normalize the Quat before casting
	m_thisRotation = glm::mat4_cast(drawQuat);
	
	m_thisRotation = m_thisRotation * m_lastRotation;
	m_transform = m_thisRotation;
}

glm::mat4 wrench::gl::Arcball::getTransform(void)
{
	return m_transform;
}

void wrench::gl::Arcball::applyTransform(void)
{
	glMultMatrixf(glm::value_ptr(m_transform));
}

glm::vec3 wrench::gl::Arcball::mapPointToSphere(const glm::vec2& point) const
{
	glm::vec3 mappedVector(0.0f);
	
    //	Adjust point coords and scale down to range of [-1 ... 1]
    mappedVector.x  =        (point.x * m_adjustWidth)  - 1.0f;
    mappedVector.y  = 1.0f - (point.y * m_adjustHeight);
	
    //	Compute the length of the vector to see if it is inside or outside the circle
	GLfloat length = glm::length(mappedVector);
	
    if (length > 1.0f)	//	It's on the outside
    {
		//	Normalize the vector
		mappedVector = glm::normalize(mappedVector);
    }
    else				//	It's on the inside
    {
        //	Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
        mappedVector.z = glm::sqrt(1.0f - length);
    }
	
	return mappedVector;
}

void wrench::gl::Arcball::setBounds(GLfloat width, GLfloat height)
{		
	//	Set adjustment factor for width & height
	this->m_adjustWidth  = 1.0f / ((width  - 1.0f) * 0.5f);
	this->m_adjustHeight = 1.0f / ((height - 1.0f) * 0.5f);
}
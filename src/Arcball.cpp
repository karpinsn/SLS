#include "Arcball.h"                                            // ArcBall header

void Arcball::mapToSphere(const glm::vec2& NewPt, glm::vec3& NewVec) const
{
	glm::vec2 TempPt;
    GLfloat length;
	
    //Copy paramter into temp point
    TempPt = NewPt;
	
    //Adjust point coords and scale down to range of [-1 ... 1]
    TempPt.x  =        (TempPt.x * this->AdjustWidth)  - 1.0f;
    TempPt.y  = 1.0f - (TempPt.y * this->AdjustHeight);
	
    //Compute the square of the length of the vector to the point from the center
    //length      = (TempPt.x * TempPt.x) + (TempPt.y * TempPt.y);
	length = glm::length(TempPt);
	
    //If the point is mapped outside of the sphere... (length > radius squared)
    if (length > 1.0f)
    {
		//	Normalize the vector
        NewVec.z = 0.0f;
		glm::normalize(NewVec);
    }
    else    //Else it's on the inside
    {
        //Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
        NewVec.x = TempPt.x;
        NewVec.y = TempPt.y;
        NewVec.z = glm::sqrt(1.0f - length);
    }
}

//Create/Destroy
Arcball::Arcball(GLfloat NewWidth, GLfloat NewHeight)
{
    //Clear initial values
	m_startVector = glm::vec3(0.0f);
	m_endVector = glm::vec3(0.0f);
	
    //Set initial bounds
    this->setBounds(NewWidth, NewHeight);
}

//Mouse down
void Arcball::click(const glm::vec2& NewPt)
{
    //Map the point to the sphere
    this->mapToSphere(NewPt, m_startVector);
}

//Mouse drag, calculate rotation
void Arcball::drag(const glm::vec2& NewPt, glm::quat& NewRot)
{
    //Map the point to the sphere
    this->mapToSphere(NewPt, m_endVector);
	
    //Return the quaternion equivalent to the rotation
    if (true)
    {
		glm::vec3  Perp;
		
        //Compute the vector perpendicular to the begin and end vectors
        Perp = glm::cross(m_startVector, m_endVector);
		
        //Compute the length of the perpendicular vector
        if (glm::length(Perp) > Epsilon)    //if its non-zero
        {
            //We're ok, so return the perpendicular vector as the transform after all
            NewRot.x = Perp.x;
            NewRot.y = Perp.y;
            NewRot.z = Perp.z;
            //In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
            NewRot.w = glm::dot(this->m_startVector, m_endVector);
        }
        else                                    //if its zero
        {
            //The begin and end vectors coincide, so return an identity transform
            NewRot.x = 
            NewRot.y = 
            NewRot.z = 
            NewRot.w = 0.0f;
        }
    }
}
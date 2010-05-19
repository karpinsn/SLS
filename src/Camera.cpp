#include "Camera.h"

Camera::Camera()
{
	tb_transform = new Matrix();
	tb_accuTransform = new Matrix();
	
    tb_mode = 1; // default is the rotation mode
    tb_currPos[0] = 0.0f;
    tb_currPos[1] = 0.0f;
    tb_prevPos[0] = 0.0f;
    tb_prevPos[1] = 0.0f;
    tb_moving = GL_FALSE;
    tb_width = 1;
    tb_height = 1;
	m_currentView = 0;
}

Camera::~Camera(void)
{
	delete tb_transform;
	delete tb_accuTransform;
}

void Camera::init()
{
    /* put the identity in the trackball transform */
    glPushMatrix();
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform->m);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_accuTransform->m);
    glPopMatrix();
	m_viewLength = tb_accuTransform->m[14];
}

void Camera::init(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
    /* put the identity in the trackball transform */
    glPushMatrix();
    glLoadIdentity();
	tb_transform->Identity();
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_accuTransform->m);
    glPopMatrix();
	m_viewLength = tb_accuTransform->m[14];
}

void Camera::initRotatedCam(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	/* put the identity in the trackball transform */
    glPushMatrix();
    glLoadIdentity();
	tb_transform->Identity();
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_accuTransform->m);
    glPopMatrix();
	m_viewLength = tb_accuTransform->m[14];
}

void Camera::setMode(int mode)
{
	tb_transform->Identity();
    tb_mode = mode;
}

void Camera::applyMatrix()
{
  // multiple the current one with the previous transformation
  glPushMatrix();
  glLoadIdentity();
	
	float accRotation[16];
	for(int i=0; i<16;i++)
	{
		accRotation[i] = tb_accuTransform->m[i];
	}
	// make accRotation as a rotation only matrix
  accRotation[12] = 0.0;
  accRotation[13] = 0.0;
  accRotation[14] = 0.0;
	
  glTranslatef(tb_accuTransform->m[12], tb_accuTransform->m[13], tb_accuTransform->m[14]); // Translation later
  glMultMatrixf((GLfloat *)tb_transform);                     // Addition transformation second
  glMultMatrixf((GLfloat *)accRotation);                      // Pure rotation first
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_accuTransform);

  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_accuTransform->m);
  
	
  glPopMatrix();
  // Set to the current matrix
  glMultMatrixf((GLfloat *)tb_accuTransform->m);
}

void Camera::reshape(int width, int height)
{
  tb_width  = width;
  tb_height = height;
}

void Camera::mousePressed(int x, int y)
{
	m_viewLength = tb_accuTransform->m[14];
	tb_transform->Identity();
	
    tb_currPos[0] = (float)x / (float)tb_width;
    tb_currPos[1] = (float)y / (float)tb_height;
    tb_prevPos[0] = (float)x / (float)tb_width;
    tb_prevPos[1] = (float)y / (float)tb_height;
    tb_moving = GL_TRUE;
}

void Camera::mouseRelease(int x, int y)
{
	tb_moving = GL_FALSE;
}

void Camera::mouseMotion(int x, int y)
{
    if (!tb_moving) return;

    tb_currPos[0] = (float)x / (float)tb_width;
    tb_currPos[1] = (float)y / (float)tb_height;

    float dx = tb_currPos[0] - tb_prevPos[0];
	float dy = tb_currPos[1] - tb_prevPos[1];
	float mag = sqrtf(dx * dx  + dy * dy);

    if ( mag < 1.0e-6f ) return;

    switch (tb_mode)
    {
        case 1: _rotate(dx, dy); break;
        case 2: _zoom(dx, dy); break;
        case 3: _twist(dx, dy); break;
        case 4: _pan(dx, dy); break;
		default: break;
    }


    tb_prevPos[0] = tb_currPos[0];
    tb_prevPos[1] = tb_currPos[1];
}

void Camera::_rotate(float dx, float dy)
{
	float sx[3], sy[3], sz[3], axis[3];
	float mag = sqrtf( dx * dx  + dy * dy );
	float angle = mag * 200.0f;
	
	_getScreenXYZ(sx, sy, sz);

	axis[0] = sx[0] * dy + sy[0] * dx;
    axis[1] = sx[1] * dy + sy[1] * dx;
    axis[2] = sx[2] * dy + sy[2] * dx;

	_atRotate(axis[0], axis[1], axis[2], angle);
}

void Camera::_getScreenXYZ(float sx[3], float sy[3], float sz[3])
{
    float mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)mat);
    sx[0] = mat[0];
    sx[1] = mat[4];
    sx[2] = mat[8];
    sy[0] = mat[1];
    sy[1] = mat[5];
    sy[2] = mat[9];
    sz[0] = mat[2];
    sz[1] = mat[6];
    sz[2] = mat[10];
}

void Camera::_atRotate(float x,  float y,  float z,  float angle)
{
   int i = 0;
   float rad = (float)(angle * 0.017453293);
   float cosAng = (float)(cos(rad));
   float sinAng = (float)(sin(rad));

   // normalize axis(x,y,z)
   float demon = sqrt(x*x + y*y + z*z);
   float axis[3] = {0.0f, 0.0f, 0.0f};
   if (demon!= 0.0f) 
   {
      axis[0] = x / demon;
      axis[1] = y / demon; 
      axis[2] = z / demon;
   }

   // reset tb_transform matrix
   for(i=0; i<16;i++) tb_transform->m[i] = 0.0f;

   float x2 = axis[0] * axis[0];
   float xy = axis[0] * axis[1];
   float xz = axis[0] * axis[2];

   float y2 = axis[1] * axis[1];
   float yz = axis[1] * axis[2];
   float z2 = axis[2] * axis[2];

   tb_transform->m[0] = x2 + (cosAng * (1 - x2));
   tb_transform->m[1] = xy - (cosAng * xy) + (sinAng * axis[2]);
   tb_transform->m[2] = xz - (cosAng * xz) - (sinAng * axis[1]);

   tb_transform->m[4] = xy - (cosAng * xy) - (sinAng * axis[2]);
   tb_transform->m[5] = y2 + (cosAng * (1 - y2));
   tb_transform->m[6] = yz - (cosAng * yz) + (sinAng * axis[0]);

   tb_transform->m[8] = xz - (cosAng * xz) + (sinAng * axis[1]);
   tb_transform->m[9] = yz - (cosAng * yz) - (sinAng * axis[0]);
   tb_transform->m[10] = z2 + (cosAng * (1 - z2));

   tb_transform->m[15] = 1.0f;
}

void Camera::_zoom(float dx, float dy)
{	
    // Implement zoom function
	float zoomfactor = m_viewLength * ((1.0f/(1.0f + dy * 2.0f))-1.0f);
	
	tb_transform->Identity();
	tb_transform->m[14] = zoomfactor;
}

void Camera::_twist(float dx, float dy)
{
    // Implement twist function	
	Vector previousClickVec(.5f, .5f, 0.0f, tb_prevPos[0], tb_prevPos[1], 0.0f);
	previousClickVec.normalize();
	Vector currentClickVec(.5f, .5f, 0.0f, tb_currPos[0], tb_currPos[1], 0.0f);
	currentClickVec.normalize();
	
	float sx[3], sy[3], sz[3];	
	_getScreenXYZ(sx, sy, sz);
	Vector screenX(sx);
	screenX.normalize();
	
	float theta1 = acosf(screenX.dotProduct(previousClickVec));
	float theta2 = acosf(screenX.dotProduct(currentClickVec));
	float theta3 = (theta2 - theta1) * RadToDeg;
	//	Due to the fact that the angle could be > 180 from x axis, make sure to
	//	invert the angle if it dips below the screen Y
	theta3 = tb_currPos[1] > .5 ? theta3 * -1 : theta3;

	_atRotate(sz[0], sz[1], sz[2], theta3);	
}

void Camera::_pan(float dx, float dy)
{
    float distance = tb_accuTransform->m[14];
	float worldWidth = 2.0f * distance * tanf((60.0f/2.0f) * DegToRad);
	
	float aspect = (float)tb_width / (float)tb_height;
	float wx = -worldWidth * aspect * dx;
	float wy = worldWidth * dy;
	
	tb_transform->Identity();
	tb_transform->m[12] = wx;
	tb_transform->m[13] = wy;	
}
#include "Camera.h"

wrench::gl::Camera::Camera()
{	
    tb_mode = 1; // default is the rotation mode
    tb_currPos[0] = 0.0f;
    tb_currPos[1] = 0.0f;
    tb_prevPos[0] = 0.0f;
    tb_prevPos[1] = 0.0f;
    tb_moving = GL_FALSE;
    tb_width = 512;
    tb_height = 512;
	m_currentView = 0;
	tb_mode = 2;
}

wrench::gl::Camera::~Camera(void)
{
}

void wrench::gl::Camera::init()
{
    /* put the identity in the trackball transform */
    glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(tb_transform));
		glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(tb_accuTransform));
    glPopMatrix();
	m_viewLength = glm::value_ptr(tb_accuTransform)[14];	//	TODO come back and fix this
	m_viewLength = 1.0;
}

void wrench::gl::Camera::init(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
    /* put the identity in the trackball transform */
    glPushMatrix();
    glLoadIdentity();
	
	//	Set to the identity
	glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(tb_transform));
	
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(tb_accuTransform));
    glPopMatrix();
	m_viewLength = glm::value_ptr(tb_accuTransform)[14];
		m_viewLength = .3;
}

void wrench::gl::Camera::initRotatedCam(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	/* put the identity in the trackball transform */
    glPushMatrix();
    glLoadIdentity();
	
	//	Set to the identity
	glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(tb_transform));
	
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(tb_accuTransform));
    glPopMatrix();
	m_viewLength = glm::value_ptr(tb_accuTransform)[14];
		m_viewLength = 2.0;
}

void wrench::gl::Camera::setMode(int mode)
{
	//	Set to the identity
	tb_transform = glm::mat4();
    tb_mode = mode;
}

void wrench::gl::Camera::applyMatrix()
{
  // multiple the current one with the previous transformation
  glPushMatrix();
  {
	  glLoadIdentity();
	
	  float accRotation[16];
	  for(int i=0; i<16;i++)
	  {
			accRotation[i] = glm::value_ptr(tb_accuTransform)[i];
	  }
	  // make accRotation as a rotation only matrix
	  accRotation[12] = 0.0;
	  accRotation[13] = 0.0;
	  accRotation[14] = 0.0;
	
	  glTranslatef(glm::value_ptr(tb_accuTransform)[12], glm::value_ptr(tb_accuTransform)[13], glm::value_ptr(tb_accuTransform)[14]); // Translation later
	  glMultMatrixf(glm::value_ptr(tb_transform));                     // Addition transformation second
	  glMultMatrixf(accRotation);                      // Pure rotation first

	  tb_transform = glm::mat4();
	  glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(tb_accuTransform));
  
  }
  glPopMatrix();
  // Set to the current matrix
  glMultMatrixf(glm::value_ptr(tb_accuTransform));
}

void wrench::gl::Camera::reshape(int width, int height)
{
  tb_width  = width;
  tb_height = height;
}

void wrench::gl::Camera::mousePressed(int x, int y)
{
	m_viewLength = glm::value_ptr(tb_accuTransform)[14];
	
	//	Set to the identity
	//tb_transform = glm::mat4();
	
    tb_currPos[0] = (float)x / (float)tb_width;
    tb_currPos[1] = (float)y / (float)tb_height;
    tb_prevPos[0] = (float)x / (float)tb_width;
    tb_prevPos[1] = (float)y / (float)tb_height;
    tb_moving = GL_TRUE;
}

void wrench::gl::Camera::mouseRelease(int x, int y)
{
	tb_transform = glm::mat4();
	tb_moving = GL_FALSE;
}

void wrench::gl::Camera::mouseMotion(int x, int y)
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
        case ROTATE_MODE:	_rotate(dx, dy);	break;
        case ZOOM_MODE:		_zoom(dx, dy);		break;
        case TWIST_MODE:	_twist(dx, dy);		break;
        case PAN_MODE:		_pan(dx, dy);		break;
		default:								break;
    }

    tb_prevPos[0] = tb_currPos[0];
    tb_prevPos[1] = tb_currPos[1];
}

void wrench::gl::Camera::_rotate(float dx, float dy)
{
	float sx[3], sy[3], sz[3], axis[3];
	float mag = sqrtf( dx * dx  + dy * dy );
	float angle = mag * 200.0f;
	
	_getScreenXYZ(sx, sy, sz);

	axis[0] = sx[0] * dy + sy[0] * dx;
    axis[1] = sx[1] * dy + sy[1] * dx;
    axis[2] = sx[2] * dy + sy[2] * dx;

	//	Set the identity
	tb_transform = glm::mat4();
	//	Then rotate
	tb_transform = glm::rotate(tb_transform, angle, glm::vec3(axis[0], axis[1], axis[2]));
}

void wrench::gl::Camera::_getScreenXYZ(float sx[3], float sy[3], float sz[3])
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

void wrench::gl::Camera::_zoom(float dx, float dy)
{	
    // Implement zoom function
	float zoomfactor = m_viewLength * ((1.0f/(1.0f + dy * 2.0f))-1.0f);
	
	//	Set to the identity
	tb_transform = glm::mat4();
	
	glm::value_ptr(tb_transform)[14] = zoomfactor;
}

void wrench::gl::Camera::_twist(float dx, float dy)
{
    // Implement twist function	
	glm::vec3 previousClickVec = glm::vec3(tb_prevPos[0], tb_prevPos[1], 0.0f) - glm::vec3(.5f, .5f, 0.0f);
	glm::vec3 currentClickVec = glm::vec3(tb_currPos[0], tb_currPos[1], 0.0f) - glm::vec3(.5f, .5f, 0.0f);
	previousClickVec = glm::normalize(previousClickVec);
	currentClickVec = glm::normalize(currentClickVec);
	
	float sx[3], sy[3], sz[3];	
	_getScreenXYZ(sx, sy, sz);
	glm::vec3 screenX = glm::vec3(sx[0], sx[1], sx[2]);
	screenX = glm::normalize(screenX);
	
	float theta1 = acosf(glm::dot(screenX, previousClickVec));
	float theta2 = acosf(glm::dot(screenX, currentClickVec));
	float theta3 = (theta2 - theta1) * RadToDeg;
	//	Due to the fact that the angle could be > 180 from x axis, make sure to
	//	invert the angle if it dips below the screen Y
	theta3 = tb_currPos[1] > .5 ? theta3 * -1 : theta3;

	//	Set the identity
	tb_transform = glm::mat4();
	//	Then rotate
	tb_transform = glm::rotate(tb_transform, theta3, glm::vec3(sz[0], sz[1], sz[2]));
}

void wrench::gl::Camera::_pan(float dx, float dy)
{
    float distance = glm::value_ptr(tb_accuTransform)[14];
	
	float worldWidth = 2.0f * distance * tanf((60.0f/2.0f) * DegToRad);
	
	float aspect = (float)tb_width / (float)tb_height;
	float wx = -worldWidth * aspect * dx;
	float wy = worldWidth * dy;
	
	//	Set to the identity
	tb_transform = glm::mat4();
	
	glm::value_ptr(tb_transform)[12] = wx;
	glm::value_ptr(tb_transform)[13] = wy;	
}
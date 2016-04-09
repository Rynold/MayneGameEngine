#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

#include "LinearMath/btIDebugDraw.h"
#include <GL\glew.h>
#include <glm.hpp>
#include <vector>
#include "Shader.h"

class GLDebugDrawer : public btIDebugDraw
{
private:
	GLuint VAO;
	GLuint VBO;

	std::vector<GLfloat> lines;

	int m_debugMode;

	Shader* debugShader;

public:

	GLDebugDrawer();
	virtual ~GLDebugDrawer();

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	virtual void	drawSphere(const btVector3& p, btScalar radius, const btVector3& color);

	virtual void	drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha);

	virtual void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	//virtual void    drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans, const btVector3& color) override;

	//virtual void    drawPlane(const btVector3& planeNormal, btScalar planeConst, const btTransform& transform, const btVector3& color);

	virtual void	reportErrorWarning(const char* warningString);

	virtual void	draw3dText(const btVector3& location, const char* textString);

	virtual void	setDebugMode(int debugMode);

	virtual int		getDebugMode() const { return m_debugMode; }

	void DrawWorld(glm::mat4 proj, glm::mat4 mod);
};

#endif//GL_DEBUG_DRAWER_H
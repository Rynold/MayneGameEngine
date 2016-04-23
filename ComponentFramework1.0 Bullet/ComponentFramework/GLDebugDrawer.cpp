#include "GLDebugDrawer.h"
#include "gtc\matrix_transform.inl"
#include <stdio.h> //printf debugging

#define BUFFER_OFFSET(x)  ((const void*) (x))

GLDebugDrawer::GLDebugDrawer()
	:m_debugMode(0)
{
	debugShader = new Shader("Shaders/debugShader.vert", "Shaders/debugShader.frag");
}

GLDebugDrawer::~GLDebugDrawer()
{
	glDeleteBuffers(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);

	delete debugShader;
}

void GLDebugDrawer::DrawWorld(glm::mat4 proj, glm::mat4 mod)
{
	debugShader->Use();
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(GLfloat), &lines[0], GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)));


	glBindVertexArray(this->VAO);
	int numVertices = lines.size() / 6;

	glUniformMatrix4fv(glGetUniformLocation(debugShader->Program, "ProjectionMatrix"), 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(debugShader->Program, "ModelViewMatrix"), 1, GL_FALSE, &mod[0][0]);

	glDrawArrays(GL_LINES, 0, numVertices);

	lines.clear();
	glBindVertexArray(0);

	glDeleteBuffers(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
}

void	GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
	lines.push_back(from.x());
	lines.push_back(from.y());
	lines.push_back(from.z());
	lines.push_back(fromColor.x());
	lines.push_back(fromColor.y());
	lines.push_back(fromColor.z());

	lines.push_back(to.x());
	lines.push_back(to.y());
	lines.push_back(to.z());
	lines.push_back(toColor.x());
	lines.push_back(toColor.y());
	lines.push_back(toColor.z());
}

void	GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	drawLine(from, to, color, color);
}

//void GLDebugDrawer::drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans, const btVector3& color)
//{
//	btIDebugDraw::drawBox(bbMin, bbMax, trans, color);
//	
//}

//void GLDebugDrawer::drawPlane(const btVector3& planeNormal, btScalar planeConst, const btTransform& transform, const btVector3& color)
//{
//
//
//	btIDebugDraw::drawPlane(planeNormal, planeConst, transform, color);
//}

void GLDebugDrawer::drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
{
	glColor4f(color.getX(), color.getY(), color.getZ(), btScalar(1.0f));
	glPushMatrix();
	glTranslatef(p.getX(), p.getY(), p.getZ());

	int lats = 5;
	int longs = 5;

	int i, j;
	for (i = 0; i <= lats; i++) {
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar)(i - 1) / lats);
		btScalar z0 = radius*sin(lat0);
		btScalar zr0 = radius*cos(lat0);

		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar)i / lats);
		btScalar z1 = radius*sin(lat1);
		btScalar zr1 = radius*cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			btScalar lng = 2 * SIMD_PI * (btScalar)(j - 1) / longs;
			btScalar x = cos(lng);
			btScalar y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}

	glPopMatrix();
}


void	GLDebugDrawer::drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha)
{
	//	if (m_debugMode > 0)
	{
		const btVector3	n = btCross(b - a, c - a).normalized();
		glBegin(GL_TRIANGLES);
		glColor4f(color.getX(), color.getY(), color.getZ(), alpha);
		glNormal3d(n.getX(), n.getY(), n.getZ());
		glVertex3d(a.getX(), a.getY(), a.getZ());
		glVertex3d(b.getX(), b.getY(), b.getZ());
		glVertex3d(c.getX(), c.getY(), c.getZ());
		glEnd();
	}
}

void	GLDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;

}

void	GLDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	glRasterPos3f(location.x(), location.y(), location.z());
	//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void	GLDebugDrawer::reportErrorWarning(const char* warningString)
{
	printf("%s\n", warningString);
}

void	GLDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

	{
		btVector3 to = pointOnB + normalOnB * 1;//distance;
		const btVector3&from = pointOnB;
		glColor4f(color.getX(), color.getY(), color.getZ(), 1.f);
		//glColor4f(0,0,0,1.f);
		glBegin(GL_LINES);
		glVertex3d(from.getX(), from.getY(), from.getZ());
		glVertex3d(to.getX(), to.getY(), to.getZ());
		glEnd();


		//		glRasterPos3f(from.x(),  from.y(),  from.z());
		//		char buf[12];
		//		sprintf(buf," %d",lifeTime);
		//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);


	}
}
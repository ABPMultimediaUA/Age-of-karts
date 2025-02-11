
#ifndef DEBUGFISICAS_H
#define DEBUGFISICAS_H

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include <cstdlib>
#include <iostream>
#include "TMotor.hpp"

using namespace std;

class DebugDraw : public btIDebugDraw
{

public:
	DebugDraw(GLFWwindow *const device) : mode(DBG_NoDebug) {}

	void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
	void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
	void reportErrorWarning(const char *text);
	void draw3dText(const btVector3 &location, const char *text);
	void setDebugMode(int mode) { this->mode = mode; }
	int getDebugMode() const { return this->mode; }

private:
	int mode;
	unsigned int index = 0;
};

#endif /* DEBUGFISICAS_H */
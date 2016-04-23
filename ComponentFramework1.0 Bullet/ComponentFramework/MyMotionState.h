#ifndef MYMOTIONSTATE_H
#define MYMOTIONSTATE_H

#include "btBulletDynamicsCommon.h"
#include "GameObject.h"

class MyMotionState : public btMotionState
{
protected:
	GameObject* object;
	btTransform initialPosition;

public:

	MyMotionState(btTransform &initialPos, GameObject* obj)
	{
		initialPosition = initialPos;
		object = obj;
	}

	virtual ~MyMotionState()
	{
		object = nullptr;
	}

	void SetObject(GameObject* obj)
	{
		object = obj;
	}

	virtual void getWorldTransform(btTransform &worldTrans) const
	{
		worldTrans = initialPosition;
	}

	virtual void setWorldTransform(const btTransform &worldTrans)
	{
		if (object == nullptr) return;
		btQuaternion rot = worldTrans.getRotation();
		btVector3 pos = worldTrans.getOrigin();
		object->_transform->SetBulletTransform(worldTrans);
		//GLfloat m[16];
		//worldTrans.getOpenGLMatrix(&(object->_transform->temp[0]));

		//object->SetPosition(pos.getX(), pos.getY(), pos.getZ());
		//object->SetOrientation(rot.getX(), rot.getY(), rot.getZ(), rot.getW());

		
		//worldTrans.getOpenGLMatrix(object->_transform->GetOpenGLMatrix());
	}
};

#endif
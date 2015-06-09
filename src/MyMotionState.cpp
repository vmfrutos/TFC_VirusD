#include "MyMotionState.h"


MyMotionState::MyMotionState(const btTransform & initialPos, SceneNode * node)
{
	mVisibleObj = node;
	mTransform = initialPos;
	mCOM = btTransform::getIdentity();
}

MyMotionState::MyMotionState(const btTransform & initialPos)
{
	mVisibleObj = 0;
	mTransform = initialPos;
}

MyMotionState::~MyMotionState(){}

void
MyMotionState::setNode(SceneNode * node)
{
	mVisibleObj = node;
}

btTransform
MyMotionState::getWorldTransform() const
{
	return mCOM.inverse() * mTransform;
}

void
MyMotionState::getWorldTransform(btTransform & worldTrans) const
{
	worldTrans = mCOM.inverse() * mTransform;
}

void
MyMotionState::setWorldTransform(const btTransform & worldTrans)
{
	if (mVisibleObj == 0)
		return;

	mTransform = worldTrans;
	btTransform transform = mTransform * mCOM;
	btQuaternion rot = transform.getRotation();
	btVector3 pos = transform.getOrigin();
	mVisibleObj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	mVisibleObj->setPosition(pos.x(), pos.y(), pos.z());
}

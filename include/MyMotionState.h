#ifndef __MYMOTIONSTATE_H__
#define __MYMOTIONSTATE_H__

#include <Ogre.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>

using namespace Ogre;

class MyMotionState : public btDefaultMotionState
{
	protected:
		SceneNode * mVisibleObj;
		btTransform mTransform;
		btTransform mCOM;

	public:
		MyMotionState(const btTransform & initialPos, SceneNode * node);
		MyMotionState(const btTransform & initialPos);
		~MyMotionState();
		void setNode(SceneNode * node);
		btTransform getWorldTransform() const;
		void getWorldTransform(btTransform & worldTrans) const;
		void setWorldTransform(const btTransform & worldTrans);

};
#endif

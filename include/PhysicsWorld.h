#ifndef __PHYSICWORLD__H
#define __PHYSICWORLD__H

#include <Ogre.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include "MyMotionState.h"

#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"

using namespace Ogre;
class PhysicsWorld
{
	private:
		btAlignedObjectArray<btCollisionShape*> mCollisionShapes;
		btBroadphaseInterface * mBroadphase;
		btCollisionDispatcher * mDispatcher;
		btConstraintSolver * mSolver;
		btDefaultCollisionConfiguration * mColConfig;
		btDiscreteDynamicsWorld * mWorld;
		SceneNode * mRootSceneNode;



	public:
		PhysicsWorld(SceneManager* sceneManager);
		~PhysicsWorld();
		void addStaticPlane(SceneNode * node);
		void addStaticPlane2(SceneNode * node);
		void addTriangleMesh(SceneNode* node,Entity * ent,Vector3 initPosition);
		btRigidBody* addKinematicBody(SceneNode* node,Vector3 initPosition,btCollisionShape* capsule);
		btRigidBody * addDynamicBox(SceneNode * node, float m = 1.0f);
		btRigidBody * addRigidBody(btTransform transform, btCollisionShape * shape, btScalar mass, SceneNode * node = 0);
		void addCollisionShape(btCollisionShape * colShape);
		btDiscreteDynamicsWorld * getDynamicsWorld();
		btCollisionWorld * getCollisionWorld();
		btBroadphaseInterface * getBroadphase();
		void setRootSceneNode(SceneNode * node);
		btVector3 toBullet(const Vector3 & vec) const;
		void shootBox(const Vector3 & camPosition);
		static void debugBtVector3(const btVector3 & vec, const char * str = 0);
};
#endif

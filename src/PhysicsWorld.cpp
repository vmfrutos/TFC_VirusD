#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(SceneManager* sceneManager)
{

	mColConfig = new btDefaultCollisionConfiguration;
	mDispatcher = new btCollisionDispatcher(mColConfig);
	mBroadphase = new btDbvtBroadphase;
	mSolver = new btSequentialImpulseConstraintSolver;
	mWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mColConfig);

	mWorld->setGravity(btVector3(0, -10, 0));
	mRootSceneNode = 0;

}

PhysicsWorld::~PhysicsWorld()
{
	for (int i = mWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject * obj = mWorld->getCollisionObjectArray()[i];
		btRigidBody * body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
			delete body->getMotionState();

		mWorld->removeCollisionObject(obj);

		delete obj;
	}

	delete mWorld;
	delete mSolver;
	delete mBroadphase;
	delete mDispatcher;
	delete mColConfig;
}

void
PhysicsWorld::addStaticPlane(SceneNode * node)
{
	btCollisionShape * groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	mCollisionShapes.push_back(groundShape);
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, 0, 0));

	MyMotionState * motionState = new MyMotionState(groundTransform, node);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0, motionState, groundShape, btVector3(0, 0, 0));
	btRigidBody * body = new btRigidBody(rbInfo);

	mWorld->addRigidBody(body);
}

void
PhysicsWorld::addStaticPlane2(SceneNode * node)
{
	btCollisionShape * groundShape = new btBoxShape(btVector3(5, 0, 5));
	mCollisionShapes.push_back(groundShape);
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, 3, 0));

	MyMotionState * motionState = new MyMotionState(groundTransform, node);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0, motionState, groundShape, btVector3(0, 0, 0));
	btRigidBody * body = new btRigidBody(rbInfo);

	mWorld->addRigidBody(body);
}

void PhysicsWorld::addTriangleMesh(SceneNode* node,Entity * ent,Vector3 initPosition){
	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new OgreBulletCollisions::StaticMeshToShapeConverter(ent);
	OgreBulletCollisions::TriangleMeshCollisionShape *colisionShape = trimeshConverter->createTrimesh();



	btCollisionShape* triangleMeshShape = colisionShape->getBulletShape();
	delete trimeshConverter;

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(initPosition.x,initPosition.y,initPosition.z));
	MyMotionState * motionState = new MyMotionState(transform, node);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0, motionState, triangleMeshShape, btVector3(0, 0, 0));
	btRigidBody * body = new btRigidBody(rbInfo);
	mWorld->addRigidBody(body);

}

btRigidBody* PhysicsWorld::addKinematicBody(SceneNode* node,Vector3 initPosition,btCollisionShape* capsule){


	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(initPosition.x,initPosition.y,initPosition.z));
	MyMotionState * motionState = new MyMotionState(transform, node);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(50, motionState, capsule, btVector3(initPosition.x,initPosition.y,initPosition.z));
	btRigidBody * body = new btRigidBody(rbInfo);
	body->setAngularFactor(btVector3(0, 1, 0));
	//body->setCollisionFlags( body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	//body->setActivationState( DISABLE_DEACTIVATION );
	mWorld->addRigidBody(body);
	return body;

}

btRigidBody *
PhysicsWorld::addDynamicBox(SceneNode * node, float m)
{
	btCollisionShape * colShape = new btBoxShape(btVector3(1, 1, 1));
	mCollisionShapes.push_back(colShape);
	btTransform boxTransform;
	boxTransform.setIdentity();

	btScalar mass(m);
	btVector3 localInertia(0, 0, 0);

	colShape->calculateLocalInertia(mass, localInertia);

	boxTransform.setOrigin(btVector3(node->getPosition().x, node->getPosition().y, node->getPosition().z));

	MyMotionState * motionState = new MyMotionState(boxTransform, node);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
	btRigidBody * body = new btRigidBody(rbInfo);

	mWorld->addRigidBody(body);

	return body;
}

btRigidBody *
PhysicsWorld::addRigidBody(btTransform transform, btCollisionShape * shape, btScalar mass, SceneNode * node)
{
	mCollisionShapes.push_back(shape);
	btVector3 localInertia(0, 0, 0);

	shape->calculateLocalInertia(mass, localInertia);
	MyMotionState * motionState = new MyMotionState(transform, node);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody * body = new btRigidBody(rbInfo);

	mWorld->addRigidBody(body);

	return body;
}


void
PhysicsWorld::addCollisionShape(btCollisionShape * colShape)
{
	mCollisionShapes.push_back(colShape);
}

btDiscreteDynamicsWorld *
PhysicsWorld::getDynamicsWorld()
{
	return mWorld;
}

btCollisionWorld *
PhysicsWorld::getCollisionWorld()
{
	return mWorld->getCollisionWorld();
}

btBroadphaseInterface *
PhysicsWorld::getBroadphase()
{
	return mBroadphase;
}

void
PhysicsWorld::setRootSceneNode(SceneNode * node)
{
	mRootSceneNode = node;
}

btVector3
PhysicsWorld::toBullet(const Vector3 & vec) const
{
	return btVector3(vec.x, vec.y, vec.z);
}

void
PhysicsWorld::shootBox(const Vector3 & camPosition)
{
	if (mRootSceneNode)
	{
		SceneNode * node = mRootSceneNode->createChildSceneNode(camPosition);
		btRigidBody * box = addDynamicBox(node);
		box->applyCentralImpulse(btVector3(50, 0, 0));
	}
}

void
PhysicsWorld::debugBtVector3(const btVector3 & vec, const char * str)
{
	std::cout << str << " x: " << vec.x() << "; y: " << vec.y() << "; z: " << vec.z() << std::endl;
}

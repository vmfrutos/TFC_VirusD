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


void PhysicsWorld::addTriangleMesh(SceneNode* node,Entity * ent,Vector3 initPosition){
	node->setPosition(initPosition);

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
	body->setFriction(100000);

	string name = node->getName();
	mRigidBodies[name] = body;

	mWorld->addRigidBody(body);

}

void PhysicsWorld::addDinamicBody(SceneNode* node,Entity * ent,Vector3 initPosition,float mass,Vector3 collisionBox){
	node->setPosition(initPosition);

	btCollisionShape * colShape = new btBoxShape(btVector3(collisionBox.x,collisionBox.y, collisionBox.z));
	mCollisionShapes.push_back(colShape);


	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(initPosition.x,initPosition.y,initPosition.z));

	btRigidBody * body = addRigidBody(transform,colShape,mass,node);

	string name = node->getName();
	mRigidBodies[name] = body;

	mWorld->addRigidBody(body);

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
	body->setFriction(100000);

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
PhysicsWorld::debugBtVector3(const btVector3 & vec, const char * str)
{
	std::cout << str << " x: " << vec.x() << "; y: " << vec.y() << "; z: " << vec.z() << std::endl;
}


void
PhysicsWorld::deleteRigidBody(string name){

	btRigidBody *rBody = mRigidBodies[name];


	if (rBody->getMotionState())
	{
		delete rBody->getMotionState();
	}
	mBroadphase->getOverlappingPairCache()->cleanProxyFromPairs(rBody->getBroadphaseHandle(),mWorld->getDispatcher() );
	mWorld->removeRigidBody(rBody);
	delete rBody;

	mRigidBodies.erase(name);

}


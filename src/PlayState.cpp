#include "PlayState.h"
#include "PauseState.h"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter ()
{

	_root = Ogre::Root::getSingletonPtr();

	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManagerPlay");

	_camera = _sceneMgr->createCamera("PlayCamera");

	_camera->setPosition(0,2,-5);

	_camera->setFOVy (Ogre::Degree(50));

	_camera->setNearClipDistance(1);

	_camera->setFarClipDistance(3000);

	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

	// Nuevo background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));

	// Se inicializa bullet
	_physicsWorld = new PhysicsWorld(_sceneMgr);

	// Se construye un debugDrawer
	_debugDrawer = new DebugDrawer(_sceneMgr->getRootSceneNode(), _physicsWorld->getDynamicsWorld());

	// Se asocia el debugDrawer al mundo físico
	_physicsWorld->getDynamicsWorld()->setDebugDrawer(_debugDrawer);

	createScene();

	_exitGame = false;
}

void
PlayState::exit ()
{
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void
PlayState::pause()
{
}

void
PlayState::resume()
{
	// Se restaura el background colour.
	//_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{

	_physicsWorld->getDynamicsWorld()->stepSimulation(1 / 100.f, 10);


	_debugDrawer->step();



	return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{

	if (_exitGame)
		return false;

	return true;
}

bool
PlayState::frameRenderingQueued
(const Ogre::FrameEvent& evt)
{

	_character->updateCharacter(evt.timeSinceLastFrame);

	for (unsigned int i=0; i< _enemies.size();i++){
		_enemies[i]->updateCharacter(evt.timeSinceLastFrame);
	}

	return true;
}


void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
	// Tecla p --> PauseState.
	if (e.key == OIS::KC_P) {
		pushState(PauseState::getSingletonPtr());
	}

	if (e.key == OIS::KC_1) {
		_debugDrawer->setDebugMode(true);
	}

	if (e.key == OIS::KC_2) {
		_debugDrawer->setDebugMode(false);
	}

	_character->injectKeyDown(e);


}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}


	_character->injectKeyUp(e);


}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
	_character->injectMouseMove(e);
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	if (e.state.buttonDown(OIS::MB_Left))
		_physicsWorld->shootBox(_camera->getPosition());
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PlayState*
PlayState::getSingletonPtr ()
{
	return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{ 
	assert(msSingleton);
	return *msSingleton;
}

void PlayState::createScene()
{
	Light * light = _sceneMgr->createLight("Light");
	light->setType(Light::LT_DIRECTIONAL);
	light->setDirection(Vector3(1, -1, 0));



	// Creacion de la entidad y del SceneNode del plano ------------------------
	SceneNode* groundNode = _sceneMgr->createSceneNode("ground");
	Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "Plane.mesh");
	groundNode->attachObject(groundEnt);
	Vector3 initPos(0,0,0);
	_sceneMgr->getRootSceneNode()->addChild(groundNode);
	_physicsWorld->addTriangleMesh(groundNode,groundEnt,initPos);


	/// -- Protagonista

	btVector3 pos(5.0, 10.0,5.0);
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(pos);
	Vector3 origin(pos.getX(), pos.getY(), pos.getZ());

	btPairCachingGhostObject * characterGhostObject = new btPairCachingGhostObject();
	characterGhostObject->setWorldTransform(startTransform);

	btScalar characterHeight = 1.8f;
	btScalar characterWidth = 0.2f;

	btConvexShape * capsule = new btCapsuleShape(characterWidth, characterHeight);


	_physicsWorld->addCollisionShape(capsule);
	characterGhostObject->setCollisionShape(capsule);

	characterGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);


	// duck setup
	btConvexShape * duck = new btCapsuleShape(characterWidth, characterHeight / 3);
	_physicsWorld->addCollisionShape(duck);

	btScalar stepHeight = 0.35f;

	_character = new Hero("protagonista","protagonista.mesh",_sceneMgr, _camera, characterGhostObject, capsule, stepHeight, _physicsWorld->getCollisionWorld(), origin);

	_physicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	_physicsWorld->getDynamicsWorld()->addCollisionObject(characterGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	_physicsWorld->getDynamicsWorld()->addAction(_character->getCCPhysics());


	/// -- Enemigos

	for (int i=0; i< 1;i++) {
		btVector3 posEnemy;
		if (i==0)
			posEnemy.setValue(5.0, 3.0,6.0);
		if (i==1)
			posEnemy.setValue(8,3.0,9.0);
		if (i==2)
			posEnemy.setValue(2,3,1);

		btTransform startTransformEnemy;
		startTransformEnemy.setIdentity();
		startTransformEnemy.setOrigin(posEnemy);

		Vector3 originEnemy(posEnemy.getX(), posEnemy.getY(), posEnemy.getZ());


		btPairCachingGhostObject * characterNPCGhostObject = new btPairCachingGhostObject();
		characterNPCGhostObject->setWorldTransform(startTransformEnemy);

		btScalar characterHeight = 1.8f;
		btScalar characterWidth = 0.2f;
		btConvexShape * capsuleNPC = new btCapsuleShape(characterWidth, characterHeight);


		_physicsWorld->addCollisionShape(capsuleNPC);
		characterNPCGhostObject->setCollisionShape(capsuleNPC);

		characterNPCGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		btConvexShape * duckEnemy = new btCapsuleShape(characterWidth, characterHeight / 3);
		_physicsWorld->addCollisionShape(duckEnemy);

		Enemy* enemy = new Enemy("enemy_" + StringConverter::toString(i),"zombi_01.mesh",_sceneMgr, characterNPCGhostObject, capsuleNPC, stepHeight, _physicsWorld->getCollisionWorld(), originEnemy);
		_enemies.push_back(enemy);
		//_character2 = new CharacterControllerManager("enemy","protagonista.mesh",_sceneMgr, _camera, characterNPCGhostObject, capsuleNPC, stepHeight, _physicsWorld->getCollisionWorld(), originEnemy);

		_physicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		_physicsWorld->getDynamicsWorld()->addCollisionObject(characterNPCGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
		_physicsWorld->getDynamicsWorld()->addAction(enemy->getCCPhysics());

	}
	_physicsWorld->setRootSceneNode(_sceneMgr->getRootSceneNode());
}

Hero* PlayState::getHero(){
	return _character;
}
std::vector<Enemy*> PlayState::getEnemies(){
	return _enemies;
}



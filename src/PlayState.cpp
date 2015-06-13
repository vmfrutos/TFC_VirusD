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

	_camera->setFarClipDistance(450);

	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

	// Nuevo background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));

	// Se inicializa bullet
	_physicsWorld = new PhysicsWorld(_sceneMgr);

	// Se construye un debugDrawer
	_debugDrawer = new DebugDrawer(_sceneMgr->getRootSceneNode(), _physicsWorld->getDynamicsWorld());
	_debugDrawer->setDebugMode(false);

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
	_root->destroySceneManager(_sceneMgr);

	delete _character;
	for (unsigned int i=0; i< _enemies.size();i++){
		delete _enemies[i];
	}
	_enemies.clear();
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

	if (_exitGame){
		changeState(IntroState::getSingletonPtr());
	}

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

	Vector3 initPos(0,0,0);

	// Creacion de la entidad y del SceneNode del plano ------------------------
	SceneNode* groundNode = _sceneMgr->createSceneNode("ground");
	Entity* groundEnt = _sceneMgr->createEntity("ground", "Plane.mesh");
	groundNode->attachObject(groundEnt);
	_sceneMgr->getRootSceneNode()->addChild(groundNode);
	_physicsWorld->addTriangleMesh(groundNode,groundEnt,initPos);

	SceneNode* roadNode = _sceneMgr->createSceneNode("road");
	Entity* roadEnt = _sceneMgr->createEntity("road", "Road.mesh");
	roadNode->attachObject(roadEnt);
	_sceneMgr->getRootSceneNode()->addChild(roadNode);
	_physicsWorld->addTriangleMesh(roadNode,roadEnt,initPos);

	SceneNode* wallNode = _sceneMgr->createSceneNode("wall");
	Entity* wallEnt = _sceneMgr->createEntity("wall", "Muro.mesh");
	wallNode->attachObject(wallEnt);
	_sceneMgr->getRootSceneNode()->addChild(wallNode);
	_physicsWorld->addTriangleMesh(wallNode,wallEnt,initPos);


	/// -- Protagonista
	Vector3 origin = Properties::getSingletonPtr()->getPropertyVector("hero.position");
	btVector3 btOrigin(origin.x,origin.y,origin.z);
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btOrigin);

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
	int numEmenigos = Properties::getSingletonPtr()->getPropertyInt("enemy.number");

	for (int i=0; i< numEmenigos;i++) {
		Vector3 originEnemy = Properties::getSingletonPtr()->getPropertyVector("enemy.position." + StringConverter::toString(i+1));
		btVector3 btOriginEnemy(originEnemy.x,originEnemy.y,originEnemy.z);

		btTransform startTransformEnemy;
		startTransformEnemy.setIdentity();
		startTransformEnemy.setOrigin(btOriginEnemy);


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

	// Se añaden los sectores

	// Sector 6
	SceneNode* sector06Node = _sceneMgr->createSceneNode("sector06");
	Entity* sector06Ent = _sceneMgr->createEntity("sector06", "Sector_06.mesh");
	sector06Node->attachObject(sector06Ent);
	_sceneMgr->getRootSceneNode()->addChild(sector06Node);
	_physicsWorld->addTriangleMesh(sector06Node,sector06Ent,initPos);



	_physicsWorld->setRootSceneNode(_sceneMgr->getRootSceneNode());
}

Hero* PlayState::getHero(){
	return _character;
}
std::vector<Enemy*> PlayState::getEnemies(){
	return _enemies;
}



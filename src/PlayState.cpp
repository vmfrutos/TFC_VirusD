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
	//_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);

	if (Properties::getSingletonPtr()->getPropertyBool("set.fog")){
		_viewport->setBackgroundColour(fadeColour);
		_sceneMgr->setFog(Ogre::FOG_EXP, fadeColour, 0.0018, 0.5, 2000);
	}

	// Se inicializa bullet
	_physicsWorld = new PhysicsWorld(_sceneMgr);

	// Se construye un debugDrawer
	_debugDrawer = new DebugDrawer(_sceneMgr->getRootSceneNode(), _physicsWorld->getDynamicsWorld());
	_debugDrawer->setDebugMode(false);

	// Se asocia el debugDrawer al mundo físico
	_physicsWorld->getDynamicsWorld()->setDebugDrawer(_debugDrawer);

	createScene();

	_exitGame = false;
	_gameOver = false;
	_gameCompleted = false;
	_timeOver  = false;

	_hud = new Hud();
	_hud->resetTime(Properties::getSingletonPtr()->getPropertyInt("time.to.end"));

	// Se carga la pista de audio
	_audio = GameSound::getSingletonPtr()->loadTrack("principal.mp3");

	// Se pone en modo loop
	_audio->play(true);

	srand (time(NULL));
}

void
PlayState::exit ()
{
	_audio->stop();
	delete _hud;
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
	_root->destroySceneManager(_sceneMgr);

	delete _character;
	for (unsigned int i=0; i< _enemies.size();i++){
		delete _enemies[i];
	}
	_enemies.clear();
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));


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
	float fps;
	float deltaT = evt.timeSinceLastFrame;
	if (deltaT == 0.0) {
		fps = 1000; // esto es por evitar división por 0 en equipos muy rápidos
	} else {
		fps = 1.0f / deltaT;
	}

	_physicsWorld->getDynamicsWorld()->stepSimulation(1 / 100.f, 10);
	//_physicsWorld->getDynamicsWorld()->stepSimulation(deltaT);


	_debugDrawer->step();

	_timeOver = _hud->update(deltaT,fps);

	_gameOver = _character->collisionWithEnemy(_enemies);

	_gameCompleted = _character->isGameCompleted();



	return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{

	if (_exitGame){
		changeState(IntroState::getSingletonPtr());
	}

	if (_gameOver || _timeOver){
		changeState(GameOverState::getSingletonPtr());
	}

	if (_gameCompleted){
		changeState(CompletedState::getSingletonPtr());
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

	if (e.key == OIS::KC_3) {
		_hud->setVisibleFPS(true);
	}

	if (e.key == OIS::KC_4) {
		_hud->setVisibleFPS(false);
	}

	if (e.key == OIS::KC_Q) {
		_character->checkObject();
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
	setLights();

	Vector3 initPos(0,0,0);

	// Creacion de la entidad y del SceneNode del plano ------------------------
	SceneNode* groundNode = _sceneMgr->createSceneNode("ground");
	Entity* groundEnt = _sceneMgr->createEntity("ground", "Plane.mesh");
	groundEnt->setCastShadows(false);
	groundNode->attachObject(groundEnt);
	_sceneMgr->getRootSceneNode()->addChild(groundNode);
	_physicsWorld->addTriangleMesh(groundNode,groundEnt,initPos);

	SceneNode* roadNode = _sceneMgr->createSceneNode("road");
	Entity* roadEnt = _sceneMgr->createEntity("road", "Road.mesh");
	roadEnt->setCastShadows(false);
	roadNode->attachObject(roadEnt);
	_sceneMgr->getRootSceneNode()->addChild(roadNode);
	_physicsWorld->addTriangleMesh(roadNode,roadEnt,initPos);

	SceneNode* wallNode = _sceneMgr->createSceneNode("wall");
	Entity* wallEnt = _sceneMgr->createEntity("wall", "Muro.mesh");
	wallEnt->setCastShadows(false);
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

	Vector3 cylinderSize = Properties::getSingletonPtr()->getPropertyVector("hero.collisionShape");
	btCylinderShape * capsule = new btCylinderShape(btVector3(cylinderSize.x,cylinderSize.y,cylinderSize.z));

	//btScalar characterHeight = 1.7f;
	//btScalar characterWidth = 0.2f;
	//btConvexShape *capsule = new btCapsuleShape(characterWidth, characterHeight);



	_physicsWorld->addCollisionShape(capsule);
	characterGhostObject->setCollisionShape(capsule);

	characterGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	// duck setup
	//btConvexShape * duck = new btCapsuleShape(characterWidth, characterHeight / 3);
	//_physicsWorld->addCollisionShape(duck);


	btScalar stepHeight = 0.15f;

	_character = new Hero("protagonista","protagonista.mesh",_sceneMgr, _camera, characterGhostObject, capsule, stepHeight, _physicsWorld->getCollisionWorld(), origin);

	_physicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	_physicsWorld->getDynamicsWorld()->addCollisionObject(characterGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	_physicsWorld->getDynamicsWorld()->addAction(_character->getCCPhysics());


	/// -- Enemigos
	int numEmenigos = Properties::getSingletonPtr()->getPropertyInt("enemy.number");

	for (int i=0; i< numEmenigos;i++) {
		srand (time(NULL)*i);

		float x;
		float z;

		// Primero se decide aleatoriamente en cual de las posicion de la x
		int signo = rand() % 2;
		x = (float)((rand() % 190)+1); // entre 1 y 190
		if (signo == 0){
			x = -x;
		}

		// La posicion de la z es una de las 7 posibles
		int altura = rand() % 7;

		switch(altura){
		case 0:
			z = -190;
			break;

		case 1:
			z = -105;
			break;

		case 2:
			z = -50;
			break;

		case 3:
			z = 0;
			break;

		case 4:
			z = 60;
			break;
		case 5:
			z = 115;
			break;

		case 6:
			z = 170;
			break;


		}




		Vector3 originEnemy = Vector3(x,1.0,z);
		btVector3 btOriginEnemy(originEnemy.x,originEnemy.y,originEnemy.z);

		btTransform startTransformEnemy;
		startTransformEnemy.setIdentity();
		startTransformEnemy.setOrigin(btOriginEnemy);


		btPairCachingGhostObject * characterNPCGhostObject = new btPairCachingGhostObject();
		characterNPCGhostObject->setWorldTransform(startTransformEnemy);

		//btScalar characterHeight = 1.7f;
		//btScalar characterWidth = 0.2f;
		//btConvexShape * capsuleNPC = new btCapsuleShape(characterWidth, characterHeight);
		Vector3 cylinderSize = Properties::getSingletonPtr()->getPropertyVector("hero.collisionShape");
		btCylinderShape * capsuleNPC = new btCylinderShape(btVector3(cylinderSize.x,cylinderSize.y,cylinderSize.z));


		_physicsWorld->addCollisionShape(capsuleNPC);
		characterNPCGhostObject->setCollisionShape(capsuleNPC);

		characterNPCGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		//btConvexShape * duckEnemy = new btCapsuleShape(characterWidth, characterHeight / 3);
		//_physicsWorld->addCollisionShape(duckEnemy);

		Enemy* enemy = new Enemy("enemy_" + StringConverter::toString(i),"zombi_01.mesh",_sceneMgr, characterNPCGhostObject, capsuleNPC, stepHeight, _physicsWorld->getCollisionWorld(), originEnemy,i);
		_enemies.push_back(enemy);
		//_character2 = new CharacterControllerManager("enemy","protagonista.mesh",_sceneMgr, _camera, characterNPCGhostObject, capsuleNPC, stepHeight, _physicsWorld->getCollisionWorld(), originEnemy);

		_physicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		_physicsWorld->getDynamicsWorld()->addCollisionObject(characterNPCGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
		_physicsWorld->getDynamicsWorld()->addAction(enemy->getCCPhysics());



	}

	// Se añaden los sectores

	// Sector 1
	addObject("Casa01","House01.mesh",Vector3(-137.857,0.0,-130.391));

	// Sector 2
	addObject("Casa02","House03.mesh",Vector3(-45.7608,0.0,-130.3228));

	// Sector 3
	addObject("Casa03","House02.mesh",Vector3(43.2096,0.0,-129.1185));

	// Sector 4
	addObject("Casa04","House03.mesh",Vector3(134.0007,0.0,-130.2476));

	// Sector 5
	addObject("Park05","Park.mesh",Vector3(-135.7642,0.0,-76.5040));

	// Sector 6
	addObject("Gasolinera06","Sector_06.mesh",Vector3(0,0,0));

	// Sector 7
	addObject("Park07","Park.mesh",Vector3(43.9944,0.0,-76.5040));

	// Sector 8
	addObject("Casa08","House03.mesh",Vector3(133.9058,0.0,-76.38033));


	// Sector 9
	addObject("Casa09","House01.mesh",Vector3(-137.857,0.0,-22.497));

	// Sector 10
	addObject("Casa10","House03.mesh",Vector3(-45.9804,0.0,-22.2687));

	// Sector 11
	addObject("Casa11","House01.mesh",Vector3(42.041,0.0,-22.497));


	// Sector 12
	addObject("Casa12","House02.mesh",Vector3(133.0252,0.0,-21.2330));

	// Sector 13
	addObject("Park13","Park.mesh",Vector3(-135.56681,0.0,31.08109));

	// Sector 14
	addObject("Casa14","House02.mesh",Vector3(-46.7383,0.0,32.5588));

	// Sector 15
	addObject("Casa15","House03.mesh",Vector3(44.1694,0.0,31.4682));

	// Sector 16
	addObject("Casa16","House01.mesh",Vector3(132.0900,0.0,31.7164));

	// Sector 17
	addObject("Park17","Park.mesh",Vector3(-135.7950,0.0,85.1771));

	// Sector 18
	addObject("Park18","Park.mesh",Vector3(-45.8349,0.0,85.2130));

	// Sector 19
	addObject("Casa19","House01.mesh",Vector3(42.3032,0.0,84.9532));

	// Sector 20
	addObject("Casa20","House03.mesh",Vector3(133.9341,0.0,85.3163));

	// Sector 21
	addObject("Casa21","House02.mesh",Vector3(-136.8057,0.0,140.4290));

	// Sector 22
	addObject("Casa22","House03.mesh",Vector3(-45.8687,0.0,139.3265));

	// Sector 23
	addObject("Casa23","House01.mesh",Vector3(42.05486,0.0,139.18379));

	// Sector 24
	addObject("Park24","Park.mesh",Vector3(134.1105,0.0,139.0234));



	int numPosiciones = Properties::getSingletonPtr()->getPropertyInt("items.num.positions");

	int posKey = (rand() % numPosiciones)+1;



	// Llave
	addObjectDinamic("Key","key.mesh",Properties::getSingletonPtr()->getPropertyVector("item.position." + StringConverter::toString(posKey)),0.5,Vector3(0.50,0.34,0.94));

	// Se cambia la inicialización de la semilla
	srand (time(NULL)*posKey);

	int posCanGas;
	do {
		posCanGas = (rand() % numPosiciones)+1;

	} while (posKey == posCanGas); // No se deben posicionar los dos en el mismo sitio


	// Bidon de combustible
	addObjectDinamic("CanGas","CanGas.mesh",Properties::getSingletonPtr()->getPropertyVector("item.position." + StringConverter::toString(posCanGas)),0.5,Vector3(0.50,0.34,0.94));

	// Coche para la huida
	addObjectDinamic("Car","Car.mesh",Properties::getSingletonPtr()->getPropertyVector("car.position"),30000000000,Vector3(0.9,0.8,1.7));




	_physicsWorld->setRootSceneNode(_sceneMgr->getRootSceneNode());
}

Hero* PlayState::getHero(){
	return _character;
}
std::vector<Enemy*> PlayState::getEnemies(){
	return _enemies;
}

Hud* PlayState::getHud(){
	return _hud;
}

SceneManager* PlayState::getSceneManager(){
	return _sceneMgr;
}

PhysicsWorld* PlayState::getPhysicsWorld(){
	return _physicsWorld;
}

void PlayState::setLights(){

	//_sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	_sceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
	_sceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	_sceneMgr->setShadowTextureCount(1);
	_sceneMgr->setShadowTextureSize(16384);



	Ogre::Light* light = _sceneMgr->createLight("Light1");
	light->setDirection(0.0,0.0,0.0);
	light->setDiffuseColour(0.3,0.3,0.3);
	light->setDirection(0,0,0);
	light->setPosition(0,100,0);

	light->setType(Ogre::Light::LT_POINT);

	light->setCastShadows(true);
	light->setShadowNearClipDistance(1);
	light->setShadowFarClipDistance(400);


	_sceneMgr->setSkyDome(true, "SpaceSky", 8, 8,300);



}

void PlayState::addObject(string name,string mesh,Vector3 position){
	/*
	SceneNode* node = _sceneMgr->createSceneNode(name);
	Entity* entity = _sceneMgr->createEntity(name, mesh);
	entity->setCastShadows(false);
	node->attachObject(entity);
	_sceneMgr->getRootSceneNode()->addChild(node);
	_physicsWorld->addTriangleMesh(node,entity,position);
	 */

	StaticGeometry* stage = _sceneMgr->createStaticGeometry(name);
	Entity* ent1 = _sceneMgr->createEntity(mesh);
	stage->addEntity(ent1, position);
	stage->build();  // Operacion para construir la geometria
	_physicsWorld->addTriangleMesh( _sceneMgr->createSceneNode(name),ent1,position);
}

void PlayState::addObjectDinamic(string name,string mesh,Vector3 position,float mass,Vector3 collisionBox){
	SceneNode* node = _sceneMgr->createSceneNode(name);
	Entity* entity = _sceneMgr->createEntity(name, mesh);
	entity->setCastShadows(false);
	node->attachObject(entity);
	_sceneMgr->getRootSceneNode()->addChild(node);
	_physicsWorld->addDinamicBody(node,entity,position,mass,collisionBox);
}

void  PlayState::deleteObject(string name){


	// Se elimina del mundo fisico
	//_physicsWorld->deleteRigidBody(name);
	//_sceneMgr->getSceneNode(name)->getAttachedObject(name)->setVisible(false);

	// Se oculta del mundo visual
	_sceneMgr->getSceneNode(name)->getAttachedObject(name)->setVisible(false);



}


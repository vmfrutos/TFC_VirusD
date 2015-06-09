#include "Enemy.h"

Enemy::Enemy(String name, String mesh,SceneManager* sceneMgr, btPairCachingGhostObject * ghostObject, btConvexShape * convexShape, btScalar stepHeight, btCollisionWorld * collisionWorld,Vector3 & origin, int upAxis){

	_CCNPC = new CharacterControllerNPC(name,mesh,sceneMgr, origin);

	_CCPhysics = new MyKinematicCharacterController(ghostObject, convexShape, stepHeight, btVector3(origin.x,origin.y,origin.z), upAxis);

	_sceneManager = sceneMgr;

	_bodyNode = _CCNPC->getBodySceneNode();


	mIsFalling = _CCPhysics->onGround();
	_walkDirection = Vector3::ZERO;
	_keyDirection = Vector3::ZERO;
	mJumped = false;
	mRun = false;

	srand (time(NULL));
}

CharacterControllerNPC * Enemy::getCCNPC(){
	return _CCNPC;
}
MyKinematicCharacterController * Enemy::getCCPhysics(){
	return _CCPhysics;
}


void Enemy::updateCharacter(Real deltaTime)
{
	Real direction;
	if (mRun) {
		direction = RUN_SPEED * deltaTime;
	} else {
		direction = WALK_SPEED * deltaTime;
	}

	Vector3 NPC_Pos = _CCNPC->getPosition();

	//btVector3 pos = mCCPhysics->getPosition();
	btVector3 pos = _CCPhysics->getGhostObject()->getWorldTransform().getOrigin();



	Vector3 position(pos.x(), pos.y(), pos.z());


	if (position != NPC_Pos)
	{
		_bodyNode->translate((position - NPC_Pos) * direction);
	}

	_goalDirection = Vector3::ZERO;   // we will calculate this

	if (_keyDirection != Vector3::ZERO)
	{
		// calculate actuall goal direction in world based on player's key directions
		//mGoalDirection += _keyDirection.z * mCameraNode->getOrientation().zAxis();
		//mGoalDirection += _keyDirection.x * mCameraNode->getOrientation().xAxis();
		_goalDirection = _keyDirection;
		_goalDirection.y = 0;
		_goalDirection.normalise();

		Quaternion toGoal = _bodyNode->getOrientation().zAxis().getRotationTo(_goalDirection);

		// calculate how much the character has to turn to face goal direction
		Real yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;

		// turn as much as we can, but not more than we need to
		if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
		else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));

		_bodyNode->yaw(Degree(yawToGoal));

		btVector3 walkDirection(_goalDirection.x * direction, _goalDirection.y * direction, _goalDirection.z * direction);

		_CCPhysics->setWalkDirection(walkDirection);
		_CCNPC->setIsMoving(true);
	}
	else
	{

		btVector3 walkDirection(0,0,0);
		_CCPhysics->setWalkDirection(walkDirection);
		_CCNPC->setIsMoving(false);
	}
	_CCNPC->addTime(deltaTime);
	IA();
}

void Enemy::walk(Direction direction){

	if ((direction == X_POSITIVE) || (direction == X_NEGATIVE) ||(direction == Z_POSITIVE) || (direction == Z_NEGATIVE))
	{

		if (_keyDirection.isZeroLength()){
			_CCNPC->animWalk01Start();
		}
	}

	if (direction == Z_NEGATIVE) {
		_keyDirection.z = -1;
	} else if (direction == Z_POSITIVE){
		_keyDirection.z = 1;
	} else if (direction == X_NEGATIVE){
		_keyDirection.x = -1;
	} else if (direction == X_POSITIVE){
		_keyDirection.x = 1;
	}
}

void Enemy::stop(){

	_keyDirection.z = 0;

	_keyDirection.x = 0;

	_CCNPC->animWalk01End();

}

void Enemy::IA(){
	// De momento simplemente siguen al personaje principal en linea recta

	// un 30% de las veces se mueve d eforma aleatoria para evitar que todos los enemigos hagan lo mismo

	int val = rand() % 10;
	if (val <= 3){
		moveRandom();
	} else {

		// Se recupera la posicion del personaje principal
		Hero* hero = PlayState::getSingletonPtr()->getHero();
		Vector3 posHero = hero->getCCPlayer()->getPosition();
		Vector3 posEnemy = _CCNPC->getPosition();
		if (posHero.x > posEnemy.x){
			walk(X_POSITIVE);
		} else if (posHero.x < posEnemy.x){
			walk(X_NEGATIVE);
		}

		if (posHero.z > posEnemy.z){
			walk(Z_POSITIVE);
		} else if (posHero.z < posEnemy.z){
			walk(Z_NEGATIVE);
		}
	}
}

void Enemy::moveRandom(){

	int val = rand() % 4;
	switch(val) {
	case 0: walk(X_POSITIVE);
	break;
	case 1: walk(X_NEGATIVE);
	break;
	case 2: walk(Z_POSITIVE);
	break;
	case 3: walk(Z_NEGATIVE);
	break;

	}
}


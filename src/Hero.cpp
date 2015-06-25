#include "Hero.h"

Hero::Hero(String name, String mesh, SceneManager * scnMgr, Camera * cam, btPairCachingGhostObject * ghostObject,
														btConvexShape * convexShape, btScalar stepHeight, btCollisionWorld * collisionWorld,
														Vector3 & origin, int upAxis
														)
{
	mCCPlayer = new CharacterControllerPlayer(name,mesh,cam->getSceneManager(), origin);
	mCCPhysics = new MyKinematicCharacterController(ghostObject, convexShape, stepHeight, btVector3(origin.x,origin.y,origin.z), upAxis);

	mSceneManager = scnMgr;

	mBodyNode = mCCPlayer->getBodySceneNode();

	setupCamera(cam);

	cameraHeight = Properties::getSingletonPtr()->getPropertyFloat("hero.camera.heigh");

	mIsFalling = mCCPhysics->onGround();
	mWalkDirection = Vector3::ZERO;
	mKeyDirection = Vector3::ZERO;
	mJumped = false;
	mRun = false;
}

Hero::Hero(SceneManager * scnMgr, CharacterControllerPlayer * ccPlayer, MyKinematicCharacterController * ccPhysics
														)
{
	mCCPlayer = ccPlayer;
	mCCPhysics = ccPhysics;

	mSceneManager = scnMgr;
	mJumped = false;
	mRun = false;

}

CharacterControllerPlayer* Hero::getCCPlayer()
{
	return mCCPlayer;
}
MyKinematicCharacterController* Hero::getCCPhysics()
{
	return mCCPhysics;
}


void Hero::injectKeyDown(const OIS::KeyEvent & evt)
{
	if (((evt.key == OIS::KC_SPACE) ||(evt.key == OIS::KC_W) || (evt.key == OIS::KC_S) || (evt.key == OIS::KC_A) || (evt.key == OIS::KC_D)) && mCCPhysics->onGround())
	{

		if (evt.key == OIS::KC_SPACE) {
			if (mRun == false) {
				mRun = true;
				mCCPlayer->animRunStart();
			}
		} else if (mKeyDirection.isZeroLength()){
			mCCPlayer->animWalkStart();
		}
	}

	if (evt.key == OIS::KC_W) {
		mKeyDirection.z = -1;
	} else if (evt.key == OIS::KC_S){
		mKeyDirection.z = 1;
	} else if (evt.key == OIS::KC_A){
		mKeyDirection.x = -1;
	} else if (evt.key == OIS::KC_D){
		mKeyDirection.x = 1;
	}

	/*
	switch (evt.key)
	{
		case OIS::KC_W:
			mKeyDirection.z = -1;
			break;

		case OIS::KC_S:
			mKeyDirection.z = 1;
			break;

		case OIS::KC_A:
			mKeyDirection.x = -1;
			break;

		case OIS::KC_D:
			mKeyDirection.x = 1;
			break;

		//Todo; Mirar los saltos

		case OIS::KC_SPACE:
			if (mCCPhysics->canJump())
			{
				mCCPhysics->jump();
				mCCPlayer->animJumpStart();

				mJumped = true;
			}

			break;
		 */
		/*case OIS::KC_LCONTROL:
			mCCPhysics->duck();
			break;

	}*/
}

void Hero::injectKeyUp(const OIS::KeyEvent & evt)
{


	if (evt.key == OIS::KC_W || evt.key == OIS::KC_S){
		mKeyDirection.z = 0;
	} else if (evt.key == OIS::KC_A || evt.key == OIS::KC_D){
		mKeyDirection.x = 0;
	}

	/*
	switch (evt.key)
	{
		case OIS::KC_W:
		case OIS::KC_S:
			mKeyDirection.z = 0;
			break;

		case OIS::KC_A:
		case OIS::KC_D:
			mKeyDirection.x = 0;
			break;


		case OIS::KC_LCONTROL:
			mCCPhysics->stand();
			break;

	}
	*/

	// pretty much the same case as above (the !mJumped if statement) - if we jumped while holding w/s/a/d and then released
	// it, it really wouldn't look very good if the ANIM_JUMP_LOOP would be replaced by ANIM_IDLE_TOP and ANIM_IDLE_BASE

	if (((evt.key == OIS::KC_SPACE) ||(evt.key == OIS::KC_W) || (evt.key == OIS::KC_S) || (evt.key == OIS::KC_A) || (evt.key == OIS::KC_D)) && mCCPhysics->onGround())
	{
		if (evt.key == OIS::KC_SPACE) {
			if (mRun == true) {
				mRun = false;
				if (mKeyDirection.isZeroLength()){
					mCCPlayer->animRunEnd();
				} else {
					mCCPlayer->animWalkStart();
				}
			}
		} else if (mKeyDirection.isZeroLength()){
			mCCPlayer->animWalkEnd();
		}
	}

	if (evt.key == OIS::KC_SPACE) {
		mRun = false;
	}
}

void Hero::injectMouseMove(const OIS::MouseEvent & evt)
{
	// update camera goal based on mouse movement
	updateCameraGoal(-0.05f * evt.state.X.rel, -0.05f * evt.state.Y.rel, -0.0005f * evt.state.Z.rel);
}

void Hero::updateCharacter(Real deltaTime)
{
	Real direction;
	if (mRun) {
		direction = RUN_SPEED * deltaTime;
	} else {
		direction = WALK_SPEED * deltaTime;
	}

	Vector3 playerPos = mCCPlayer->getPosition();

	//btVector3 pos = mCCPhysics->getPosition();
	btVector3 pos = mCCPhysics->getGhostObject()->getWorldTransform().getOrigin();

	Vector3 position(pos.x(), pos.y(), pos.z());


	if (position != playerPos)
	{
		mBodyNode->translate((position - playerPos) * direction);


		//TODO: Mirar los saltos
		/*
		if (!mIsFalling && !mCCPhysics->onGround()) // last frame we were on ground and now we're in "air"
		{
			mIsFalling = true;

			if (!mJumped) // if we jumped, let the CharacterController_Player's updateAnimations take care about this
				mCCPlayer->animJumpLoop();
		}
		else if (mCCPhysics->onGround() && mIsFalling) // last frame we were falling and now we're on the ground
		{
			mIsFalling = false;
			mJumped = false;

			mCCPlayer->animJumpEnd();
		}
		*/
	}

	mGoalDirection = Vector3::ZERO;   // we will calculate this

	if (mKeyDirection != Vector3::ZERO)
	{
		// calculate actuall goal direction in world based on player's key directions
		mGoalDirection += mKeyDirection.z * mCameraNode->getOrientation().zAxis();
		mGoalDirection += mKeyDirection.x * mCameraNode->getOrientation().xAxis();
		mGoalDirection.y = 0;
		mGoalDirection.normalise();

		Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(mGoalDirection);

		// calculate how much the character has to turn to face goal direction
		Real yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;

		// turn as much as we can, but not more than we need to
		if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
		else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));

		mBodyNode->yaw(Degree(yawToGoal));

		//mCCPhysics->setWalkDirection(mGoalDirection.x * direction, mGoalDirection.y * direction, mGoalDirection.z * direction);
		btVector3 walkDirection(mGoalDirection.x * direction, mGoalDirection.y * direction, mGoalDirection.z * direction);
		mCCPhysics->setWalkDirection(walkDirection);
		mCCPlayer->setIsMoving(true);
	}
	else
	{
		//mCCPhysics->setWalkDirection(0, 0, 0);
		btVector3 walkDirection(0,0,0);
		mCCPhysics->setWalkDirection(walkDirection);
		mCCPlayer->setIsMoving(false);
	}


	updateCamera(deltaTime);

	mCCPlayer->addTime(deltaTime);

}

void Hero::setupCamera(Camera * cam)
{
	mCameraPivot = mSceneManager->getRootSceneNode()->createChildSceneNode();

	mCameraGoal = mCameraPivot->createChildSceneNode(Vector3(0, 0, Properties::getSingletonPtr()->getPropertyFloat("hero.camera.init.distance")));

	mCameraNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mCameraNode->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

	mCameraPivot->setFixedYawAxis(true);
	mCameraGoal->setFixedYawAxis(true);
	mCameraNode->setFixedYawAxis(true);

	cam->setNearClipDistance(0.1);
	mCameraNode->attachObject(cam);
	mCameraNode->setAutoTracking(true, mCameraPivot);

	mPivotPitch = 0;
}

void Hero::updateCamera(Real deltaTime)
{
	mCameraPivot->setPosition(mBodyNode->getPosition() + Vector3::UNIT_Y * cameraHeight);

	Vector3 goalOffset = mCameraGoal->_getDerivedPosition() - mCameraNode->getPosition();
	mCameraNode->translate(goalOffset * deltaTime * 9);
}

void Hero::updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom)
{
	mCameraPivot->yaw(Degree(deltaYaw), Node::TS_WORLD);

	// bound the pitch
	if (!(mPivotPitch + deltaPitch > 25 && deltaPitch > 0) &&
		!(mPivotPitch + deltaPitch < -60 && deltaPitch < 0))
	{
		mCameraPivot->pitch(Degree(deltaPitch), Node::TS_LOCAL);
		mPivotPitch += deltaPitch;
	}

	Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
	Real distChange = deltaZoom * dist;

	// bound the zoom
	if (!(dist + distChange < 8 && distChange < 0) &&
		!(dist + distChange > 25 && distChange > 0))
	{
		mCameraGoal->translate(0, 0, distChange, Node::TS_LOCAL);
	}
}


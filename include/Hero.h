#ifndef __HERO_H__
#define __HERO_H__


#include "CharacterControllerPlayer.h"
#include "MyKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "Properties.h"
#include "PlayState.h"
#include "Hud.h"
#include "Enemy.h"

#include <OIS/OIS.h>

using namespace Ogre;

class btGhostObject;
class btPairCachingGhostObject;
class btOverlappingPairCache;

class Enemy;
class Hero
{
private:
	int _walkSpeed;
	int _runSpeed;
	static const int TURN_SPEED = 500;

	MyKinematicCharacterController* mCCPhysics;
	CharacterControllerPlayer * mCCPlayer;

	SceneManager * mSceneManager;

	Camera * mCamera;
	SceneNode * mBodyNode;
	SceneNode * mCameraPivot;
	SceneNode * mCameraGoal;
	SceneNode * mCameraNode;
	Real mPivotPitch;
	Vector3 mWalkDirection;
	Real cameraHeight;

	bool mIsFalling;
	bool mJumped;
	bool mRun;

	Vector3 mGoalDirection;
	Vector3 mKeyDirection;

	float _maxTimeRunning; // Máximo tiempo que el personaje puede correr sin parar
	float _timeRest; // Tiempo de descanso para poder volver a correr
	float _timeRunningAccumulated; // tiempo acumulado corriendo sin parar
	float _timeRestAccumulated; // tiempo acumulado descansando
	bool _cansado; // Variable que indica que el personaje está cansado y no puede correr.


	enum objectID
	{
		KEY,
		GAS_CAN,
		CAR,
	};

	std::map<objectID,bool> _inventary;


public:
	Hero(String name, String mesh, SceneManager * scnMgr, Camera * cam, btPairCachingGhostObject * ghostObject,
			btConvexShape * convexShape, btScalar stepHeight, btCollisionWorld * collisionWorld,
			Vector3 & origin, int upAxis = 1
	);


	/*
	Hero(SceneManager * scnMgr, CharacterControllerPlayer * ccPlayer,
			MyKinematicCharacterController * ccPhysics
	);
	 */

	CharacterControllerPlayer * getCCPlayer();
	MyKinematicCharacterController * getCCPhysics();
	void injectKeyDown(const OIS::KeyEvent & evt);
	void injectKeyUp(const OIS::KeyEvent & evt);
	void injectMouseMove(const OIS::MouseEvent & evt);
	void updateCharacter(Real dt);

	void checkObject();

	bool collisionWithEnemy(std::vector<Enemy*> enemies);
	bool isGameCompleted();


private:
	void setupCamera(Camera * cam);
	void updateCamera(Real deltaTime);
	void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom);
	Quaternion updateOrientation(Real deltaTime);

	bool checkDistance(objectID id);
	void getObject(objectID id);
};
#endif

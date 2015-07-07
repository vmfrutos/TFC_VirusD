#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"

#include "GameState.h"
#include "PhysicsWorld.h"
#include "physics_debugdraw.h"
#include "Hero.h"
#include "Enemy.h"
#include "IntroState.h"
#include "Properties.h"
#include "Hud.h"
#include "GameOverState.h"
#include "CompletedState.h"

using namespace Ogre;
using namespace std;

class Enemy;
class Hero;

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:
  PlayState () {}

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);
  bool frameRenderingQueued (const Ogre::FrameEvent& evt);


  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();

  Hero* getHero();
  std::vector<Enemy*> getEnemies();

  Hud* getHud();

  SceneManager* getSceneManager();

  PhysicsWorld* getPhysicsWorld();

  void deleteObject(string name);


 private:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  PhysicsWorld * _physicsWorld;
  DebugDrawer * _debugDrawer;
  Hero* _character;
  std::vector<Enemy*> _enemies;

  Hud* _hud;

  TrackPtr _audio;

  void createScene();
  void addObject(string name, string mesh,Vector3 position);
  void addObjectDinamic(string name,string mesh,Vector3 position,float mass,Vector3 collisionBox);



  void setLights();
  bool _exitGame;
  bool _gameOver;
  bool _gameCompleted;
  bool _timeOver;

};

#endif

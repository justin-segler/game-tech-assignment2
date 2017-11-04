#ifndef _GAME_H
#define _GAME_H


#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <math.h> 

#include "LinearMath/btQuaternion.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "btBulletDynamicsCommon.h"

#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>
#include <Overlay/OgreOverlaySystem.h>

#include "Racket.h"
#include "SoundManager.h"
#include "Target.h"
#include "Gui.h"
#include "NetManager.h"
#include "MainMenu.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#  include <OIS/OISEvents.h>
#  include <OIS/OISInputManager.h>
#  include <OIS/OISKeyboard.h>
#  include <OIS/OISMouse.h>

#  include <OGRE/SdkTrays.h>
#  include <OGRE/SdkCameraMan.h>
#else
#  include <OISEvents.h>
#  include <OISInputManager.h>
#  include <OISKeyboard.h>
#  include <OISMouse.h>

#  include <SdkTrays.h>
#  include <SdkCameraMan.h>
#endif

#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
// D3D10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  include "OgreStaticPluginLoader.h"
#endif

enum GameState { Menu, SinglePlayer, MultiPlayer, GameOver };

//---------------------------------------------------------------------------
class Game : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{
    friend void myTickCallback(btDynamicsWorld *world, btScalar timeStep);
public:
    Game(void);
    virtual ~Game(void);

    virtual void go(void);
    Ogre::SceneNode*            ballNode;
    Ogre::Entity*               ballEntity;
    Ogre::Entity*               groundEntity;
    Ogre::SceneNode*            groundNode;
    Ogre::Entity*               ceilingEntity;
    Ogre::SceneNode*            ceilingNode;
    Ogre::Entity*               leftWallEntity;
    Ogre::SceneNode*            leftWallNode;
    Ogre::Entity*               rightWallEntity;
    Ogre::SceneNode*            rightWallNode;
    Ogre::Entity*               frontWallEntity;
    Ogre::SceneNode*            frontWallNode;
    Ogre::Entity*               backWallEntity;
    Ogre::SceneNode*            backWallNode; 
    Ogre::Vector3               move;
    btDiscreteDynamicsWorld*    World;
    btAlignedObjectArray<btRigidBody*>* Objects;
    btBroadphaseInterface       *BroadPhase;
    btDefaultCollisionConfiguration *CollisionConfiguration;
    btCollisionDispatcher       *Dispatcher;
    btSequentialImpulseConstraintSolver *Solver;
    btRigidBody*                ballRigidBody;

    Racket*                     racket;
    SoundManager                sound;
    Target*                     target;
    Gui*                        gui;
    NetManager*                 netManager;
    MainMenu*                   mainMenu;

    GameState                   gameState;
    bool movingUp, movingDown, movingLeft, movingRight;
    double racketSoundThresh;
    bool makeNewTarget = false;
    bool primaryClient = false;
    float runningTime = 0;
    size_t gridSize;
    bool gameStarted = false;

protected:
    virtual bool initNetwork(void);
    virtual bool setup();
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool updatePhysics(const Ogre::FrameEvent& evt);
    virtual bool enter(void);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	virtual void createScene(void);
    virtual void createLight(void);
    virtual void createGround(void);
    virtual void createCeiling(void);
    virtual void createRightWall(void);
    virtual void createLeftWall(void);
    virtual void createFrontWall(void);
    virtual void createBackWall(void);
    virtual void createBall(void);

    virtual void wallCollision(Ogre::SceneNode* wallNode);
    virtual void racketCollision();
    virtual void targetCollision();

    // Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    // Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

    Ogre::Root*                 mRoot;
    Ogre::Camera*               mCamera;
    Ogre::SceneManager*         mSceneMgr;
    Ogre::RenderWindow*         mWindow;
    Ogre::String                mResourcesCfg;
    Ogre::String                mPluginsCfg;

    Ogre::OverlaySystem*        mOverlaySystem;

    bool                        mCursorWasVisible;	// Was cursor visible before dialog appeared?
    bool                        mShutDown;

    OgreBites::SdkCameraMan*    mCameraMan;

    //OIS Input devices
    OIS::InputManager*          mInputManager;
    OIS::Mouse*                 mMouse;
    OIS::Keyboard*              mKeyboard;

    // Added for Mac compatibility
    Ogre::String                 m_ResourcePath;

#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader m_StaticPluginLoader;
#endif
};

#endif
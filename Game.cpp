#include "Game.h"
#include <OgreMeshManager.h>
#include <OgreQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include <OgreTextureManager.h>
#include <iostream>
#include <future>
#include <thread>
#include <chrono>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <macUtils.h>
#endif

//---------------------------------------------------------------------------
Game::Game(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    mOverlaySystem(0),
    ballNode(0),
    ballEntity(0),
    sound(),
    gui(0),
    netManager(0)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    m_ResourcePath = "";
#endif
}
//---------------------------------------------------------------------------
Game::~Game(void)
{
  if (mOverlaySystem) delete mOverlaySystem;

  // Remove ourself as a Window listener
  Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);
  delete mRoot;
}

//---------------------------------------------------------------------------
bool Game::configure(void)
{
    // Show the configuration dialog and initialise the system.
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg.
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise.
        // Here we choose to let the system create a default rendering window by passing 'true'.
        mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
void Game::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    // Initialize the OverlaySystem (changed for Ogre 1.9)
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
}

// FREECAM is an option to use for debugging.  Easier to see scene with free cam.
// Set this to 1 for free camera
// Set this to 0 for follow camera (following racket)
#define FREECAM 0

//---------------------------------------------------------------------------
void Game::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,80,120));
    
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,80,-120));
    mCamera->setNearClipDistance(5);
#if FREECAM
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
#endif
}
//---------------------------------------------------------------------------
void Game::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);
    std::cout << "After initializing OIS? " << std::endl;

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    // Set initial mouse clipping size
    windowResized(mWindow);

    // Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);

    std::cout << "End of createFrameListener" << std::endl;
}
//---------------------------------------------------------------------------
void Game::destroyScene(void)
{
}
//---------------------------------------------------------------------------
bool Game::initNetwork(void)
{

}
//---------------------------------------------------------------------------
void Game::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//---------------------------------------------------------------------------
void Game::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

            // OS X does not set the working directory relative to the app.
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location.
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
                archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//---------------------------------------------------------------------------
void Game::createResourceListener(void)
{
}
//---------------------------------------------------------------------------
void Game::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//---------------------------------------------------------------------------
void Game::go(void)
{
#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources_d.cfg";
    mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
#else
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources.cfg";
    mPluginsCfg = m_ResourcePath + "plugins.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // TODO: End game and show end game screen

        // Either replay game or quit game

        // If replay game, call setup() again

        // If quit game, 

            // Drop clients

            // Close server

    // Clean up
    destroyScene();
}
//---------------------------------------------------------------------------
bool Game::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    //initializes the network
    initNetwork();

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Sets default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Creates any resource listeners (for loading screens)
    createResourceListener();
    
    // Loads resources
    loadResources();

    // Check if primary or secondary client

    // If primary,

        // Go into main menu and wait for another player to join

            // Establish Server
            //netManager->startServer(); 

            // Establish Primary client
            //netManager->

            // Scan for activity

        // Establish second player

            // Hit server

            // Establish Secondary Client

        // Set timer and start game

    // If secondary

        // Connect to server as client

        // Wait for timer to be set

    // Creates the scene
    enter();

    createFrameListener();

    ballRigidBody->setUserPointer(this);

    return true;
};

//---------------------------------------------------------------------------
// This function gets called by bullet at the end of every simulation frame.
void myTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    int numManifolds = world->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        int numContacts = contactManifold->getNumContacts();
        for (int j = 0; j < numContacts; j++)
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance() < 0.f)
            {
                const btVector3& ptA = pt.getPositionWorldOnA(); //Position of contact on first obj
                const btVector3& ptB = pt.getPositionWorldOnB(); //Position of contact on second obj
                
                const btVector3& normalOnB = pt.m_normalWorldOnB;
                if(obA->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
                {
                    Game* app = static_cast<Game*>(obB->getUserPointer());
                    app->racketCollision();
                }
                else if(obB->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
                {
                    Game* app = static_cast<Game*>(obA->getUserPointer());
                    app->racketCollision();
                }
                else if(obA->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE)
                {
                    Game* app = static_cast<Game*>(obB->getUserPointer());
                    app->targetCollision();
                }
                else if(obB->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE)
                {
                    Game* app = static_cast<Game*>(obA->getUserPointer());
                    app->targetCollision();
                }
                else if(obA->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
                {
                    Game* app = static_cast<Game*>(obB->getUserPointer());
                    app->wallCollision(static_cast<Ogre::SceneNode*>(obA->getUserPointer()));
                }
                else if(obB->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
                {
                    Game* app = static_cast<Game*>(obA->getUserPointer());
                    app->wallCollision(static_cast<Ogre::SceneNode*>(obB->getUserPointer()));
                }
            }
        }
    }
}

bool Game::enter(void) {
    BroadPhase = new btDbvtBroadphase();;
    CollisionConfiguration = new btDefaultCollisionConfiguration();
    Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
    Solver = new btSequentialImpulseConstraintSolver();
    World = new btDiscreteDynamicsWorld(Dispatcher, BroadPhase, Solver, CollisionConfiguration);
    World->setGravity(btVector3(0, -30, 0));
    Objects = new btAlignedObjectArray<btRigidBody*>();
    World->setInternalTickCallback(myTickCallback);

    movingUp = movingDown = movingLeft = movingRight = false;
    racketSoundThresh = 0.0;

    createScene();
#if FREECAM == 0
    mCamera->setAutoTracking(true, racket->getCentralNode());
#endif
}

//---------------------------------------------------------------------------
bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

#if FREECAM
    mCameraMan->frameRenderingQueued(evt);
#else
    Ogre::Vector3 movement = Ogre::Vector3::ZERO;
    if(movingUp)
        movement.y += 1;
    if(movingDown)
        movement.y -= 1;
    if(movingLeft)
        movement.x -= 1;
    if(movingRight)
        movement.x += 1;
    movement *= 100.0 * evt.timeSinceLastFrame;
    racket->move(movement);
    mCamera->move(movement);
    if(mCamera->getPosition().x < -96 || mCamera->getPosition().x > 96 
    || mCamera->getPosition().y < 2.5    || mCamera->getPosition().y > 197.5)
    {
        racket->move(-movement);
        mCamera->move(-movement);
    }
    racket->updateSwing(evt);
#endif
    updatePhysics(evt);

    if(racketSoundThresh > 0)
        racketSoundThresh -= evt.timeSinceLastFrame;
    if(racketSoundThresh < 0)
        racketSoundThresh = 0.0;
    if (makeNewTarget) {
        World->removeRigidBody(target->getRigidBody());
        delete target;
        target = new Target(mSceneMgr, World, Objects);
        makeNewTarget = false;
    }

    return true;
}

bool Game::updatePhysics(const Ogre::FrameEvent& evt) {

    World->stepSimulation(evt.timeSinceLastFrame, 10);

    int i = 0;

    // Updates balls position upon a collision
    btTransform ballTrans;
    ballRigidBody->getMotionState()->getWorldTransform(ballTrans);
    Ogre::Vector3 ballVect(ballTrans.getOrigin().getX(),ballTrans.getOrigin().getY(), ballTrans.getOrigin().getZ());

    btQuaternion ballBtq = ballRigidBody->getOrientation();
    Ogre::Quaternion ballQuart = Ogre::Quaternion(ballBtq.w(),ballBtq.x(),ballBtq.y(),ballBtq.z());

    ballNode->setPosition(ballVect);
    ballNode->setOrientation(ballQuart);

    // If collision with target, delete target and create new target
    if (makeNewTarget) {
        World->removeRigidBody(target->getRigidBody());
        delete target;
        target = new Target(mSceneMgr, World, Objects);
        makeNewTarget = false;
    }
}


//---------------------------------------------------------------------------
bool Game::keyPressed( const OIS::KeyEvent &arg )
{
    if (arg.key == OIS::KC_ESCAPE) {
    mShutDown = true;
    }

    #if FREECAM
        mCameraMan->injectKeyDown(arg);
    #else
        switch(arg.key)
        {
            case OIS::KC_W:
                movingUp = true;
            break;
            case OIS::KC_S:
                movingDown = true;
            break;
            case OIS::KC_A:
                movingLeft = true;
            break;
            case OIS::KC_D:
                movingRight = true;
            break;
            case OIS::KC_M:
                gui->destroyWindow();
                gui->createMultiplayer();
            break;
            case OIS::KC_N:
                gui->destroyWindow();
                gui->createSingle();
            break;
            case OIS::KC_E:
                gui->destroyWindow();
                gui->createEnd();
            break;

        }

    #endif

    if(arg.key == OIS::KC_T)
    {
        // If T is pressed, the existing target is removed and a new target is added
        // This was mostly for testing.
        World->removeRigidBody(target->getRigidBody());
        delete target;
        target = new Target(mSceneMgr, World, Objects);
    } 
    if (arg.key == OIS::KC_SPACE) 
    {
        // If the spacebar is pressed, the ball's position and velocity is reset
        btDefaultMotionState* ballMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
        ballRigidBody->setMotionState(ballMotionState);
        ballRigidBody->setLinearVelocity(btVector3(0,0,0));
        gui->resetScore();
    }

  return true;
}
//---------------------------------------------------------------------------
bool Game::keyReleased(const OIS::KeyEvent &arg)
{

#if FREECAM
    mCameraMan->injectKeyUp(arg);
#else
    switch(arg.key)
    {
        case OIS::KC_W:
            movingUp = false;
        break;
        case OIS::KC_S:
            movingDown = false;
        break;
        case OIS::KC_A:
            movingLeft = false;
        break;
        case OIS::KC_D:
            movingRight = false;
        break;
    }
#endif
    return true;
}
//---------------------------------------------------------------------------
bool Game::mouseMoved(const OIS::MouseEvent &arg)
{
#if FREECAM
    mCameraMan->injectMouseMove(arg);
#else
    // Mouse position is [0,0] at top left and [screenwidth, screenheight] at bottom right
    Ogre::Vector2 mousePos(arg.state.X.abs, arg.state.Y.abs);
    Ogre::Vector2 screenMiddle(mWindow->getWidth() / 2, mWindow->getHeight() / 2);
    Ogre::Vector3 mouseDiff(mousePos.x - screenMiddle.x, mousePos.y - screenMiddle.y, 0);
    mouseDiff.y *= -1;
    mouseDiff.normalise();
    racket->setMouseRotation(mouseDiff);
#endif
    return true;
}
//---------------------------------------------------------------------------
bool Game::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
#if FREECAM
    mCameraMan->injectMouseDown(arg, id);
#else
    if(id == OIS::MB_Left)
    {
        if(racket->swing())
        {
            sound.woosh();
        }
    }
#endif
    return true;
}
//---------------------------------------------------------------------------
bool Game::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
#if FREECAM
    mCameraMan->injectMouseUp(arg, id);
#else

#endif
    return true;
}
//---------------------------------------------------------------------------
// Adjust mouse clipping area
void Game::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//---------------------------------------------------------------------------
// Unattach OIS before window shutdown (very important under Linux)
void Game::windowClosed(Ogre::RenderWindow* rw)
{
    // Only close for window that created OIS (the main window in these demos)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
//---------------------------------------------------------------------------
void Game::wallCollision(Ogre::SceneNode* wallNode)
{
    if(std::abs(ballRigidBody->getLinearVelocity().y()) > 0.05)
        sound.ball();

    if(wallNode == backWallNode)
    {
        gui->increaseScore();
    }
    else if(wallNode == frontWallNode)
    {
        gui->resetScore();
    }

}
void Game::racketCollision()
{
    if(racketSoundThresh == 0.0)
    {
        sound.racket();
        racketSoundThresh = 1.0;
    }

}
void Game::targetCollision()
{
    sound.ball();
    sound.success();
    gui->increaseScore(5);

    makeNewTarget = true;
}

//---------------------------------------------------------------------------
void Game::createScene(void)
{
    sound.playMusic();

    gridSize = 100;

    // Creates point light
    createLight();

    // Scales materials for walls
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("sibenik/pod");
    material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScale(4.0, 4.0);
    material = Ogre::MaterialManager::getSingleton().getByName("Examples/Rockwall");
    material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScale(4.0, 4.0);

    //create planes
    createGround();
    createCeiling();
    createRightWall(); 
    createLeftWall();
    createFrontWall();
    createBackWall();

    // Creates ball
    createBall();

    // Creates racket
    racket = new Racket(mSceneMgr, mCamera->getPosition() - Ogre::Vector3(0,0,100), World, Objects);

    // Creates first target
    target = new Target(mSceneMgr, World, Objects);

    // Initializes the GUI
    gui = new Gui();
    gui->createRender();
    gui->createWindow();
}

/* This function creates the light in the scene */
void Game::createLight(void) 
{
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(0, 250, 0);
    light->setDiffuseColour(0.4, 0.4, 0.4);

    Ogre::Light* light2 = mSceneMgr->createLight();
    light2->setPosition(0, 150, 250);
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
}
/* This function creates the ball in the scene */
void Game::createBall(void) 
{
    ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Ball_Node");
    ballEntity = mSceneMgr->createEntity("mySphere", "sphere.mesh");
    ballEntity->setMaterialName("Ball");
    ballEntity->setCastShadows(true);
    ballNode->attachObject(ballEntity);
    ballNode->setPosition(Ogre::Vector3(0,50,0));

    Ogre::Vector3 size(0.05, 0.05, 0.05);
    ballNode->setScale(size);

    // Establishing a rigid body
    btCollisionShape *ballShape = new btSphereShape(size.x * 100.0);
    ballShape->setUserPointer(this);
    btDefaultMotionState* ballMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
    btScalar ballMass = 1.0f;
    btVector3 ballInertia(0, 0, 0);
    ballShape->calculateLocalInertia(ballMass, ballInertia);
    btRigidBody::btRigidBodyConstructionInfo ballRigidBodyCI(ballMass, ballMotionState, ballShape, ballInertia);
    ballRigidBody = new btRigidBody(ballRigidBodyCI);
    ballRigidBody->setRestitution(1.0f);
    World->addRigidBody(ballRigidBody);
    Objects->push_back(ballRigidBody);
}

/* This function creates the ground */
void Game::createGround(void) 
{
    Ogre::MovablePlane gPlane( Ogre::Vector3::UNIT_Y, 0 );
    Ogre::MeshManager::getSingleton().createPlane("Ground_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, gPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_X);
    groundEntity = mSceneMgr->createEntity("Ground_Plane","Ground_Grid");
    groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_Ground_Node_");
    groundNode->attachObject(groundEntity);
    groundEntity->setMaterialName("sibenik/pod");
    groundEntity->setCastShadows(false);
    groundNode->setPosition(0,0,0);
    groundNode->setScale(Ogre::Vector3(5,1,5));

    // Initializes a rigid body
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(groundNode->getPosition().x,groundNode->getPosition().y,groundNode->getPosition().z));   
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(0, 1, 0),0);
    Shape->setUserPointer((void *) (groundNode));

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);
    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);

    RigidBody->setRestitution(0.8f);
    RigidBody->setUserPointer((void *) (groundNode));
    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
    
}

/* This function creates the ceiling */
void Game::createCeiling(void) 
{
    Ogre::MovablePlane cPlane( Ogre::Vector3::UNIT_Y, 0 );
    Ogre::MeshManager::getSingleton().createPlane("Ceiling_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, cPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_X);
    ceilingEntity = mSceneMgr->createEntity("Ceiling_Plane","Ceiling_Grid");
    ceilingNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_Ceiling_Node_");
    ceilingNode->attachObject(ceilingEntity);
    ceilingEntity->setMaterialName("sibenik/pod");
    ceilingEntity->setCastShadows(false);
    ceilingNode->setPosition(0,200,0);
    ceilingNode->setScale(Ogre::Vector3(5,5,5));
    ceilingNode->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3(0,0,1)));

    // Initializes a rigid body
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(ceilingNode->getPosition().x,ceilingNode->getPosition().y,ceilingNode->getPosition().z));   
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(0, -1, 0),0);
    Shape->setUserPointer((void *) (ceilingNode));
    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);
    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);
    RigidBody->setUserPointer((void *) (ceilingNode));

    RigidBody->setRestitution(0.8f);
    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
}

/* This function creates the wall behind the camera */
void Game::createFrontWall(void) 
{
    Ogre::MovablePlane fwPlane( Ogre::Vector3::UNIT_Z, 0 );
    Ogre::MeshManager::getSingleton().createPlane("FWall_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, fwPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_Y);
    frontWallEntity = mSceneMgr->createEntity("FWall_Plane","FWall_Grid");
    frontWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_FWall_Node_");
    frontWallNode->attachObject(frontWallEntity);
    frontWallEntity->setMaterialName("Examples/Rockwall");
    frontWallEntity->setCastShadows(false);
    frontWallNode->setPosition(0,0,90);
    frontWallNode->setScale(Ogre::Vector3(5,5,5));
    frontWallNode->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3(0,1,0)));

    // Initializes a rigid body
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(frontWallNode->getPosition().x,frontWallNode->getPosition().y,frontWallNode->getPosition().z));   
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(0, 0, -1), 0);
    Shape->setUserPointer((void *) (frontWallNode));

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);
    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);
    RigidBody->setUserPointer((void *) (frontWallNode));

    RigidBody->setRestitution(0.8f);
    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
}

/* This function creates the wall in front of the camera */
void Game::createBackWall(void) 
{
    Ogre::MovablePlane bwPlane( Ogre::Vector3::UNIT_Z, 0 );
    Ogre::MeshManager::getSingleton().createPlane("BWall_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, bwPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_Y);
    backWallEntity = mSceneMgr->createEntity("BWall_Plane","BWall_Grid");
    backWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_BWall_Node_");
    backWallNode->attachObject(backWallEntity);
    backWallEntity->setMaterialName("Examples/Rockwall");
    backWallEntity->setCastShadows(false);
    backWallNode->setPosition(0,100,-200);
    backWallNode->setScale(Ogre::Vector3(5,5,5));

    // Initializes a rigid body
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(backWallNode->getPosition().x,backWallNode->getPosition().y,backWallNode->getPosition().z));   
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(0, 0, 1),0);
    Shape->setUserPointer((void *) (backWallNode));

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);
    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);
    RigidBody->setUserPointer((void *) (backWallNode));
    RigidBody->setRestitution(0.8f);

    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
}

/* This function creates the right wall */
void Game::createRightWall(void) 
{
    Ogre::MovablePlane rwPlane( Ogre::Vector3::UNIT_X, 0 );
    Ogre::MeshManager::getSingleton().createPlane("RWall_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rwPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_Y);
    rightWallEntity = mSceneMgr->createEntity("RWall_Plane","RWall_Grid");
    rightWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_RWall_Node_");
    rightWallNode->attachObject(rightWallEntity);
    rightWallEntity->setMaterialName("Examples/Rockwall");
    rightWallEntity->setCastShadows(false);
    rightWallNode->setPosition(100,0,0);
    rightWallNode->setScale(Ogre::Vector3(5,5,5));
    rightWallNode->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3(0,1,0)));

    // Initializes a rigid body
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(rightWallNode->getPosition().x,rightWallNode->getPosition().y,rightWallNode->getPosition().z)); 
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(-1, 0, 0),0);
    Shape->setUserPointer((void *) (rightWallNode));

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);
    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);
    RigidBody->setUserPointer((void *) (rightWallNode));
    RigidBody->setRestitution(0.8f);

    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
}

/* This function creates the left wall */
void Game::createLeftWall(void) 
{
    Ogre::MovablePlane lwPlane( Ogre::Vector3::UNIT_X, 0 );
    Ogre::MeshManager::getSingleton().createPlane("LWall_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, lwPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_Y);
    leftWallEntity = mSceneMgr->createEntity("LWall_Plane","LWall_Grid");
    leftWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_LWall_Node_");
    leftWallNode->attachObject(leftWallEntity);
    leftWallEntity->setMaterialName("Examples/Rockwall");
    leftWallEntity->setCastShadows(false);
    leftWallNode->setPosition(-100,0,0);
    leftWallNode->setScale(Ogre::Vector3(5,5,5));

    // Initializes a rigid body
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(leftWallNode->getPosition().x,leftWallNode->getPosition().y,leftWallNode->getPosition().z));   
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(1, 0, 0),0);
    Shape->setUserPointer((void *) (leftWallNode));

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);
    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);
    RigidBody->setUserPointer((void *) (leftWallNode));
    RigidBody->setRestitution(0.8f);

    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
}

//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Game app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
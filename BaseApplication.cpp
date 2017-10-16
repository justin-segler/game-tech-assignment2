/* 
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "BaseApplication.h"

#include <OgreTextureManager.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <macUtils.h>
#endif

//---------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
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
    gui(0)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    m_ResourcePath = "";
#endif
}
//---------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
  if (mOverlaySystem) delete mOverlaySystem;

  // Remove ourself as a Window listener
  Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);
  delete mRoot;
}

//---------------------------------------------------------------------------
bool BaseApplication::configure(void)
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
void BaseApplication::chooseSceneManager(void)
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
void BaseApplication::createCamera(void)
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
void BaseApplication::createFrameListener(void)
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
void BaseApplication::destroyScene(void)
{
}
//---------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//---------------------------------------------------------------------------
void BaseApplication::setupResources(void)
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
void BaseApplication::createResourceListener(void)
{
}
//---------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//---------------------------------------------------------------------------
void BaseApplication::go(void)
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

    // Clean up
    destroyScene();
}
//---------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
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
            std::cout << "===== CONTACT ======" << std::endl;
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance() < 0.f)
            {
                const btVector3& ptA = pt.getPositionWorldOnA(); //Position of contact on first obj
                const btVector3& ptB = pt.getPositionWorldOnB(); //Position of contact on second obj
                //std::cout << "ptA: " << ptA.x() << " " << ptA.y() << " " << ptA.z() << " ... ptB: " << ptB.x() << " " << ptB.y() << " " << ptB.z() << std::endl;
                const btVector3& normalOnB = pt.m_normalWorldOnB;
                if(obA->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
                {
                    BaseApplication* app = static_cast<BaseApplication*>(obB->getUserPointer());
                    app->racketCollision();
                }
                else if(obB->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
                {
                    BaseApplication* app = static_cast<BaseApplication*>(obA->getUserPointer());
                    app->racketCollision();
                }
                else if(obA->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE)
                {
                    BaseApplication* app = static_cast<BaseApplication*>(obB->getUserPointer());
                    app->targetCollision();
                }
                else if(obB->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE)
                {
                    BaseApplication* app = static_cast<BaseApplication*>(obA->getUserPointer());
                    app->targetCollision();
                }
                else if(obA->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
                {
                    BaseApplication* app = (BaseApplication*)(obB->getUserPointer());
                    app->wallCollision();
                }
                else if(obB->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
                {
                    BaseApplication* app = (BaseApplication*)(obA->getUserPointer());
                    app->wallCollision();
                }
            }
        }
    }
}

bool BaseApplication::enter(void) {

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
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
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

    racket->updateSwing(evt);
#endif
    updatePhysics(evt);

    if(racketSoundThresh > 0)
        racketSoundThresh -= evt.timeSinceLastFrame;
    if(racketSoundThresh < 0)
        racketSoundThresh = 0.0;

    return true;
}

bool BaseApplication::updatePhysics(const Ogre::FrameEvent& evt) {

    World->stepSimulation(evt.timeSinceLastFrame, 10);

    int i = 0;

    btTransform ballTrans;
    ballRigidBody->getMotionState()->getWorldTransform(ballTrans);
    Ogre::Vector3 ballVect(ballTrans.getOrigin().getX(),ballTrans.getOrigin().getY(), ballTrans.getOrigin().getZ());

    btQuaternion ballBtq = ballRigidBody->getOrientation();
    Ogre::Quaternion ballQuart = Ogre::Quaternion(ballBtq.w(),ballBtq.x(),ballBtq.y(),ballBtq.z());

    ballNode->setPosition(ballVect);
    ballNode->setOrientation(ballQuart);

        /*btTransform racketTrans;
        racketRigidBody->getMotionState()->getWorldTransform(racketTrans);
        Ogre::Vector3 racketVect(racketTrans.getOrigin().getX(),racketTrans.getOrigin().getY(), racketTrans.getOrigin().getZ());

        btQuaternion racketBtq = racketRigidBody->getOrientation();
        Ogre::Quaternion racketQuart = Ogre::Quaternion(racketBtq.w(),racketBtq.x(),racketBtq.y(),racketBtq.z());

        racketNode->setPosition(racketVect);
        racketNode->setOrientation(racketQuart);*/
    //}
}


//---------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
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
        }

    #endif

    if(arg.key == OIS::KC_T)
    {
        delete target;
        target = new Target(mSceneMgr, World, Objects);
    } 
    if (arg.key == OIS::KC_SPACE) {
        World->removeRigidBody(ballRigidBody);

        ballNode->setPosition(Ogre::Vector3(0,50,0));

        Ogre::Vector3 size(0.05, 0.05, 0.05);
        ballNode->setScale(size);

        btCollisionShape *ballShape = new btSphereShape(size.x * 100.0);

        btDefaultMotionState* ballMotionState =
                    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
        btScalar ballMass = 1.0f;
        btVector3 ballInertia(0, 0, 0);
        ballShape->calculateLocalInertia(ballMass, ballInertia);
        btRigidBody::btRigidBodyConstructionInfo ballRigidBodyCI(ballMass, ballMotionState, ballShape, ballInertia);
        ballRigidBody = new btRigidBody(ballRigidBodyCI);

        ballRigidBody->setRestitution(1.0f);
        World->addRigidBody(ballRigidBody);
    }

  return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::keyReleased(const OIS::KeyEvent &arg)
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
bool BaseApplication::mouseMoved(const OIS::MouseEvent &arg)
{
#if FREECAM
    mCameraMan->injectMouseMove(arg);
#else
    // std::cout << "X: " << arg.state.X.abs << " , Y: " << arg.state.Y.abs << std::endl;
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
bool BaseApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
#if FREECAM
    mCameraMan->injectMouseDown(arg, id);
#else
    if(id == OIS::MB_Left)
    {
        if(racket->swing())
            sound.woosh();
    }
#endif
    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
#if FREECAM
    mCameraMan->injectMouseUp(arg, id);
#else

#endif
    return true;
}
//---------------------------------------------------------------------------
// Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
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
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
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
void BaseApplication::wallCollision()
{
    std::cout << " === WALL ===" << std::endl;

    if(std::abs(ballRigidBody->getLinearVelocity().y()) > 0.05)
        sound.ball();
}
void BaseApplication::racketCollision()
{
    std::cout << " === RACKET ===" << std::endl;
    if(racketSoundThresh == 0.0)
    {
        sound.racket();
        racketSoundThresh = 1.0;
    }

}
void BaseApplication::targetCollision()
{
    std::cout << " === TARGET ===" << std::endl;
    sound.ball();
    sound.success();

}
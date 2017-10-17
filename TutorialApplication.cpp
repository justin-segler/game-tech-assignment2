/* 
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

#include "TutorialApplication.h"
#include <OgreMeshManager.h>
#include <OgreQuaternion.h>
#include <btBulletDynamicsCommon.h>

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
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
void TutorialApplication::createLight(void) 
{
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(0, 250, 0);
    light->setDiffuseColour(0.4, 0.4, 0.4);

    Ogre::Light* light2 = mSceneMgr->createLight();
    light2->setPosition(0, 150, 250);
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
}
/* This function creates the ball in the scene */
void TutorialApplication::createBall(void) 
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
void TutorialApplication::createGround(void) 
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
void TutorialApplication::createCeiling(void) 
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
void TutorialApplication::createFrontWall(void) 
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
void TutorialApplication::createBackWall(void) 
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
void TutorialApplication::createRightWall(void) 
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
void TutorialApplication::createLeftWall(void) 
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
        TutorialApplication app;

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

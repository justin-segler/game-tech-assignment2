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
    
    gridSize = 100;
    //create point light
    createLight();

    //create planes
    createGround();
    createCeiling();
    createRightWall(); 
    createLeftWall();
    createFrontWall();
    createBackWall();

    //create ball
    createBall();

    //set starting speed and direction of ball
    //setInitialBallSpeed();

    racket = new Racket(mSceneMgr, mCamera->getPosition() - Ogre::Vector3(0,0,100), World, Objects);

    target = new Target(mSceneMgr, World, Objects);
    gui = new Gui();
    gui->createRender();
    gui->createWindow();
}

void TutorialApplication::createLight(void) 
{
    /*Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20, 150, 50);*/

    Ogre::Light* light2 = mSceneMgr->createLight();
    light2->setPosition(0, 150, 250);
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
}

/*void TutorialApplication::setInitialBallSpeed(void) {
    dx = (double)((rand()%20)-9)/100;
    dy = (double)((rand()%20)-9)/100;
    dz = (double)((rand()%20)-9)/100;
    move = Ogre::Vector3(dx, dy, dz);
}*/

void TutorialApplication::createBall(void) 
{
    ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Ball_Node");
    ballEntity = mSceneMgr->createEntity("mySphere", "sphere.mesh");
    ballEntity->setMaterialName("BaseWhite");
    ballEntity->setCastShadows(true);
    ballNode->attachObject(ballEntity);
    ballNode->setPosition(Ogre::Vector3(0,50,0));

    //ballNode->setScale(Ogre::Vector3(1,1,1));
    // Ogre::AxisAlignedBox boundingB = ballNode->getBoundingBox();
    // Ogre::Vector3 size = boundingB.getSize(); 
    //
    // Let's just do this instead: 

    Ogre::Vector3 size(0.05, 0.05, 0.05);
    ballNode->setScale(size);

    btCollisionShape *ballShape = new btSphereShape(size.x * 100.0);
    // ^^ seems like bullet's units are different from Ogre's.  1.0 in Ogre is about 100.0 in Bullet
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

void TutorialApplication::createGround(void) 
{
    Ogre::MovablePlane gPlane( Ogre::Vector3::UNIT_Y, 0 );
    Ogre::MeshManager::getSingleton().createPlane("Ground_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, gPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_X);
    groundEntity = mSceneMgr->createEntity("Ground_Plane","Ground_Grid");
    groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_Ground_Node_");
    groundNode->attachObject(groundEntity);
    groundEntity->setMaterialName("BaseWhite");
    groundEntity->setCastShadows(false);
    groundNode->setPosition(0,0,0);
    groundNode->setScale(Ogre::Vector3(5,1,5));

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(groundNode->getPosition().x,groundNode->getPosition().y,groundNode->getPosition().z));
    
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(0, 1, 0),0);

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);

    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);

    RigidBody->setRestitution(0.5f);
    RigidBody->setUserPointer((void *) (groundNode));

    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
    
}

void TutorialApplication::createCeiling(void) 
{
    Ogre::MovablePlane cPlane( Ogre::Vector3::UNIT_Y, 0 );
    Ogre::MeshManager::getSingleton().createPlane("Ceiling_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, cPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_X);
    ceilingEntity = mSceneMgr->createEntity("Ceiling_Plane","Ceiling_Grid");
    ceilingNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_Ceiling_Node_");
    ceilingNode->attachObject(ceilingEntity);
    ceilingEntity->setMaterialName("BaseWhite");
    ceilingEntity->setCastShadows(false);
    ceilingNode->setPosition(0,200,0);
    ceilingNode->setScale(Ogre::Vector3(5,5,5));
    ceilingNode->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3(0,0,1)));

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(ceilingNode->getPosition().x,ceilingNode->getPosition().y,ceilingNode->getPosition().z));
    
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(0, -1, 0),0);

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);

    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);

    RigidBody->setUserPointer((void *) (ceilingNode));

    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
}

void TutorialApplication::createFrontWall(void) 
{
    Ogre::MovablePlane fwPlane( Ogre::Vector3::UNIT_Z, 0 );
    Ogre::MeshManager::getSingleton().createPlane("FWall_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, fwPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_Y);
    frontWallEntity = mSceneMgr->createEntity("FWall_Plane","FWall_Grid");
    frontWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_FWall_Node_");
    frontWallNode->attachObject(frontWallEntity);
    frontWallEntity->setMaterialName("BaseWhite");
    frontWallEntity->setCastShadows(false);
    frontWallNode->setPosition(0,0,100);
    frontWallNode->setScale(Ogre::Vector3(5,5,5));
    frontWallNode->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3(0,1,0)));

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(frontWallNode->getPosition().x,frontWallNode->getPosition().y,frontWallNode->getPosition().z));
    
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(0, 0, 1),0);

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);

    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);

    RigidBody->setUserPointer((void *) (frontWallNode));

    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
}

void TutorialApplication::createBackWall(void) 
{
    Ogre::MovablePlane bwPlane( Ogre::Vector3::UNIT_Z, 0 );
    Ogre::MeshManager::getSingleton().createPlane("BWall_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, bwPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_Y);
    backWallEntity = mSceneMgr->createEntity("BWall_Plane","BWall_Grid");
    backWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_BWall_Node_");
    backWallNode->attachObject(backWallEntity);
    backWallEntity->setMaterialName("BaseWhite");
    backWallEntity->setCastShadows(false);
    backWallNode->setPosition(0,100,-100);
    backWallNode->setScale(Ogre::Vector3(2,2,2));

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(backWallNode->getPosition().x,backWallNode->getPosition().y,backWallNode->getPosition().z));
    
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(0, 0, -1),0);

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);

    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);

    RigidBody->setUserPointer((void *) (backWallNode));

    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
}

void TutorialApplication::createRightWall(void) 
{
    Ogre::MovablePlane rwPlane( Ogre::Vector3::UNIT_X, 0 );
    Ogre::MeshManager::getSingleton().createPlane("RWall_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rwPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_Y);
    rightWallEntity = mSceneMgr->createEntity("RWall_Plane","RWall_Grid");
    rightWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_RWall_Node_");
    rightWallNode->attachObject(rightWallEntity);
    rightWallEntity->setMaterialName("BaseWhite");
    rightWallEntity->setCastShadows(false);
    rightWallNode->setPosition(100,0,0);
    rightWallNode->setScale(Ogre::Vector3(5,5,5));
    rightWallNode->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3(0,1,0)));

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(rightWallNode->getPosition().x,rightWallNode->getPosition().y,rightWallNode->getPosition().z));
    
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(-1, 0, 0),0);

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);

    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);

    RigidBody->setUserPointer((void *) (rightWallNode));

    World->addRigidBody(RigidBody);
    Objects->push_back(RigidBody);
}

void TutorialApplication::createLeftWall(void) 
{
    Ogre::MovablePlane lwPlane( Ogre::Vector3::UNIT_X, 0 );
    Ogre::MeshManager::getSingleton().createPlane("LWall_Grid",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, lwPlane,
        gridSize,gridSize,20,20,true,1,gridSize/4,gridSize/4,Ogre::Vector3::UNIT_Y);
    leftWallEntity = mSceneMgr->createEntity("LWall_Plane","LWall_Grid");
    leftWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("_LWall_Node_");
    leftWallNode->attachObject(leftWallEntity);
    leftWallEntity->setMaterialName("BaseWhite");
    leftWallEntity->setCastShadows(false);
    leftWallNode->setPosition(-100,0,0);
    leftWallNode->setScale(Ogre::Vector3(5,5,5));

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(leftWallNode->getPosition().x,leftWallNode->getPosition().y,leftWallNode->getPosition().z));
    
    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
    btCollisionShape *Shape = new btStaticPlaneShape(btVector3(1, 0, 0),0);

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);

    btRigidBody *RigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);

    RigidBody->setUserPointer((void *) (leftWallNode));

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

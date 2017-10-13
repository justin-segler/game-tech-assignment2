
#include "Target.h"

Target::Target(Ogre::SceneManager* scnMgr, btDiscreteDynamicsWorld* World, btAlignedObjectArray<btRigidBody*>* Objects)
{
	Ogre::Entity* target = scnMgr->createEntity("target.mesh");
    target->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    rootNode->attachObject(target);
    rootNode->pitch( Ogre::Degree(90) );
    rootNode->setPosition(rand() % 200 - 100, 20 + rand() % 200, -1 * rand() % 200);
    rootNode->setScale(20, 20, 20);

    btCollisionShape *targetShape = new btSphereShape(rootNode->getScale().x);

    btDefaultMotionState* targetMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
    btScalar targetMass = 1.0f;
    btVector3 targetInertia(0, 0, 0);
    targetShape->calculateLocalInertia(targetMass, targetInertia);
    btRigidBody::btRigidBodyConstructionInfo targetRigidBodyCI(targetMass, targetMotionState, targetShape, targetInertia);
    btRigidBody* targetRigidBody = new btRigidBody(targetRigidBodyCI);

    targetRigidBody->setRestitution(1.0f);
    World->addRigidBody(targetRigidBody);
    Objects->push_back(targetRigidBody);
    rootNode->showBoundingBox(true);
}

Target::~Target()
{
	Ogre::SceneManager* scnMgr = rootNode->getCreator();
	rootNode->removeAndDestroyAllChildren();
	scnMgr->destroySceneNode(rootNode);
}
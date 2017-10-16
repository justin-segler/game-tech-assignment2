
#include "Target.h"

Target::Target(Ogre::SceneManager* scnMgr, btDiscreteDynamicsWorld* World, btAlignedObjectArray<btRigidBody*>* Objects)
{
    srand(time(NULL));
	Ogre::Entity* target = scnMgr->createEntity("target.mesh");
    target->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    rootNode->attachObject(target);
    rootNode->pitch( Ogre::Degree(90) );
    rootNode->setPosition(rand() % 160 - 80, 20 + rand() % 160, -90);
    rootNode->setScale(20, 20, 20);

    Ogre::AxisAlignedBox boundingB = target->getBoundingBox();
    btCollisionShape *targetShape = new btCylinderShapeZ(
        btVector3(boundingB.getHalfSize().x * rootNode->getScale().x,
                  boundingB.getHalfSize().y * rootNode->getScale().y,
                  boundingB.getHalfSize().z * rootNode->getScale().z));

    btVector3 initialPos(rootNode->getPosition().x, rootNode->getPosition().y, rootNode->getPosition().z);
    btDefaultMotionState* targetMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), initialPos));
    btVector3 targetInertia(0, 0, 0);
    targetShape->calculateLocalInertia(0, targetInertia);
    btRigidBody::btRigidBodyConstructionInfo targetRigidBodyCI(0, targetMotionState, targetShape, targetInertia);
    btRigidBody* targetRigidBody = new btRigidBody(targetRigidBodyCI);

    targetRigidBody->setRestitution(1.0f);
    World->addRigidBody(targetRigidBody);
    Objects->push_back(targetRigidBody);
}

Target::~Target()
{
	Ogre::SceneManager* scnMgr = rootNode->getCreator();
	rootNode->removeAndDestroyAllChildren();
	scnMgr->destroySceneNode(rootNode);
}
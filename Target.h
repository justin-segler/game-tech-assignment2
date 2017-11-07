#ifndef __TARGET_H_
#define __TARGET_H_

#include <Ogre.h>
#include <cstdlib>
#include <btBulletDynamicsCommon.h>

class Target
{
protected:
	btRigidBody* rigidBody;
public:
	Target(Ogre::SceneManager* scnMgr, btDiscreteDynamicsWorld* World, btAlignedObjectArray<btRigidBody*>* Objects);
	~Target();
	Ogre::SceneNode* rootNode;
	btRigidBody* getRigidBody() {return rigidBody;}
};

#endif

#include "Target.h"

Target::Target(Ogre::SceneManager* scnMgr)
{
	Ogre::Entity* target = scnMgr->createEntity("target.mesh");
    target->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    rootNode->attachObject(target);
    rootNode->pitch( Ogre::Degree(90) );
    rootNode->setPosition(rand() % 200 - 100, 20 + rand() % 200, -1 * rand() % 200);
    rootNode->setScale(20, 20, 20);
}

Target::~Target()
{
	Ogre::SceneManager* scnMgr = rootNode->getCreator();
	rootNode->removeAndDestroyAllChildren();
	scnMgr->destroySceneNode(rootNode);
}
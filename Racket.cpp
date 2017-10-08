#include <OgreEntity.h> 
#include <OgreSceneManager.h> 
#include "Racket.h"

#define CENTER_OFFSET 40.0

Racket::Racket(Ogre::SceneManager* scnMgr) 
{ 
	Ogre::Entity* racket = scnMgr->createEntity("racket.mesh"); 
	racket->setMaterialName("Racket"); 
	racket->setCastShadows(true); 

	centralNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	centralNode->setPosition(0.0, 0.0, 0.0);

	racketNode = centralNode->createChildSceneNode();
	racketNode->attachObject(racket);

	racketNode->scale(10.0f, 1.0f, 10.0f);
	racketNode->pitch(Ogre::Degree(90));
}

Racket::Racket(Ogre::SceneManager* scnMgr, Ogre::Vector3 pos) 
{ 
	Ogre::Entity* racket = scnMgr->createEntity("racket.mesh"); 
	racket->setMaterialName("Racket"); 
	racket->setCastShadows(true);

	centralNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	centralNode->setPosition(pos);

	racketNode = centralNode->createChildSceneNode();
	racketNode->attachObject(racket);

	racketNode->pitch(Ogre::Degree(90));
	racketNode->scale(10.0f, 1.0f, 10.0f);
    racketNode->setPosition(0.0, CENTER_OFFSET, 0);
}

void Racket::move(const Ogre::Vector3& movement)
{
	centralNode->translate(movement);
}

void Racket::setRotation(const Ogre::Vector3& dir)
{
	racketNode->setPosition(dir * CENTER_OFFSET);

	// Points the +Z axis of the racketNode at the position of the centralNode
	racketNode->lookAt(centralNode->getPosition(), Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_Z);
}
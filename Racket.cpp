#include <OgreEntity.h> 
#include <OgreSceneManager.h> 
#include "Racket.h"


Racket::Racket(Ogre::SceneManager* scnMgr) 
{ 
	Ogre::Entity* racket = scnMgr->createEntity("racket.mesh"); 
	racket->setMaterialName("Racket"); 
	racket->setCastShadows(true); 
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(); 
	rootNode->attachObject(racket); 
	rootNode->scale(10.0f, 1.0f, 10.0f); 
	rootNode->setPosition(0.0, 0.0, 0.0);
}

Racket::Racket(Ogre::SceneManager* scnMgr, Ogre::Vector3 pos) 
{ 
	Ogre::Entity* racket = scnMgr->createEntity("racket.mesh"); 
	racket->setMaterialName("Racket"); 
	racket->setCastShadows(true); 
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(); 
	rootNode->attachObject(racket); 
	rootNode->scale(10.0f, 1.0f, 10.0f); 

    Ogre::Vector3 racketCameraOffset(0, -15, -15);
    rootNode->setPosition(pos + racketCameraOffset);
}
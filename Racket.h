#ifndef __RACKET_H_
#define __RACKET_H_

#include <Ogre.h> 

class Racket
{
protected: 
	Ogre::SceneNode* rootNode; 
public: 
	Racket(Ogre::SceneManager* scnMgr); 
	Racket(Ogre::SceneManager* scnMgr, Ogre::Vector3 pos); 
	~Racket(); 
	Ogre::SceneNode* getNode() { return rootNode; } 
};


#endif
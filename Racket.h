#ifndef __RACKET_H_
#define __RACKET_H_

class Racket
{
protected: 
	Ogre::SceneNode* rootNode; 
public: 
	Racket(Ogre::SceneManager* scnMgr); 
	Racket(Ogre::SceneManager* scnMgr, const Ogre::Vector3& pos, const Ogre::Real& speed, const Ogre::Vector3& dir); 
	~Racket(); 
	Ogre::SceneNode* getNode() { return rootNode; } 
};


#endif
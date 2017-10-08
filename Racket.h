#ifndef __RACKET_H_
#define __RACKET_H_

#include <Ogre.h> 

class Racket
{
protected:
	/* centralNode:
	 *   This is the sceneNode that the racket will rotate around.
	 *   It is the parent SceneNode to racketNode.
	 */ 
	Ogre::SceneNode* centralNode;

	/* racketNode:
	 *   This SceneNode will contain the entity for the racket and will orbit
	 *   the centralNode;
	 */
	Ogre::SceneNode* racketNode;
public: 
	Racket(Ogre::SceneManager* scnMgr); 
	Racket(Ogre::SceneManager* scnMgr, Ogre::Vector3 pos); 
	~Racket(); 
	Ogre::SceneNode* getCentralNode() { return centralNode; }
	void setRotation(const Ogre::Vector3& dir);
	void move(const Ogre::Vector3& movement);
};


#endif
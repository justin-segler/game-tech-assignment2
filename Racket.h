#ifndef __RACKET_H_
#define __RACKET_H_

#include <Ogre.h> 
#include <vector>
#include <btBulletDynamicsCommon.h>

class Racket
{
protected:
	
	Ogre::SceneNode* centralNode;
	/*   This is the sceneNode that the racket will rotate around.
	 *   It is the parent SceneNode to racketNode.
	 */ 

	Ogre::SceneNode* racketNode;
	/*   This SceneNode will contain the entity for the racket and will orbit
	 *   the centralNode.
	 */
	bool swinging, backSwing;
	double swingState;
	/*	 Keeps track of the current state of the swing animation.
	 *    0 is the default state, no swing initiated.
	 *    1 is the final state, fully extended forward (pos = [0,0, Z - Center_offset]).
	 *    0-1 means racket is in the middle of animating, the value represents completion percentage.
	 */

	 Ogre::Vector3 swingStart;
public: 
	Racket(Ogre::SceneManager* scnMgr, Ogre::Vector3 pos); 
	~Racket(); 
	Ogre::SceneNode* getCentralNode() { return centralNode; }
	void setMouseRotation(const Ogre::Vector3& dir);
	void setRotation(const Ogre::Vector3& dir);
	void move(const Ogre::Vector3& movement);
	void swing();
	void updateSwing(const Ogre::FrameEvent& evt);
	void getRigidBody(void);
};


#endif
#ifndef __RACKET_H_
#define __RACKET_H_

#include <Ogre.h> 
#include <vector>
#include <btBulletDynamicsCommon.h>

class RacketMotionState : public btMotionState
{
private:
	btTransform transform;
public:
	RacketMotionState(const btTransform& trans) : transform(trans) {}
	void 	getWorldTransform (btTransform &worldTrans) const;
	void 	setWorldTransform (const btTransform &worldTrans);
	void	updateTransform(const Ogre::Quaternion& quat, const Ogre::Vector3& pos);
};

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
	/*   Keeps track of the initial position of the racket as the swing animation begins.
	 *   It is set in swing().  The value of swingStart means nothing unless swinging == true.
	 */

	btRigidBody* rigidBody;
	/*   Bullet rigid body
	 */

	RacketMotionState* motionState;
	/*   Custom motion state:
	 *		Tells bullet where the racket is, each simulation frame.
	 *		This motion state needs to be updated whenever the racket moves.
	 */
public: 
	Racket(Ogre::SceneManager* scnMgr, Ogre::Vector3 pos, btDiscreteDynamicsWorld* World, btAlignedObjectArray<btRigidBody*>* Objects);  
	~Racket(); 
	Ogre::SceneNode* getCentralNode() { return centralNode; }
	void setMouseRotation(const Ogre::Vector3& dir);
	void setRotation(const Ogre::Vector3& dir);
	void move(const Ogre::Vector3& movement);
	bool swing();
	void updateSwing(const Ogre::FrameEvent& evt);
	btRigidBody* setRigidBody(void);
};


#endif
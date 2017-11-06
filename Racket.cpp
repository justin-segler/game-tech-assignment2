#include <OgreEntity.h> 
#include <OgreSceneManager.h> 
#include <OgreMeshManager.h>
#include <OgreQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include "Racket.h"

/* Center_offset:
 *  This is the distance from the racket to the center node.
 *  The racket orbits the center node, so this distance will ALWAYS
 *  be constant.
 */
#define CENTER_OFFSET 40.0


/* Racket class constructor*/
Racket::Racket(Ogre::SceneManager* scnMgr, Ogre::Vector3 pos, btDiscreteDynamicsWorld* World, btAlignedObjectArray<btRigidBody*>* Objects) 
{ 
	Ogre::Entity* racket = scnMgr->createEntity("racket.mesh"); 
	racket->setMaterialName("Racket"); 
	racket->setCastShadows(true);

	centralNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	centralNode->setPosition(pos);

	racketNode = centralNode->createChildSceneNode();
	racketNode->attachObject(racket);
	racketNode->scale(10.0f, 1.0f, 10.0f);
	racketNode->setPosition(0.0, CENTER_OFFSET, 0);
	racketNode->setAutoTracking(true, centralNode, Ogre::Vector3::UNIT_Z);

	swingState = 0;
	swinging = backSwing = false;
	swingStart = Ogre::Vector3::ZERO;

	// Sets up the rigid body for the racket
	Ogre::Vector3 scale = Ogre::Vector3(racketNode->getScale().x*.5f, racketNode->getScale().y*.5f, racketNode->getScale().z*.5f);
	btCollisionShape* racketShape = new btBoxShape(btVector3(scale.x * 1.25, scale.y * 1.25, scale.z * 2));

	btVector3 initialPos(pos.x, pos.y + CENTER_OFFSET, pos.z);
	motionState = new RacketMotionState(btTransform(btQuaternion(0, 0, 0, 1), initialPos));
	btScalar racketMass = 3.0f;
	btVector3 racketInertia(0, 0, 0);
	racketShape->calculateLocalInertia(racketMass, racketInertia);
	btRigidBody::btRigidBodyConstructionInfo racketRigidBodyCI(racketMass, motionState, racketShape, racketInertia);
	rigidBody = new btRigidBody(racketRigidBodyCI);
	rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	rigidBody->setRestitution(1.0f);
	
	World->addRigidBody(rigidBody);
	Objects->push_back(rigidBody);
}

/* This is a function that moves the racket */
void Racket::move(const Ogre::Vector3& movement)
{
	centralNode->translate(movement);
}

/*  This is a separate function from setRoation because
 *  we don't want mouse control to interfere with swinging */
void Racket::setMouseRotation(const Ogre::Vector3& dir)
{
	if(swinging)
		return;
	setRotation(dir);
}
/*  Sets the rotation & position of the racket relative to the
 *  center point. Keep in mind that dir must be a unit vector,
 *  and racketNode will ALWAYS point towards its parent.      */
void Racket::setRotation(const Ogre::Vector3& dir)
{
	racketNode->setPosition(dir * CENTER_OFFSET);
	motionState->updateTransform(racketNode->getOrientation(), racketNode->getPosition() + centralNode->getPosition());
}

void Racket::setPosition(const Ogre::Vector3& pos)
{
	racketNode->setPosition(pos);
	motionState->updateTransform(racketNode->getOrientation(), racketNode->getPosition() + centralNode->getPosition());
}


/* This function calculates the swing of the racket. */
bool Racket::swing()
{
	if(swinging)
		return false;
	swingStart = racketNode->getPosition();
	swinging = true;
	swingState = 0.0000001;
	return true;

}

// This is the threshold for how far the racket should rotate forward when swung.
//    (i.e. the maximum value for swingState):
// In theory 1.0 would mean 90 degrees of rotation, but the quaternions are funky
// and aren't doing what they should?  So, about 1.4 goes to 90 degrees.
// This could definitely use some fine-tuning.
// Putting SWING_THRESH at higher than 1.4 will overswing by a bit, like a follow-through.
// Putting SWING_THRESH below 1.4 will underswing, just a quick swat.
#define SWING_THRESH 1.2

// This is the total amount of time the swing animation will take.  Note that it
// does not affect the actual physical range of the swing, just how fast it moves. 
#define TOTAL_SWING_TIME 0.35

void Racket::updateSwing(const Ogre::FrameEvent& evt)
{
	if(!swinging)
		return;
	if(swingStart == Ogre::Vector3::ZERO)
		return;
	// This increment amount controls the speed of the animation.
	swingState += (backSwing ? -1.0 : 1.0) * SWING_THRESH * evt.timeSinceLastFrame / TOTAL_SWING_TIME;

	// These checks handle the frames where we have reached the end
	// of an animation, just making sure we don't over-rotate due to
	// framerate drops.
	if(swingState >= SWING_THRESH)
		swingState = SWING_THRESH;
	if(swingState <= 0)
		swingState = 0.0;

	// We know the starting position is swingStart, and 
	// the ending position is (0,0,-CENTER_OFFSET).	
	// So we can use this info to construct a quaternion
	// that describes our desired rotation.
	Ogre::Vector3 rotAxis = swingStart.crossProduct(Ogre::Vector3(0,0,-1));
	// ^^ rotAxis is the axis of rotation, found by cross product of initial and final position
	Ogre::Quaternion rotQuat(Ogre::Degree(swingState) * Ogre::Math::PI / 2.0, rotAxis);
	// ^^ rotQuat describes a rotation around rotAxis, with the angle being proportional to swingState.
	Ogre::Vector3 newPos = rotQuat * swingStart;
	newPos.normalise();
	setRotation(newPos);

	//If we are at max extension, begin backswing
	if(swingState == SWING_THRESH)
		backSwing = true;

	//If we have finished our backswing, the animation is complete
	if(swingState == 0.0)
	{
		swinging = false;
		backSwing = false;
	}
}

/* This function gets the world transform. */
void RacketMotionState::getWorldTransform (btTransform &trans) const
{
	trans = transform;
}

/* This function sets the world transform. */
void RacketMotionState::setWorldTransform (const btTransform &trans )
{
	transform = trans;
}

/* This function updates the world transform. */
void RacketMotionState::updateTransform(const Ogre::Quaternion& quat, const Ogre::Vector3& pos)
{
	transform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
}
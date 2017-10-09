#ifndef __TARGET_H_
#define __TARGET_H_

#include <Ogre.h>
#include <cstdlib>

class Target
{
protected:
	Ogre::SceneNode* rootNode;
public:
	Target(Ogre::SceneManager* scnMgr);
	~Target();
};

#endif
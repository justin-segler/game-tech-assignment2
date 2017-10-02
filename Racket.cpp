

Racket::Racket(Ogre::SceneManager* scnMgr) 
{ 
	Ogre::Entity* racket = scnMgr->createEntity("racket.mesh"); 
	racket->setMaterialName("Racket"); 
	racket->setCastShadows(true); 
	rootNode = scnMgr->createSceneNode(); 
	rootNode->attachObject(racket); 
	rootNode->scale(10.0f, 1.0f, 10.0f); 
	rootNode->setPosition(0.0, 0.0, 0.0);
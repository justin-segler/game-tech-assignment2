#include "Gui.h"

 
Gui::Gui(): score(0)
{
}

/* Creates CEGUI renderer */
void Gui::setup(void){
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(false);
}

/* Creates CEGUI windows */
void Gui::createWindow(void){
	
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "root");

	// Initializes title
	CEGUI::Window *title = wmgr.createWindow("TaharezLook/Titlebar", "title");
	title->setText("  The Racket Game");
	title->setSize(CEGUI::USize(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
	title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(title);

	// Initializes score
	wScore = wmgr.createWindow( "TaharezLook/StaticText", "score" );
	wScore->setText("   Score: " +  std::to_string(score));
	wScore->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
	sheet->addChild(wScore);

	// Initializes reset text
	wResetText = wmgr.createWindow( "TaharezLook/StaticText", "resetText" );
	wResetText->setText("    To reset the game, press spacebar.");
	wResetText->setSize(CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
	wResetText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(wResetText);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

}

/*creates single player gui*/ 
void Gui::createSingle(void){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "root");

	// Initializes title
	CEGUI::Window *title = wmgr.createWindow("TaharezLook/Titlebar", "title");
	title->setText("  The Racket Game");
	title->setSize(CEGUI::USize(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
	title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(title);

	// Initializes score
	wScore = wmgr.createWindow( "TaharezLook/StaticText", "score" );
	wScore->setText("   Score: " +  std::to_string(score));
	wScore->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
	sheet->addChild(wScore);

	// Initializes timer
	wTime = wmgr.createWindow( "TaharezLook/StaticText", "time" );
	wTime->setText("   Time: " +  std::to_string(mTime));
	wTime->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
	wTime->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(wTime); 

	// Initializes reset text
	wResetText = wmgr.createWindow( "TaharezLook/StaticText", "resetText" );
	wResetText->setText("    To reset the game, press spacebar.");
	wResetText->setSize(CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
	wResetText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(wResetText);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

/*creates multiplayer gui */ 
void Gui::createMultiplayer()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "root");

	// Initializes title
	CEGUI::Window *title = wmgr.createWindow("TaharezLook/Titlebar", "title");
	title->setText("  The Racket Game");
	title->setSize(CEGUI::USize(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
	title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(title);

	// Initializes score
	wScore = wmgr.createWindow( "TaharezLook/StaticText", "player" );
	wScore->setText("   Score: " +  std::to_string(score));
	wScore->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
	sheet->addChild(wScore);

	// Initializes timer
	wTime = wmgr.createWindow( "TaharezLook/StaticText", "time" );
	wTime->setText("   Time: " +  std::to_string(mTime));
	wTime->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
	wTime->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(wTime); 

	// Initializes reset text
	wResetText = wmgr.createWindow( "TaharezLook/StaticText", "resetText" );
	wResetText->setText("    To reset the game, press spacebar.");
	wResetText->setSize(CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
	wResetText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(wResetText);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}
void Gui::createMultiplayer(std::string ip)
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "root");

	// Initializes title
	CEGUI::Window *title = wmgr.createWindow("TaharezLook/Titlebar", "title");
	title->setText("  The Racket Game");
	title->setSize(CEGUI::USize(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
	title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(title);

	// Initializes score
	wScore = wmgr.createWindow( "TaharezLook/StaticText", "player" );
	wScore->setText("   Score: " +  std::to_string(score));
	wScore->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
	sheet->addChild(wScore);

	// Initializes timer
	wTime = wmgr.createWindow( "TaharezLook/StaticText", "time" );
	wTime->setText("   Time: " +  std::to_string(mTime));
	wTime->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
	wTime->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(wTime); 

	// Initializes waiting screen
	wWaiting = wmgr.createWindow("TaharezLook/StaticText", "waiting");
	wWaiting->setText("IP: " + ip + "\n\nWaiting for client...\n\n\nPress ESC to quit");
	wWaiting->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.25, 0)));
	wWaiting->setPosition(CEGUI::UVector2(CEGUI::UDim(0.375, 0), CEGUI::UDim(0.375, 0)));
	wWaiting->setProperty("HorzFormatting","HorzCentred");
	sheet->addChild(wWaiting);

	// Initializes reset text
	wResetText = wmgr.createWindow( "TaharezLook/StaticText", "resetText" );
	wResetText->setText("    To reset the game, press spacebar.");
	wResetText->setSize(CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
	wResetText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(wResetText);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

/* Destroyes all the windows */ 
void Gui::destroyWindow(void){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	wmgr.destroyAllWindows();
}
/* End game screen */
void Gui::createEnd(void){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "root");
	CEGUI::Window *end = wmgr.createWindow("TaharezLook/StaticText", "endGame");
	CEGUI::Window *label = wmgr.createWindow("TaharezLook/Label", "endText");
	label->setText("The End");
	label->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
	label->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.3, 0)));
	end->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
	end->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.3, 0)));
	sheet->addChild(end);
	sheet->addChild(label);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

// Destroys waiting screen
void Gui::connected(void){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	wmgr.destroyWindow(wWaiting);
}


/* Increments Score */
int Gui::increaseScore(void){
	score++;
	wScore->setText("   Score: " + std::to_string(score));
	return score;

}

/* Increases Score */
int Gui::increaseScore(int num){
	score+=num;
	wScore->setText("   Score: " + std::to_string(score));
	return score;

}

/* Decreases Score */
int Gui::decreaseScore(void){
	score--;
	wScore->setText("   Score: " + std::to_string(score));
	return score;
}

/* Resets Score */
void Gui::resetScore(){
	score = 0;
	wScore->setText("   Score: " + std::to_string(score));
}

/* Sets the score */
int Gui::setScore(int s){
	score = s;
	wScore->setText("   Score: " + std::to_string(score));
	return score;
}


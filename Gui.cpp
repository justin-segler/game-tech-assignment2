#include "Gui.h"

 
Gui::Gui(void): mRenderer(0),score(0)
{
}

/* Creates CEGUI renderer */
void Gui::createRender(void){
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
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
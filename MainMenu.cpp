#include "MainMenu.h"
#include <CEGUI/SchemeManager.h>
#include <CEGUI/FontManager.h>
#include <iostream>
 
MainMenu::MainMenu(void): mRenderer(0)
{
}

/* Creates CEGUI renderer */
void MainMenu::createRender(void){
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
}

/* Creates CEGUI windows */
void MainMenu::createWindow(void){
	
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "root");

	// Menu Background
	CEGUI::Window* MenuBackground = wmgr.createWindow("TaharezLook/StaticImage", "Background");
	MenuBackground->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	MenuBackground->setSize(CEGUI::USize(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
	sheet->addChild( MenuBackground );  // full screen

	CEGUI::Window *title = wmgr.createWindow("TaharezLook/Titlebar", "title");
	title->setText("                           The Racket Game");
	title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
	title->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.15, 0)));
	title->setMargin(CEGUI::UBox(CEGUI::UDim(0, 0), CEGUI::UDim(.2, 0), CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	MenuBackground->addChild(title);
	 
	// New game Button 
	CEGUI::PushButton* SinglePlayer = (CEGUI::PushButton*) wmgr.createWindow("TaharezLook/Button", "SinglePlayer");
	SinglePlayer->setText("Single Player");
	SinglePlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.4, 0)));
	SinglePlayer->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
	SinglePlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::singlePlayerPress, this));
	MenuBackground->addChild( SinglePlayer );
	 
	// Load game Button 
	CEGUI::PushButton* MultiPlayer = (CEGUI::PushButton*) wmgr.createWindow("TaharezLook/Button", "MultiPlayer");
	MultiPlayer->setText("Multi Player");
	MultiPlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.55, 0)));
	MultiPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
	MenuBackground->addChild( MultiPlayer );
	 
	// Quit game Button 
	CEGUI::PushButton* QuitGame = (CEGUI::PushButton*) wmgr.createWindow("TaharezLook/Button", "QuitGame");
	QuitGame->setText("Quit Game");
	QuitGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.7, 0)));
	QuitGame->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
	MenuBackground->addChild( QuitGame );
	 
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

}

/* Destroyes all the windows */ 
void MainMenu::destroyWindow(void){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	wmgr.destroyAllWindows();
}

void MainMenu::singlePlayerPress(void) {
	std::cout << "here" << std::endl;
}

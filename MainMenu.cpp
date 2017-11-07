#include "MainMenu.h"
#include <CEGUI/SchemeManager.h>
#include <CEGUI/FontManager.h>
#include <iostream>
 
MainMenu::MainMenu(): mRenderer(0)
{
}

/* Creates CEGUI renderer */
void MainMenu::setup(void){
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

	// Title
	CEGUI::Window *title = wmgr.createWindow("TaharezLook/Titlebar", "title");
	title->setText("                           The Racket Game");
	title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
	title->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.15, 0)));
	title->setMargin(CEGUI::UBox(CEGUI::UDim(0, 0), CEGUI::UDim(.2, 0), CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	MenuBackground->addChild(title);
	 
	// New game Button 
	CEGUI::PushButton* SinglePlayer = (CEGUI::PushButton*) wmgr.createWindow("TaharezLook/Button", "SinglePlayer");
	SinglePlayer->setText("Single Player");
	SinglePlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.3, 0)));
	SinglePlayer->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
	SinglePlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::singlePlayerPress, this));
	MenuBackground->addChild( SinglePlayer );
	
	// Header
	CEGUI::Window* multiplayerHeader = wmgr.createWindow( "TaharezLook/StaticText", "MultiPlayerHeader" );
	multiplayerHeader->setText("Multi Player:");
	multiplayerHeader->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.45, 0)));
	multiplayerHeader->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.2, 0)));
	multiplayerHeader->setProperty("VertFormatting","TopAligned");
	multiplayerHeader->setEnabled(false);
	MenuBackground->addChild(multiplayerHeader);

	// Host game Button 
	CEGUI::PushButton* MultiPlayer = (CEGUI::PushButton*) wmgr.createWindow("TaharezLook/Button", "MultiPlayer");
	MultiPlayer->setText("Host Server");
	MultiPlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3333, 0), CEGUI::UDim(0.51, 0)));
	MultiPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.3333, 0), CEGUI::UDim(0.05, 0)));
	MultiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::multiPlayerPress, this));
	MenuBackground->addChild( MultiPlayer );

	// Join game Button 
	CEGUI::PushButton* joinMulti = (CEGUI::PushButton*) wmgr.createWindow("TaharezLook/Button", "JoinMulti");
	joinMulti->setText("Join");
	joinMulti->setPosition(CEGUI::UVector2(CEGUI::UDim(0.59, 0), CEGUI::UDim(0.59, 0)));
	joinMulti->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
	joinMulti->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::multiPlayerJoinPress, this));
	MenuBackground->addChild( joinMulti );
	
	// IP label
	CEGUI::Window* ipLabel = wmgr.createWindow( "TaharezLook/StaticText", "IPLabel" );
	ipLabel->setText("IP:");
	ipLabel->setPosition(CEGUI::UVector2(CEGUI::UDim(0.32, 0), CEGUI::UDim(0.585, 0)));
	ipLabel->setSize(CEGUI::USize(CEGUI::UDim(0.26, 0), CEGUI::UDim(0.06, 0)));
	ipLabel->setEnabled(false);
	MenuBackground->addChild(ipLabel);

	// Host text
	hostText = (CEGUI::Editbox*) wmgr.createWindow("TaharezLook/Editbox", "HostText");
	hostText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.59, 0)));
	hostText->setSize(CEGUI::USize(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.05, 0)));
	MenuBackground->addChild(hostText);

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

// Event handler for single player button press
void MainMenu::singlePlayerPress(void) 
{
	destroyWindow();
	state = MM_SP;
}

// Event handler for multi player button press
void MainMenu::multiPlayerPress(void)
{
	destroyWindow();
	state = MM_Host;
}

// Event handler for join button press
void MainMenu::multiPlayerJoinPress(void)
{
	state = MM_Join;
}

// Event handler for get host text
std::string MainMenu::getHostText(void)
{
	return hostText->getText().c_str();
}
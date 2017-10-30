#include "MainMenu.h"
#include <CEGUI/SchemeManager.h>
#include <CEGUI/FontManager.h>
 
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
	 
	// New game Button 
	CEGUI::PushButton* NewGame = (CEGUI::PushButton*) wmgr.createWindow("TaharezLook/Button", "NewGame");
	NewGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
	NewGame->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.2, 0)));
	MenuBackground->addChild( NewGame );
	 
	// Load game Button 
	CEGUI::PushButton* LoadGame = (CEGUI::PushButton*) wmgr.createWindow("TaharezLook/Button", "LoadGame");
	LoadGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.45, 0)));
	LoadGame->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.2, 0)));
	MenuBackground->addChild( LoadGame );
	 
	// Quit game Button 
	CEGUI::PushButton* QuitGame = (CEGUI::PushButton*) wmgr.createWindow("TaharezLook/Button", "QuitGame");
	QuitGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.7, 0)));
	QuitGame->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.2, 0)));
	MenuBackground->addChild( QuitGame );
	 
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

}

/* Destroyes all the windows */ 
void MainMenu::destroyWindow(void){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	wmgr.destroyAllWindows();
}

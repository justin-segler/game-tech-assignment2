#include "Gui.h"


Gui::Gui(void): mRenderer(0),score(0)
{
}

void Gui::createRender(void){
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
}
void Gui::createWindow(void){
	
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "root");

	CEGUI::Window *title = wmgr.createWindow("TaharezLook/Titlebar", "title");
	title->setText("Game");
	title->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
	title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0, 0)));
	sheet->addChild(title);

	wScore = wmgr.createWindow( "TaharezLook/StaticText", "score" );
	wScore->setText("Score: " +  std::to_string(score));
	wScore->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
	sheet->addChild(wScore);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

	
}
int Gui::increaseScore(void){
	score++;
	wScore->setText("Score: " + std::to_string(score));
	return score;

}
int Gui::increaseScore(int num){
	score+=num;
	wScore->setText("Score: " + std::to_string(score));
	return score;

}
int Gui::decreaseScore(void){
	score--;
	wScore->setText("Score: " + std::to_string(score));
	return score;
}
void Gui::resetScore(){
	score = 0;
	wScore->setText("Score: " + std::to_string(score));
}
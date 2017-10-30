#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/widgets/PushButton.h>

class MainMenu{
public:
	MainMenu(void);
	void createRender(void);
	void createWindow(void);
	void destroyWindow(void);
	void singlePlayerPress(void);

protected:
	CEGUI::OgreRenderer* mRenderer;
	CEGUI::PushButton* 	 NewGame;

};
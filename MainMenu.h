#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/widgets/PushButton.h>

#define MAIN_MENU     0
#define SINGLE_PLAYER 1
#define MULTI_PLAYER  2

class MainMenu{
public:
	MainMenu();
	void setup(void);
	void createWindow(void);
	void destroyWindow(void);
	void singlePlayerPress(void);
	void multiPlayerPress(void);
	int state = MAIN_MENU;

protected:
	CEGUI::OgreRenderer* mRenderer;

};
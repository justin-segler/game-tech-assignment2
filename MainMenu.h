#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/widgets/PushButton.h>

enum MenuState{
	MM_Menu, MM_SP, MM_Host, MM_Join
};

class MainMenu{
public:
	MainMenu();
	void setup(void);
	void createWindow(void);
	void destroyWindow(void);
	void singlePlayerPress(void);
	void multiPlayerPress(void);
	void multiPlayerJoinPress(void);
	MenuState state = MM_Menu;

protected:
	CEGUI::OgreRenderer* mRenderer;
	CEGUI::Editbox* hostText;

};
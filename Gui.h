#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>


class Gui{
public:
	Gui(void);
	void createRender(void);
	void createWindow(void);
	int increaseScore(void);
	int decreaseScore(void);
	void resetScore(void);
protected:
	CEGUI::OgreRenderer* mRenderer;
	CEGUI::Window* wScore;
	int score;


};
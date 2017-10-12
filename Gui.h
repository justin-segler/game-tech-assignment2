#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>


class Gui{
public:
	Gui(void);
	void createRender(void);
protected:
	CEGUI::OgreRenderer* mRenderer;

};
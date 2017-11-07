#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>


class Gui{
public:
	Gui();
	void setup(void);
	void createWindow(void);
	void createSingle(void);
	void createMultiplayer();
	void createMultiplayer(std::string ip);
	void destroyWindow(void);
	void createEnd(void);
	int increaseScore(void);
	int increaseScore(int);
	int decreaseScore(void);
	void resetScore(void);
	int setScore(int s);
	void connected(void);
	int mTime = 60;
	CEGUI::Window* wTime;
	int score;
protected:
	CEGUI::Window* wScore;
	CEGUI::Window* wScore2;
	CEGUI::Window* wResetText;
	CEGUI::Window* wWaiting;



};
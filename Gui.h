#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>


class Gui{
public:
	Gui();
	void setup(void);
	void createWindow(void);
	void createSingle(void);
	void createMultiplayer(void);
	void destroyWindow(void);
	void createEnd(void);
	int increaseScore(void);
	int increaseScore(int);
	int decreaseScore(void);
	void resetScore(void);
	int increasePlayer2(void);
	int increasePlayer2(int);
	int decreasePlayer2(void);
	int mTime = 60;
	CEGUI::Window* wTime;
protected:
	CEGUI::Window* wScore;
	CEGUI::Window* wScore2;
	CEGUI::Window* wResetText;
	int score;
	int score2;



};
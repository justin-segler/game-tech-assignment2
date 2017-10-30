noinst_HEADERS = Game.h Racket.h SoundManager.h Target.h Gui.h NetManager.h MainMenu.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir) -std=c++11
assignment2_SOURCES = Game.cpp Racket.cpp SoundManager.cpp Target.cpp Gui.cpp NetManager.cpp MainMenu.cpp
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(OIS_CFLAGS) $(SDL_CFLAGS) -lSDL -lSDL_mixer $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(SDL_net_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(SDL_net_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system -lCEGUIOgreRenderer-0 -R/lusr/opt/cegui-0.8.7

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign

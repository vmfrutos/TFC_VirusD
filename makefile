EXEC := virusD

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/

CXX := g++

# Flags de compilación -----------------------------------------------
CXXFLAGS := -I $(DIRHEA) -Wall -DLINUX -std=c++11
CXXFLAGS += `pkg-config --cflags OGRE`
CXXFLAGS += `pkg-config --cflags OIS`
CXXFLAGS += `pkg-config	--cflags OgreBullet` 
CXXFLAGS += `pkg-config --cflags bullet`
CXXFLAGS += `pkg-config --cflags CEGUI`

# Flags del linker ---------------------------------------------------
LDFLAGS += `pkg-config --libs-only-L OgreBullet`
LDFLAGS += `pkg-config --libs-only-L bullet`
LDFLAGS += `pkg-config --libs-only-L CEGUI`
LDFLAGS += `pkg-config --libs-only-L CEGUI-OGRE`

LDLIBS := `pkg-config --libs-only-l OGRE`
LDLIBS += `pkg-config --libs-only-l OIS`
LDLIBS += `pkg-config --libs-only-l OgreBullet`
LDLIBS += `pkg-config --libs-only-l bullet`
LDLIBS += `pkg-config --libs-only-l CEGUI`
LDLIBS += `pkg-config --libs-only-l CEGUI-OGRE`
LDLIBS += -lGL
LDLIBS += -lstdc++
LDLIBS += -lConvexDecomposition
LDLIBS += -lSDL
LDLIBS += -lSDL_mixer



# Modo de compilación (-mode=release -mode=debug) --------------------
ifeq ($(mode), release) 
	CXXFLAGS += -O2 -D_RELEASE
else 
	CXXFLAGS += -g -D_DEBUG
	mode := debug
endif

# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))
	

.PHONY: all clean

all: info $(EXEC)

info:

	@echo '------------------------------------------------------'
	@echo '>>> Using mode $(mode)'
	@echo '    (Please, call "make" with [mode=debug|release])  '
	@echo '------------------------------------------------------'

# Enlazado -----------------------------------------------------------
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilación --------------------------------------------------------
$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.log $(EXEC) *~ $(DIROBJ)* $(DIRSRC)*~ $(DIRHEA)*~ 

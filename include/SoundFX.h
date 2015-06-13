#ifndef __SOUNDFXH__
#define __SOUNDFXH__

#include <SDL/SDL_mixer.h>
#include <OGRE/Ogre.h>
#include <OGRE/OgreResourceManager.h>

// Clase para mezclar efectos de sonido.
// Delega en SDL mixer.
class SoundFX: public Ogre::Resource {
 public:
  // Constructor (ver Ogre::Resource).
  SoundFX(Ogre::ResourceManager* creator,
	  const Ogre::String& resource_name,
	  Ogre::ResourceHandle handle,
	  const Ogre::String& resource_group,
	  bool isManual = false,
	  Ogre::ManualResourceLoader* loader = 0);
  
  ~SoundFX();

  int play(int loop = 0);
  int play(int channel, int loop);
  void stop (int channel);
  bool isPlaying (int channel);
        
 protected:
  void loadImpl();
  void unloadImpl();
  size_t calculateSize() const;
  
 private:
  Mix_Chunk* _pSound; // Info sobre el efecto de sonido.
  Ogre::String _path; // Ruta completa al efecto de sonido.
  size_t _size;       // Tamaño del efecto (bytes).
};


class SoundFXPtr: public Ogre::SharedPtr<SoundFX> {
 public:
  // Es necesario implementar constructores y operador de asignación.
  SoundFXPtr(): Ogre::SharedPtr<SoundFX>() {}
  explicit SoundFXPtr(SoundFX* s): Ogre::SharedPtr<SoundFX>(s) {}
  SoundFXPtr(const SoundFXPtr& s): Ogre::SharedPtr<SoundFX>(s) {}
  SoundFXPtr(const Ogre::ResourcePtr& r);

  SoundFXPtr& operator= (const Ogre::ResourcePtr& r);
};

#endif

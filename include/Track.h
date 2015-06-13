#ifndef __TRACKH__
#define __TRACKH__

#include <SDL/SDL_mixer.h>
#include <OGRE/Ogre.h>

class Track : public Ogre::Resource {
 public:
  // Constructor (ver Ogre::Resource).
  Track (Ogre::ResourceManager* pManager,
	 const Ogre::String& resource_name,
	 Ogre::ResourceHandle handle,
	 const Ogre::String& resource_group,
	 bool manual_load = false,
	 Ogre::ManualResourceLoader* pLoader = 0);
  ~Track ();

  // Manejo básico del track.
  void play (bool loop = -1);
  void pause ();
  void stop ();
  
  void fadeIn (int ms, int loop);
  void fadeOut (int ms);
  static bool isPlaying ();

 private:
  // Funcionalidad de Ogre::Resource.
  void loadImpl ();   
  void unloadImpl (); 
  size_t calculateSize () const;

  // Variables miembro.
  Mix_Music* _pTrack; // SDL
  Ogre::String _path; // Ruta al track.
  size_t _size;       // Tamaño.
};

// Smart pointer a Track.
class TrackPtr: public Ogre::SharedPtr<Track> {
 public:
  // Es necesario implementar constructores y operador de asignación.
 TrackPtr(): Ogre::SharedPtr<Track>() {}
 explicit TrackPtr(Track* m): Ogre::SharedPtr<Track>(m) {}
 TrackPtr(const TrackPtr &m): Ogre::SharedPtr<Track>(m) {}
 TrackPtr(const Ogre::ResourcePtr &r);
 TrackPtr& operator= (const Ogre::ResourcePtr& r);
};

#endif

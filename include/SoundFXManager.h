#ifndef __SOUNDFXMANAGERH__
#define __SOUNDFXMANAGERH__

#include <OGRE/Ogre.h>
#include "SoundFX.h"

// Clase encargada de gestionar recursos del tipo "SoundFX".
// Funcionalidad heredada de Ogre::ResourceManager
// y Ogre::Singleton.
class SoundFXManager: public Ogre::ResourceManager,
                      public Ogre::Singleton<SoundFXManager> {
 public:
  SoundFXManager();
  virtual ~SoundFXManager();

  virtual SoundFXPtr load(const Ogre::String& name,
			  const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  static SoundFXManager& getSingleton();
  static SoundFXManager* getSingletonPtr();

  static int getAvailableChannels();
  
 protected:
  Ogre::Resource* createImpl(const Ogre::String& name,
			     Ogre::ResourceHandle handle,
			     const Ogre::String& group,
			     bool isManual,
			     Ogre::ManualResourceLoader* loader,
			     const Ogre::NameValuePairList* createParams);
    
 private:
  static int _numChannels;
};

#endif

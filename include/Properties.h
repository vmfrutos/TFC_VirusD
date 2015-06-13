#ifndef Properties_H
#define Properties_H

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <algorithm>
#include <Ogre.h>

using namespace std;

class Properties : public Ogre::Singleton<Properties>{

public:
	Properties();
	~Properties();

	bool load(string file);
	string getPropertyStr(string property) { return _hashMap[property];}
	int getPropertyInt(string property) { return Ogre::StringConverter::parseInt(_hashMap[property]);}
	float getPropertyFloat(string property) { return Ogre::StringConverter::parseReal(_hashMap[property]);}
	bool getPropertyBool(string property);
	Ogre::Vector3 getPropertyVector(string property);
	std::vector<string> getKeys();

	// Métodos heredados de Ogre::Singleton
	static Properties& getSingleton ();
	static Properties* getSingletonPtr ();

private:

	std::map<string, string> _hashMap;
};

#endif

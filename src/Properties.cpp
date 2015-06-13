#include "Properties.h"


template<> Properties* Ogre::Singleton<Properties>::msSingleton = 0;

Properties::Properties() {

}
Properties::~Properties() {

}

/**
 * Returna true en caso de exito
 * y false si no encuentra el fichero
 */
bool
Properties::load(string file) {

	// Se limpia el hasmap
	_hashMap.clear();

	string line;
	ifstream myfile (file);

	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			// Se mira si no es un comentario
			if (line.substr(0,2) != "//" && line.substr(0,1) != "#") {

				// Se busca el igual
				std::size_t found = line.find("=");
				if (found!=std::string::npos){
					// Se ha encontrado el =
					string clave = line.substr(0,found);
					string valor = line.substr(found+1);
					_hashMap[clave] = valor;
				}

			}

		}
		myfile.close();
	} else {
		return false;
	}

	return true;

}

vector<string>
Properties::getKeys(){
	vector<string> keys;
	keys.reserve(_hashMap.size());

	for (map<string, string>::iterator iter = _hashMap.begin();
			iter != _hashMap.end();
			++iter)
	{
		keys.push_back(iter->first);
	}
	return keys;
}

Properties*
Properties::getSingletonPtr ()
{
	std::cout << "Properties::getSingletonPtr" << std::endl;
	if (msSingleton == 0) {
		std::cout << "Properties::getSingletonPtr----->>>> msSingleton == 0" << std::endl;
	}
	return msSingleton;
}

Properties&
Properties::getSingleton ()
{
	assert(msSingleton);
	return *msSingleton;
}

Ogre::Vector3
Properties::getPropertyVector(string property) {
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	Ogre::Vector3 retorno;
	string value = _hashMap[property];
	std::size_t found = value.find(",");
	if (found != std::string::	npos){
		// Se ha encontrado la ,
		x = Ogre::StringConverter::parseReal(value.substr(0,found));
		std::size_t found2 = value.find(",",found+1);
		if (found2 != std::string::	npos){
			y = Ogre::StringConverter::parseReal(value.substr(found+1,found2));
			z = Ogre::StringConverter::parseReal(value.substr(found2+1));

		}
	}

	retorno.x = x;
	retorno.y = y;
	retorno.z = z;
	return retorno;

}

bool
Properties::getPropertyBool(string property){
	string value = _hashMap[property];
	string upperValue;
	std::transform(value.begin(), value.end(), std::back_inserter(upperValue),::toupper);
	return upperValue == "TRUE";
}

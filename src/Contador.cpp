#include "Contador.h"

Contador::Contador() {
	_segundosTranscurridos = 0;
	_valorInicial = _segundosTranscurridos;
	_horas = 0;
	_minutos = 0;
	_segundos = 0;
}

Contador::Contador(Ogre::Real segundos) {
	_segundosTranscurridos = segundos;
	_valorInicial = _segundosTranscurridos;
	_horas = _segundosTranscurridos/3600;

	long minutosrestantes = static_cast<long>(_segundosTranscurridos)%3600;
	_minutos = minutosrestantes/60;
	_segundos = minutosrestantes%60;
}

Contador::~Contador(){

}
void Contador::incrementar(Ogre::Real segundosTranscurridos){
	_segundosTranscurridos += segundosTranscurridos;
	_horas = _segundosTranscurridos/3600;

	long minutosrestantes = static_cast<long>(_segundosTranscurridos)%3600;
	_minutos = minutosrestantes/60;
	_segundos = minutosrestantes%60;


}

void Contador::decrementar(Ogre::Real segundosTranscurridos){
	_segundosTranscurridos -= segundosTranscurridos;
	_horas = _segundosTranscurridos/3600;

	long minutosrestantes = static_cast<long>(_segundosTranscurridos)%3600;
	_minutos = minutosrestantes/60;
	_segundos = minutosrestantes%60;
}

int Contador::getHoras(){
	return _horas;
}
int Contador::getMinutos(){
	return _minutos;

}
int Contador::getSegundos(){
	return _segundos;
}

void Contador::reset(){
	_segundosTranscurridos = 0;
	_valorInicial = _segundosTranscurridos;
	_horas = _segundosTranscurridos/3600;

	long minutosrestantes = static_cast<long>(_segundosTranscurridos)%3600;
	_minutos = minutosrestantes/60;
	_segundos = minutosrestantes%60;
}

void Contador::reset(Ogre::Real segundos){
	_segundosTranscurridos = segundos;
	_valorInicial = _segundosTranscurridos;
	_horas = _segundosTranscurridos/3600;

	long minutosrestantes = static_cast<long>(_segundosTranscurridos)%3600;
	_minutos = minutosrestantes/60;
	_segundos = minutosrestantes%60;
}

Ogre::Real Contador::getSegundosTranscurridos(){
	return _segundosTranscurridos;
}

string Contador::getContadorStr(){
	std::ostringstream formato;
	std::ostringstream horaStr;
	std::ostringstream minStr;
	std::ostringstream segStr;
	if (getHoras() < 10) {
		horaStr << "0" << getHoras();
	} else {
		horaStr << getHoras();
	}

	if (getMinutos() < 10) {
		minStr << "0" << getMinutos();
	} else {
		minStr << getMinutos();
	}

	if (getSegundos() < 10) {
		segStr << "0" << getSegundos();
	}  else {
		segStr << getSegundos();
	}

	formato << horaStr.str() << ":" << minStr.str() << ":" << segStr.str();
	return formato.str();

}

string Contador::getContadorMinSecStr(){
	std::ostringstream formato;
	std::ostringstream minStr;
	std::ostringstream segStr;

	if (getMinutos() < 10) {
		minStr << "0" << getMinutos();
	} else {
		minStr << getMinutos();
	}

	if (getSegundos() < 10) {
		segStr << "0" << getSegundos();
	}  else {
		segStr << getSegundos();
	}

	formato << minStr.str() << ":" << segStr.str();
	return formato.str();

}


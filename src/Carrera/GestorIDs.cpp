#include "GestorIDs.hpp"
#include "obj3D.hpp"

GestorIDs::GestorIDs() {
	//nodoA= new ISceneNode();
	id = 6;
}

void GestorIDs::setIdentifier(obj3D* nodo, const char* nombre) {
	nodo->setID(id);
	id++;
	nodo->setName(nombre);
}
int GestorIDs::getIDLibre() {
	return id;
}
void GestorIDs::restartID() {
	id = 6;
}

GestorIDs& GestorIDs::instancia() {
	static GestorIDs _instancia;
	return _instancia;
}
#ifndef ESCENAMENU_H
#define ESCENAMENU_H

#include "Escena.hpp"

#include <iostream>
#include <string>

class elementoHud;
class AlSource;

class EscenaMenu : public Escena {
public:
	EscenaMenu();
	~EscenaMenu();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	Escena::tipo_escena comprobarInputs();

	// METODOS GET
	std::string getIpConexion();

	// METODOS SET

private:

	bool pressed = false;
	std::string ipConexion;

	int optionMenu;

	AlSource* fuenteOpcion;
	AlSource* fuenteMenu;

	void initHud();
	elementoHud* fondo;
};

#endif /* ESCENAMENU_H */
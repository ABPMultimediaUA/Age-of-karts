#ifndef ESCENALOBBY_H
#define ESCENALOBBY_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "Escena.hpp"
#include "Timer.hpp"
#include "Server.hpp"

class EscenaLobby : public Escena {
public:
	EscenaLobby(Escena::tipo_escena tipo, std::string ipC);
	~EscenaLobby();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	Escena::tipo_escena comprobarInputs();
	int character_callback(GLFWwindow *window, unsigned int codepoint);
	// METODOS GET
	std::string getIpConexion();

	// METODOS SET

private:
	bool lanzarServer;

	int nElementos; // Numeros para la IP (los ultimos 3)
	int nElementos2;
	std::string ipConexion;
	bool iniciar;
	bool iniciado;
	bool conectado;
	bool lanzado;
	float count;
	bool pressed;
	bool firstInit;
	Timer *time;
	//IGUIFont * fuente; // Fuente del texto a mostrar (irrlicht)
	//stringw texto; // Texto a mostrar en pantalla
	//stringw texto2;
	//IGUIStaticText *textoUI; // Nodo de irrlicht
	//ITexture* logoAOK; // Textura del logo del juego
	bool selection;
	bool offline;
	bool end;
	std::string iphost;
	std::string connecting;
	std::string *infoLobby;
};

#endif /* ESCENALOBBY_H */
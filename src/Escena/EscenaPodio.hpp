#ifndef ESCENAPODIO_H
#define ESCENAPODIO_H

#include "Escena.hpp"
#include "Corredor.hpp"

#include <iostream>
#include <vector>

class obj3D;
class cameraThird;
class TNodo;
class particleSystem;
class Timer;
class Corredor;
class AlSource;

class EscenaPodio : public Escena {
public:

	EscenaPodio(Escena::tipo_escena tipo, std::vector<Corredor::tipo_jugador> jugadores);
	~EscenaPodio();

	virtual void init() override;
	virtual void dibujar() override;
	virtual void limpiar() override;
	virtual void update() override;
	Escena::tipo_escena comprobarInputs();

private:

	float x, y, z, altura, posxPodio, posyPodio, poszPodio, incremento;
	bool pulsadoa, pulsadoz, pulsadow, pulsados, pulsadod, pulsadoc, aumento, decremento;
	obj3D* podio;
	cameraThird* camera;

	//Luces
	std::vector<obj3D*> luces;

	//particulas
	std::vector<particleSystem*> particulas;
	obj3D* objTrol;
	obj3D* primero;
	obj3D* segundo;
	obj3D* tercero;
	bool segundoPuesto;
	bool tercerPuesto;
	bool end;
	bool entra;
	TNodo* animacionPirata;
	TNodo* animacionVikingo;
	TNodo* animacionChino;
	TNodo* animacionGladiador;
	TNodo* animacionElegida;
	Timer* time;
	float tm;
	AlSource* fuentePodio;

};

#endif /* ESCENAPODIO_H */
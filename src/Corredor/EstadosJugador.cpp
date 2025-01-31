#include "EstadosJugador.hpp"

EstadosJugador::EstadosJugador() {
	estado = QUIETO;
	direccion = RECTO;
	objeto = NADA;
	estadoCoche = POR_DEFECTO;
	Inmunidad = NORMAL;
	habilidad = SINHABILIDAD;
}
void EstadosJugador::update() {
	/*
	 TextoPantalla *texto =TextoPantalla::getInstancia();
	 texto->agregar("----------------------");
	 texto->agregar("ESTADO MOVIMIENTO: ");
	 switch (estado){

		 case 0:
		 texto->agregar("QUIETO\n");
		 break;
		 case 1:
		 texto->agregar("AVANZA\n");
		 break;
		 case 2:
		 texto->agregar("MARCHA_ATRAS\n");
		 break;
		 case 3:
		 texto->agregar("FRENA\n");
		 break;
		 case 4:
		 texto->agregar("DERRAPA\n");
		 break;
		 case 5:
		 texto->agregar("DESACELERA\n");
		 break;

	 }
	 texto->agregar("DIRECCION MOVIMIENTO: ");
	 switch (direccion){

		 case 0:
		 texto->agregar("RECTO\n");
		 break;
		 case 1:
		 texto->agregar("IZQUIERDA\n");
		 break;
		 case 2:
		 texto->agregar("DERECHA\n");
		 break;
	 }
	 texto->agregar("OBJETO ACTUAL: ");

	 switch (objeto){

		 case 0:
		 texto->agregar("NADA\n");
		 break;
		 case 1:
		 texto->agregar("FLECHA\n");
		 break;
		 case 2:
		 texto->agregar("CAJA_FALSA\n");
		 break;
		 case 3:
		 texto->agregar("TURBO\n");
		 break;
		 case 4:
		 texto->agregar("ACEITE\n");
		 break;
		 case 5:
		 texto->agregar("ESCUDO\n");
		 break;
		 case 6:
		 texto->agregar("FLECHA TRIPLE\n");
		 break;
		 case 8:
		 texto->agregar("HABILIDAD\n");
		 break;
		 case 7:
		 texto->agregar("ITEM_TELEDIRIGIDO\n");
		 break;

	 }

	 texto->agregar("ESTADO COCHE: ");
	 switch (estadoCoche){
		 case 0:
		 texto->agregar("POR_DEFECTO\n");
		 break;
		 case 1:
		 texto->agregar("CON_TURBO\n");
		 break;
		 case 2:
		 texto->agregar("CON_ESCUDO\n");
		 break;
	 }
	*/
}
void EstadosJugador::setEstadoMovimiento(estado_movimiento estado_mov) {
	estado = estado_mov;
}
void EstadosJugador::setEstadoMovimiento(int estado_mov) {
	switch (estado_mov) {

	case 0:
		estado = QUIETO;
		break;
	case 1:
		estado = AVANZA;
		break;
	case 2:
		estado = MARCHA_ATRAS;
		break;
	case 3:
		estado = FRENA;
		break;
	case 4:
		estado = DERRAPA;
		break;
	case 5:
		estado = DESACELERA;
		break;

	}
}
EstadosJugador::estado_movimiento EstadosJugador::getEstadoMovimiento() {
	return estado;
}
void EstadosJugador::setDireccionMovimiento(direccion_movimiento direccion_mov) {
	direccion = direccion_mov;
}
void EstadosJugador::setDireccionMovimiento(int direccion_mov) {
	switch (direccion_mov) {

	case 0:
		direccion = RECTO;
		break;
	case 1:
		direccion = IZQUIERDA;
		break;
	case 2:
		direccion = DERECHA;
		break;

	}
}
EstadosJugador::direccion_movimiento EstadosJugador::getDireccionMovimiento() {
	return direccion;
}
void EstadosJugador::setEstadoObjeto(estado_objeto estado_obj) {
	objeto = estado_obj;
}
void EstadosJugador::setEstadoObjeto(int estado_obj) {
	objeto = estado_objeto(estado_obj);
}
EstadosJugador::estado_objeto EstadosJugador::getEstadoObjeto() {
	return objeto;
}

void EstadosJugador::setEstadoCoche(estado_coche coche) {
	estadoCoche = coche;
}

void EstadosJugador::setEstadoCoche(int coche) {
	switch (coche) {
	case 0:
		estadoCoche = POR_DEFECTO;
		break;
	case 1:
		estadoCoche = CON_TURBO;
		break;
	case 2:
		estadoCoche = CON_ESCUDO;
		break;
	}
}

EstadosJugador::estado_coche EstadosJugador::getEstadoCoche() {
	return estadoCoche;
}

void EstadosJugador::setEstadoCarrera(estado_carrera carrera) {
	estadoCarrera = carrera;
}

void EstadosJugador::setEstadoCarrera(int carrera) {
	switch (carrera) {
	case 0:
		estadoCarrera = PARRILLA;
		break;
	case 1:
		estadoCarrera = CARRERA;
		break;
	case 2:
		estadoCarrera = FIN;
		break;
	}
}
EstadosJugador::estado_carrera EstadosJugador::getEstadoCarrera() {
	return estadoCarrera;
}

void EstadosJugador::setEstadoInmunidad(estado_inmunidad inmunidad) {
	Inmunidad = inmunidad;
}

void EstadosJugador::setEstadoInmunidad(int inmunidad) {
	switch (inmunidad) {
	case 0:
		Inmunidad = NORMAL;
		break;
	case 1:
		Inmunidad = INMUNIDAD;
		break;
	}
}

EstadosJugador::estado_inmunidad EstadosJugador::getEstadoInmunidad() {
	return Inmunidad;
}

void EstadosJugador::setEstadoHabilidad(estado_habilidad Habilidad) {
	habilidad = Habilidad;
}

void EstadosJugador::setEstadoHabilidad(int Habilidad) {
	switch (Habilidad) {
	case 0:
		habilidad = SINHABILIDAD;
		break;
	case 1:
		habilidad = CONHABILIDAD;
		break;
	}
}

EstadosJugador::estado_habilidad EstadosJugador::getEstadoHabilidad() {
	return habilidad;
}
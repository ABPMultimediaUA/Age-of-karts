#include "GestorCarrera.hpp"

GestorCarrera::GestorCarrera() {
	vueltas = 3;
	jugadores = GestorJugadores::getInstancia();
	pj1 = jugadores->getJugadores();
	jugadores->setJugadores(pj1);
	//pj2 = new Corredor*[6];

}
void GestorCarrera::update() {
	for (int j = 0; j < jugadores->getNumJugadores(); j++) {
		//if (pj1.at(j) != NULL) 
			pj1.at(j)->setPosicionCarrera(pj1.at(j)->getNodo()->getID() + 1);   //Asignamos para empezar la carrera las posiciones de los corredores en parrilla, en función de las ids.
		pj2.resize(pj2.size()+1);
		pj2.at(j) = pj1.at(j);
	}
	int cont = 0;
	pj_aux = NULL;
	//ordenamos el array de jugadores en pj2, en funcion de sus waypoints de mayor a menor
	for (int j = 0; j < jugadores->getNumJugadores() - 1; j++) {
		for (int k = 0; k < jugadores->getNumJugadores() - 1; k++) {
			if (pj2.at(k)->getWaypointActual()->getWaypoint()->getID() < pj2.at(k + 1)->getWaypointActual()->getWaypoint()->getID()) {
				pj_aux = pj2.at(k);
				pj2.at(k) = pj2.at(k + 1);
				pj2.at(k + 1) = pj_aux;
			}
		}
	}
	int aux = 0;
	cont = 0;
	int x = -1;
	int y = -1;
	for (int num = vueltas; num >= 1; num--) {
		pj_aux = NULL;
		x = -1;
		y = -1;

		//una vez ordenados por waypoints pasamos a clasificarlos, distinguiendo distancias entre waypoints en caso de ambiguedad (si estan el = waypoint)
		for (int i = 0; i < jugadores->getNumJugadores(); i++) {
			y = getCorredorIndexOriginal(i);          //cogemos el mismo corredor correspondiente en el array original pj1 (asociar pj1<-->pj2)
			cont = jugadores->getNumJugadores();
			if (num == pj1.at(y)->getVueltas()) {
				aux++;
				for (int j = 0; j < jugadores->getNumJugadores(); j++) {
					if (i != j) {      //si no es el mismo entra
						x = getCorredorIndexOriginal(j);  //hacemos lo mismo para el bucle actual (asociar pj1<-->pj2)
						if (num == pj1.at(x)->getVueltas()) {
							if (pj1.at(x)->getWaypointActual()->getWaypoint()->getID() == pj1.at(y)->getWaypointActual()->getWaypoint()->getID()) {  //si estan en el mismo waypoint
								if (pj1.at(x)->getdistanciaWaypoint() > pj1.at(y)->getdistanciaWaypoint()) {     //si la distancia del waypoint pj1[y] es menor, su posicion en carrera aumenta (cont--)
									cont--;
								}
							}
							else {          //no estan en el mismo waypoint
								if (i < j) {   //si el waypoint de i es mayor (i mas pequenya) entra
									cont--;
								}
							}
						}
						else if (num > pj1.at(x)->getVueltas()) {
							cont--;
						}
					}
				}


				pj1.at(y)->setPosicionCarrera(cont);       //modificamos posicion en pj1[y]
			}
		}
	}
}

int GestorCarrera::getCorredorIndexOriginal(int n) {
	for (int i = 0; i < jugadores->getNumJugadores(); i++) {
		if (pj2.at(n)->getNodo()->getID() == pj1.at(i)->getNodo()->getID()) {
			return i;
		}
	}

	return -1;
}

void GestorCarrera::setVueltas(int i) {
	vueltas = i;
}

int GestorCarrera::getVueltas() {
	return vueltas;
}
void GestorCarrera::comprobarItemTeledirigido(int num){ //aplicar el teledirigido al siguiente corredor
    for (int i = 0; i < jugadores->getNumJugadores(); i++) {
        if (num!=1){
             if (pj1.at(i)->getPosicionCarrera()==num-1){
				pj1.at(i)->setObjetivoTelederigido();
                //return true;
            }
        }
    } 
}

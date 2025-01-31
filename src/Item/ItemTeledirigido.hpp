#ifndef ITEMTELEDIRIGIDO_H
#define ITEMTELEDIRIGIDO_H

#include "Item.hpp"

class Waypoint;

class ItemTeledirigido : public Item
{
public:

	ItemTeledirigido(btVector3 posicion, btVector3 escala, btScalar masa, 
		float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio,
		float alturaLanzamiento, int idNodo);

	~ItemTeledirigido();

	virtual void lanzarItem(int direccion, btVector3 orientacion, btTransform objeto) override;
	virtual void updateHijos() override;
	void setWaypoint(Waypoint* waypoint);
	void ActualizarRaytest();
	void movimiento();
	void direccionGiro(btVector3 posicion);
	void setWaypointActual();

private:

	int auxiliar;
	btScalar anguloGiro;
	bool enemigo;
	btVector3 PosEnemigo;
	Waypoint* actual; // Punto Actual
	Waypoint* siguiente; // Punto Siguiente
	btVector3 orientacion; //orientacio OBJ
};

#endif

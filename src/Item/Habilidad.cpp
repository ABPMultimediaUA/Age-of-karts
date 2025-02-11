#include "Habilidad.hpp"
#include "TMotor.hpp"
#include "GestorIDs.hpp"
#include "Obj3D.hpp"

Habilidad::Habilidad(int tipo, btRigidBody* n, btVector3 posicion, btVector3 escala, btScalar masa, float tiempoDesctruccion, forma_Colision fcolision, btVector3 tamanyoNodo, btScalar radio,
	float alturaLanzamiento, int idNodo) : Item(posicion, escala, masa, tiempoDesctruccion, fcolision, tamanyoNodo, radio, alturaLanzamiento, idNodo) {
	//almacenamos tipo de habilidad
	tipoHabilidad = tipo;
	NodoVehiculo = n;
	cont = 0;
	cont2 = 10;
	const char* strEscudo = "assets/escudo/escudo.obj";
	switch (tipoHabilidad) {

	case 1:
		nodo = TMotor::instancia().newMeshNode("HabilidadPirata","assets/Habilidades/Pirata/HabilidadPirata.obj","escena_raiz", false);
		nombre = "HabilidadPirata";
		GestorIDs::instancia().setIdentifier(nodo, "HabilidadPirata");
		break;

	case 2:
		nodo = TMotor::instancia().newMeshNode("HabilidadVikingo", "assets/hacha/hacha.obj", "escena_raiz", false);
		nombre = "HabilidadVikingo";
		GestorIDs::instancia().setIdentifier(nodo, "HabilidadVikingo");
		break;

	case 3:
		nodo = TMotor::instancia().newMeshNode("HabilidadGladiador", "assets/Habilidades/Gladiador/HabGladiador.obj", "escena_raiz", false);
		nombre = "HabilidadGladiador";
		GestorIDs::instancia().setIdentifier(nodo, "HabilidadGladiador");
		break;

	case 4:
		nodo = TMotor::instancia().newMeshNode("HabilidadChino", "assets/Habilidades/Chino/habChino.obj", "escena_raiz", false);
		nombre = "HabilidadChino";
		GestorIDs::instancia().setIdentifier(nodo, "HabilidadChino");
		break;

	}

	nodo->setScale(tamanyoNodo.getX(), tamanyoNodo.getY(), tamanyoNodo.getZ());

	id = nodo->getID();
	nodo->setName(nombre);

	inicializarFisicas();

	if (tipoHabilidad == 4 || tipoHabilidad == 2 || tipoHabilidad == 3) {
		rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		//rigidBody->setGravity(btVector3(0, 0, 0));

	}

}

Habilidad::~Habilidad() {
	NodoVehiculo = nullptr;
}

void Habilidad::lanzarItem(int direccion, btVector3 orientacion, btTransform objeto) {
	this->orientacion = orientacion;

	if (tipoHabilidad == 1) { // PIRATA
		rigidBody->setActivationState(DISABLE_DEACTIVATION);
		rigidBody->setGravity(btVector3(0, -1000, 0));
		rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 410, 0.5f, orientacion.getZ() * 410));
	}
}

void Habilidad::updateHijos() {
	movimiento();
}


void Habilidad::movimiento() {

	btTransform trans;
	btQuaternion quaternion;
	int giro = 0;


	switch (tipoHabilidad) {


	case 2: // VIKINGO



		giro = 15;
		trans.setOrigin(btVector3(NodoVehiculo->getCenterOfMassPosition().getX() + orientacion.getX() * 15, NodoVehiculo->getCenterOfMassPosition().getY() + 5, NodoVehiculo->getCenterOfMassPosition().getZ() + orientacion.getZ() * 15));

		orientacion = orientacion.rotate(btVector3(0, 1, 0), giro * PI / 180);
		nodo->rotate(glm::vec3(0,1,0), cont);
		//quaternion.setEulerZYX(0, nodo->getRotation().y * PI / 180, 0);

		trans.setRotation(NodoVehiculo->getCenterOfMassTransform().getRotation());

		rigidBody->setCenterOfMassTransform(trans);
		rigidBody->setLinearVelocity(btVector3(orientacion.getX() * 100000, 0, orientacion.getZ() * 100000));

		cont += 90;

		if (cont == 360)
			cont = 0;


		break;

	case 3: // GLADIADOR

		nodo->setPosition(NodoVehiculo->getCenterOfMassPosition().getX(), NodoVehiculo->getCenterOfMassPosition().getY(), NodoVehiculo->getCenterOfMassPosition().getZ());

		if (cont2 > 0){
			nodo->setScale(nodo->getScale().x + 2, tamanyoNodo.getY(), nodo->getScale().z + 2);

		}	
		trans.setOrigin(btVector3(NodoVehiculo->getCenterOfMassPosition().getX(), NodoVehiculo->getCenterOfMassPosition().getY() + 2, NodoVehiculo->getCenterOfMassPosition().getZ()));
		trans.setRotation(NodoVehiculo->getCenterOfMassTransform().getRotation());
		rigidBody->setCenterOfMassTransform(trans);

		if (cont2 != 0)
			cont2--;

		break;

	case 4: //CHINICO
		nodo->setPosition(NodoVehiculo->getCenterOfMassPosition().getX(), NodoVehiculo->getCenterOfMassPosition().getY(), NodoVehiculo->getCenterOfMassPosition().getZ());
		trans.setOrigin(btVector3(NodoVehiculo->getCenterOfMassPosition().getX(), NodoVehiculo->getCenterOfMassPosition().getY() + 2, NodoVehiculo->getCenterOfMassPosition().getZ()));
		trans.setRotation(NodoVehiculo->getCenterOfMassTransform().getRotation());
		rigidBody->setCenterOfMassTransform(trans);

		break;
	}
}


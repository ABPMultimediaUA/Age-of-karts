#include "MotorFisicas.hpp"
#include "TMotor.hpp"
#include "TRecursoMalla.hpp"
#include "TGestorRecursos.hpp"

//------------------------------\*
//---CONSTRUCTOR Motorfisicas---\*
//------------------------------\*

MotorFisicas *MotorFisicas::instancia = NULL;

MotorFisicas::MotorFisicas() {
	broadPhase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000)); //limites del mundo
	confColision = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(confColision);
	solver = new btSequentialImpulseConstraintSolver();
	mundo = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, confColision); //creacion del mundo
//	fisicasMapa = new btBulletWorldImporter(mundo);
//	fisicasMapa->loadFile("assets/Mapa01/FisicasMapaIsla.bullet"); //no recoge bien el const char con la ruta del fichero si se le pasas por parametro
}

MotorFisicas::~MotorFisicas() {
	for (int i = mundo->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = mundo->getCollisionObjectArray()[i];
		if (obj) {
			delete obj->getCollisionShape();
			mundo->removeCollisionObject(obj);
			delete obj;
		}
	}

	delete mundo;
	delete solver;
	delete dispatcher;
	delete confColision;
	delete broadPhase;

	objetos.clear();
	instancia = nullptr;
}

MotorFisicas *MotorFisicas::getInstancia() {
	if (instancia == NULL)
		instancia = new MotorFisicas();

	return instancia;
}

btDynamicsWorld *MotorFisicas::getMundo() {
	return mundo;
}

std::vector<btRigidBody *> MotorFisicas::getObjetos() {
	return objetos;
}

void MotorFisicas::setObjetos(std::vector<btRigidBody *> obj) {
	objetos = obj;
}

void MotorFisicas::initializePhysics(TRecursoMalla * mesh) {
	//Las Fisicas se cargan la primera vez que se ejecuta el juego,
	//guardando en un fichero .bullet con el nombre de la malla.
	//El resto de veces se comprobara antes de crear el archivo que no exista su .bullet, de lo contrario se cargara el archivo directamente.
	//cout << "creando las fisicas para :" << mesh->getNombre() << endl;
	std::string fileName = mesh->getNombre();
	fileName = "assets/fisicas/" + fileName;
	fileName += ".bullet";
	FILE * fileBullet;
	fileBullet = fopen(fileName.c_str(), "r");
	if (fileBullet != NULL) {
		//Si detecta fichero se carga, disminuyendo considerablemente la carga del mapa.
		btBulletWorldImporter* fileLoader = new btBulletWorldImporter(mundo);
		fileLoader->loadFile(fileName.c_str());
	}
	else {
		//cout << "El fichero no existe, CREO fisicas" << endl;
		//En caso de que no exista el fichero, se crearan las fisicas 
		std::vector<btRigidBody *> obje = getObjetos();
		// posicion inicial del obj
		btTransform Transform;
		Transform.setIdentity();
		Transform.setOrigin(btVector3(0, 0, 0));
		btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

		//creacion del contorno

		// la masa 0 por ser mapa estatico
		btVector3 LocalInertia;
		btCollisionShape *shape = CreateCollisionShape(mesh->getNombre());


		// Create the rigid body object
		btRigidBody *rigidBody = new btRigidBody(0, MotionState, shape, LocalInertia);

		rigidBody->setActivationState(DISABLE_DEACTIVATION);


		// guardar el rb en el mundo de fisicas
		mundo->addRigidBody(rigidBody);
		obje.push_back(rigidBody);
		setObjetos(objetos);
		this->setObjetos(objetos);

		saveFilePhysics(mesh->getNombre());
	}

}
void MotorFisicas::saveFilePhysics(const char *name) {
	//Guarda el fichero bullet
	btDefaultSerializer* serializer = new btDefaultSerializer();
	mundo->serialize(serializer);
	std::string fileName = name;
	fileName = "assets/fisicas/" + fileName;
	fileName += ".bullet";

	FILE* file = fopen(fileName.c_str(), "wb");
	fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file);
	fclose(file);
}

btCollisionShape  *MotorFisicas::CreateCollisionShape(const char *name) {
	//la escala estara para todos igual
	btVector3 scale(1, 1, 1);
	btCollisionShape  *shape = NULL;
	btTriangleMesh *triangleMesh = new btTriangleMesh{};
	TRecursoMalla *mesh = TMotor::instancia().getGR()->getMalla(name);

	if (mesh != NULL) {
		for (unsigned i{ 0 }; i < mesh->getIndices().size(); i += 3) {
			btVector3 vertex_1{
				mesh->getVertex()[mesh->getIndices()[i]].x,
				mesh->getVertex()[mesh->getIndices()[i]].y,
				mesh->getVertex()[mesh->getIndices()[i]].z };
			btVector3 vertex_2{
				mesh->getVertex()[mesh->getIndices()[i + 1]].x,
				mesh->getVertex()[mesh->getIndices()[i + 1]].y,
				mesh->getVertex()[mesh->getIndices()[i + 1]].z };
			btVector3 vertex_3{
				mesh->getVertex()[mesh->getIndices()[i + 2]].x,
				mesh->getVertex()[mesh->getIndices()[i + 2]].y,
				mesh->getVertex()[mesh->getIndices()[i + 2]].z };
			triangleMesh->addTriangle(vertex_1, vertex_2, vertex_3);
		}
		shape = new btBvhTriangleMeshShape(triangleMesh, true, true);
		shape->setLocalScaling(scale);
	}

	mesh = nullptr;
	triangleMesh = nullptr;

	return shape;
}

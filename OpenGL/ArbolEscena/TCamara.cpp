#include "TCamara.hpp"

TCamara::TCamara() {
	
}
TCamara::~TCamara() {

}

void TCamara::setPerspective(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados) {
	projectionMatrix = glm::perspective(
		glm::radians(grados), 			// El campo de visi�n vertical:zoom 90� (extra ancho) y 30� (zoom aumentado)
		Width / Height,					// Proporci�n ventana 4/3 == 800/600 
		corteCercano,					// Plano de corte cercano. Tan grande como sea posible o tendr�s problemas de precisi�n.
		corteLejano						// Plano de corte lejano. Tan peque�o como se pueda.
	);


}
void TCamara::setParalela(GLfloat Height, GLfloat Width, GLfloat corteCercano, GLfloat corteLejano, GLfloat grados) {
	projectionMatrix = glm::ortho(
		glm::radians(grados), 			// El campo de visi�n vertical:zoom 90� (extra ancho) y 30� (zoom aumentado)
		Width / Height,					// Proporci�n.ventana 4/3 == 800/600
		corteCercano,					// Plano de corte cercano. Tan grande como sea posible o tendr�s problemas de precisi�n.
		corteLejano						// Plano de corte lejano. Tan peque�o como se pueda.
	);

}
//void TCamara::lookAt(glm::vec3 camaraPos, glm::vec3 camaraDir, glm::vec3 camaraAlt) { //Posicion y direccion de la camara
//	glm::mat4 matrisica = glm::lookAt(camaraPos, camaraDir + camaraPos, camaraAlt);
//	viewMatrix = matrisica;
//}
glm::mat4 TCamara::getProjectionMatrix() {
	return projectionMatrix;
}


/*La posicion y orientacion de la camara y de las
luces se maneja a traves de las transformaciones
de nodo padre, como para el resto de entidades.
Las camaras y las luces se manejan
de forma especial, puesto que se pueden encontrar
en cualquier nodo del arbol y, sin embargo, al
dibujar la escena deben definirse antes de dibujar
el resto de entidades.
Los metodos beginDraw y endDraw de las c�maras y las luces suelen estar vacios.*/
void TCamara::beginDraw(Shader *shader) {
	std::cout << "(TCamara::beginDraw) Cargo Camara" << std::endl;

}
void TCamara::endDraw() {

}

bool TCamara::getActive() {
	return active;
}
void TCamara::setActive(bool s) {
	active = s;
}
#include "TMotor.hpp"
#include "GestorEscena.hpp"
#include "EscenaJuego.hpp"
#include "TGestorImagenes.hpp"
#include "Corredor.hpp"


//Callback para el redimensionamiento de la pantalla
void resize_callback(GLFWwindow* window, int width, int height);
int main(int argc, char* argv[]) {

	TMotor::instancia();
	
	//Parametros del motor
	TMotor::instancia().setShaderActive("shaderCartoon"); //Tipo de shader a usar
	TMotor::instancia().setSkyBox(); //Queremos skybox
	TMotor::instancia().setBoundingBoxes(false); //Queremos bounding boxes

	GestorEscena::instancia().cambiaEscena(Escena::tipo_escena::LOBBY);

	//Evento de glfw para redimensionar la pantalla
	glfwSetFramebufferSizeCallback(TMotor::instancia().getVentana(), resize_callback);

	Server::getInstancia()->CreateServerInterface();
	Server::getInstancia()->ServerStartup();

    // -----------------------------//
	//	GAME LOOP
	// -----------------------------//
	Escena::tipo_escena tipoActual = GestorEscena::instancia().getEscenaActiva().getTipoEscena();
	int tipo = 0;
	while (tipoActual != Escena::tipo_escena::SALIR) {
		//Se hace el update de cada escena, que incluye el dibujado de la misma
		tipoActual = GestorEscena::instancia().update();
		//Se llama a la comprobacion de eventos del motor
		TMotor::instancia().getInputs();
		//Intercambiamos buffers
		TMotor::instancia().swapBuffers();
	}
	GestorEscena::instancia().borraEscena(GestorEscena::instancia().getEscenaActiva().getTipoEscena());
	Server::getInstancia()->ShutDownServer();
    delete Server::getInstancia();
    TMotor::instancia().close();

	cout << "FIN DE JUEGO\n";
	return 0;
}

//Manejador para el evento de redimensionamiento de la pantalla
void resize_callback(GLFWwindow* ventana, int ancho, int alto) {
	TMotor::instancia().resizeScreen(ancho,alto);
	//Se redimensiona el viewport de Opengl en funcion de las medidas de la ventana
	glViewport(0, 0, ancho, alto);
}

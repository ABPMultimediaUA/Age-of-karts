FUENTE := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJETOS := $(subst src/,obj/,$(subst .cpp,.o,$(FUENTE)))

LIBRERIAS := -lIrrlicht -lXxf86vm -lGL -lX11 -lSDL -lSDL_image -lGLU -lpthread -lglfw -lGLEW -lassimp
LIBRERIAS_BULLET := 

RUTAS2 := -I. -I/usr/local/include/bullet/
RUTAS := -I./include/irrlicht -I./include/raknet -I./include/irrlicht/include -I./include/bullet -L./lib  -I./include/sdl
RUTAS_INTERNAS := -I./src -I./src/Carrera -I./src/Corredor -I./src/Escena -I./src/Item -I./src/Motor -I./src/ArbolEscena -I./src/GestorRecursos -I./src/Motor -I./src/Shader -I./include/GL -I./include/GLFW -I./include/glm -I./include/assimp -I./include -I./src/Fachada

CFLAGS := -ggdb -std=c++11 
.PHONY: objdir info all 

all: objdir exec

run: objdir exec run

alt: objdir exec2 

runalt: objdir exec2 altRun

exec2: $(OBJETOS)
	@g++ -o $@ $^ $(RUTAS2) $(RUTAS_INTERNAS)  $(LIBRERIAS) $(LIBRERIAS_BULLET) $(CFLAGS) -Wl,-rpath=/usr/local/lib
	@echo "Generado ejecutable. Ejecutar ./$@"

exportAlt: 
	export LD_LIBRARY_PATH="/usr/local/lib"


altRun:
	@./exec2
	@echo "Ejecutando."
	
altRunRed:
	@echo "Ejecutando."
	@./exec2 2

run:
	@./exec
	@echo "Ejecutando."

export: 
	LD_LIBRARY_PATH=./lib
	export LD_LIBRARY_PATH

exec: $(OBJETOS)
	#export RUTA_LIB=${HOME}:${RUTA_LIB}
	#export RUTA_LIB=$(HOME):$(RUTA_LIB)
	#export PATH=$PATH:/usr/local/bin
	#echo "$PATH"
	#sudo cp lib/*.so.2.88 /usr/local/lib
	export RUTA_LIB=/usr/local/lib

	
	g++ -fPIC -g -o $@ $^  $(RUTAS) $(RUTAS_INTERNAS) $(LIBRERIAS) $(LIBRERIAS_BULLET) $(CFLAGS) -Wl,-rpath=./lib	
	@echo "Generado ejecutable."

obj/%.o : src/%.cpp
	@g++ -fPIC -g -o $@ -c $^ $(RUTAS) $(RUTAS_INTERNAS)  $(CFLAGS)
	@echo "Compilado $@."

objdir:
	@mkdir -p obj/
	@mkdir -p obj/Fachada
	@mkdir -p obj/Corredor
	@mkdir -p obj/Escena
	@mkdir -p obj/Item
	@mkdir -p obj/Motor
	@mkdir -p obj/GestorRecursos
	@mkdir -p obj/Shader
	@mkdir -p obj/ArbolEscena
	@echo "Creando carpeta de compilados."

info:
	$(info $(FUENTE))
	$(info $(OBJETOS))

clean:
	@rm -f -r obj/
	@rm -f exec
	@rm -f exec2
	@echo "Limpiando resultado de compilacion."

help: 
	@echo "Comandos utiles en Age Of Karts"
	@echo "- make: para compilar con las librerias del proyecto"
	@echo "- make run: para compilar con make y ejecutar"
	@echo "- make alt: para compilar con las librerias instaladas en el sistema"
	@echo "- make runalt: para compilar con make alt y ejecutar"
	@echo "- make clean: para eliminar la carpeta de objetos y los ejecutables"
	@echo "- make info: para ver la informacion de los archivos de src"

#TODO:
#  Comandos para compilar en release
#  Comandos para compilar para Windows 
#  Buscar en subcarpetas en src como pueda ser src/fachada
#  Comando para compilar y ejecutar de una
# libtarget.so: main.o
#   gcc -shared  -o libtarget.so main.o

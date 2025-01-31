#include "Server.hpp"
#include <iostream>
#include "NetworkEnums.hpp"
#include "CorredorRed.hpp"

Server *Server::instancia = NULL;

#define MAX_PLAYERS 6 //numero maximo de jugadores conectados al servidor

Server *Server::getInstancia() {
	if (instancia == NULL)
		instancia = new Server(6);

	return instancia;
}

//======================================================================
// Se inicializan los parametros que necesita el servidor para funcionar
//======================================================================

Server::Server(int maxPlay)
{
    started = false; //Indica si la carrera ha empezado
    numSockets = 1; //numero de sockets
    numIPs = 1; //numero de ips
    numPlayers = 0; //numero de jugadores iniciales
    numClients = 0; //Numero de clientes conectados
    spawned = false; //false == el servidor aun no ha spwaneado su jugador ; true == ya ha spwaneado su jugador, no puede spwanear mas
    maxPlayers = maxPlay; //asignacion del numero max de jugadores
    serverPort = "6001"; //puerto que va a usar el servidor
	timeStamp = 0;
	allPlayerLoaded = false;
}

//=======================================================================================
// Se crea la interfaz del servidor obteniendo una instancia de la clase RakPeerInterface
//=======================================================================================
void Server::CreateServerInterface()
{
    server = RakNet::RakPeerInterface::GetInstance();
    
    //establecer una contrasenya que deben introducir los clientes que quieran conectarse al servidor
    server->SetIncomingPassword("ageofkarts", (int)strlen("ageofkarts"));
    
    //tiempo que se espera a desconectar el servidor si este no es capaz de realizar ninguna entrega de mensajes
    server->SetTimeoutTime(30000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
    
    //si el server ha fallado en su creacion, se notifica con un error
    if (!server)
    {
        std::cout << "Error N01: Failed to createServer\n";
    }
}

//=======================================================================================
// Puesta en marcha del servidor. En esta funcion, se inicia el servidor de manera 
// que ya es capaz de recibir conexiones de otros clientes y paquetes de datos
//=======================================================================================
void Server::ServerStartup()
{
    //Se establece el puerto para el socket
    socketDescriptor.port = atoi(serverPort.c_str()); 
    
    //el socket utiliza el protocolo IPv4
    socketDescriptor.socketFamily = AF_INET;	
    
    //se pone en marcha el servidor mediante el inicio de los hilos de red y la apertura del puerto de escucha esablecido anteriormente	  
    // Startup(num max de jugadores, descripcion del socket a usar, num de descripciones de sockets);
    bool b = server->Startup(maxPlayers, &socketDescriptor, 1) == RakNet::RAKNET_STARTED;
    
    //si la inicializacion no devuelve el mensaje RAKNET_STARTED, significa que ha fallado y en ese caso se muestra por pantalla
    if (!b)
    {
        std::cout << "Error N03: Failed to Start Server\n";
    }
    
    //se establecen algunos parametros del servidor
    server->SetMaximumIncomingConnections(maxPlayers); //num de conexiones maximas aceptadas
    server->SetOccasionalPing(true); 	//true == se realizan ping ocacionales a los clientes para comprobar, por ejemplo, paquetes perdidos 
    //false == no se hacen pings
    server->SetUnreliableTimeout(1000); //se modifica el tiempo de espera de desconexion del servidor
    
    //se llama al metodo DebugServerInfo() que contiene el resto de inicializacion del servidor
    DebugServerInfo();
}

//============================================================================================
// Complementa a Startup(). Se termina de poner en marcha el servidor obteniendo las 
// direcciones de los sockets usados, la IP del servidor y el identificador unico global GUID
//============================================================================================
void Server::DebugServerInfo()
{
    //se crea una estructura de sockets mediante un tipo List, usando la nueva clase RakNetSocket2
    DataStructures::List<RakNet::RakNetSocket2 *> sockets;
    
    //se obtienen los distintos sockets usados
    server->GetSockets(sockets);
    
    //se notifica por pantalla que el servidor se ha puesto en marcha :)
    std::cout << "Server Started! \n";
    
    //se muestran las direcciones de los sockets que se estan usando mediante la funcion GetBoundAddress()
    std::cout << "Socket addresses used: \n";
    for (unsigned int i = 0; i < sockets.Size(); i++)
    {
        std::cout << i + 1 << ". " << sockets[i]->GetBoundAddress().ToString(true) << std::endl;
        //cout<<"iPV4: "<<server->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS,0).ToString(true)<<endl;
        //cout<<"iPV42: "<<server->GetInternalID(sockets[i]->GetBoundAddress(),0).ToString(true)<<endl;
        //cout<<"iPV62: "<<server->GetExternalID(sockets[i]->GetBoundAddress()).ToString(true)<<endl;
        numSockets++;
    }
    
    //se muestran las diferentes IPs del servidor mediante GetNumberOfAddresses() y GetLocalIP()
    std::cout << "My IP addresses: \n";
    
    for (unsigned int i = 0; i < server->GetNumberOfAddresses(); i++)
    {
        std::cout << i + 1 << ". " << server->GetLocalIP(i);
        
        numIPs++;
    }
    
    //se obtiene y muestra por pantalla el GUID del servidor
    std::cout << " My GUID is " << server->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString() << std::endl;
}


//==================================================================================================================
// funcion que recoge los paquetes recibidos por el servidor y realiza unas acciones u otras segun su identificador
//==================================================================================================================
void Server::ReceivePackets()
{

	GestorJugadores *jugadores = GestorJugadores::getInstancia();
    //DebugServerInfo();

    
	//bucle donde se reciben los distintos paquetes, se tratan y se deasignan
	for (p = server->Receive(); p; server->DeallocatePacket(p), p = server->Receive())
	{
		//se coge el identificador del paquete recibido para los casos del switch
		packetIdentifier = GetPacketIdentifier(p);
		players = jugadores->getJugadores();
		//se inicializan el tipo de RakNet para los mensajes de entrada y salida
		RakNet::BitStream bsIn(p->data, p->length, false); //|mensaje de entrada
		RakNet::BitStream bsOut;						   //|mensaje de salida	

		//vector de posicion de Irrlicht
		float posicion[3];
		float rotacion[3];
		btVector3 pos2[6];
		pos2[0].setX(-10);
		pos2[0].setY(0);
		pos2[0].setZ(310);
		pos2[1].setX(-10);
		pos2[1].setY(0);
		pos2[1].setZ(290);
		pos2[2].setX(-20);
		pos2[2].setY(0);
		pos2[2].setZ(310);
		pos2[3].setX(-20);
		pos2[3].setY(0);
		pos2[3].setZ(290);
		pos2[4].setX(-30);
		pos2[4].setY(0);
		pos2[4].setZ(310);
		pos2[5].setX(-30);
		pos2[5].setY(0);
		pos2[5].setZ(290);
		int id;
		int param;
		int param2;
		float paramFloat;
		float diff = 0.0;
		bool reset = false;
		bool lanzar = false;
		bool ulti = false;
		bool parambool = false;
		structPrediccion prediccionAux;

		RakNet::RakString paramRakString;
		std::string paramString;
			
		structClients client;	
		
		CorredorRed *jugador;
		
		unsigned short numConnections;
		RakNet::SystemAddress systems[10];


		//switch para comprobar el tipo de paquete recibido
		switch (packetIdentifier)
		{

		//uno de los clientes se ha desconectado del servidor
		case ID_DISCONNECTION_NOTIFICATION:
			paramString = p->systemAddress.ToString(true);
			std::cout << "ID_DISCONNECTION_NOTIFICATION de " << paramString << std::endl;
			std::cout<< "NumPlayers: " << numPlayers << std::endl;
			playerDisconnection(paramString);
			break;

		//el cliente ya esta conectado (en caso de realizar un connect)
            case ID_ALREADY_CONNECTED:
                std::cout << "ID_ALREADY_CONNECTED\n";
                bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
                server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
                param=numConnections;
                if (!started){
                    std::cout << "Numero de conexiones actuales: " << param << std::endl;
                    typeID = ID_LOAD_CURRENT_CLIENTS;
                    bsOut.Write(typeID);
                    bsOut.Write(param);
                    for(int i= 0; i< clientes.size(); i++){
                        paramRakString = clientes.at(i).ip.c_str();
                        bsOut.Write(paramRakString);
                        bsOut.Write(clientes.at(i).tipoCorredor);
                        bsOut.Write(clientes.at(i).ready);
                    } 
                    server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
                }else{
                    playerDisconnection(p->systemAddress.ToString(true));                    
                }
                break;
            case ID_CHECK_ALREADY_CONNECTED:
                std::cout << "ID_ALREADY_CONNECTED\n";
                bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
                server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
                param=numConnections;
                if (!started){
                    std::cout << "Numero de conexiones actuales: " << param << std::endl;
                    typeID = ID_LOAD_CURRENT_CLIENTS;
                    bsOut.Write(typeID);
                    bsOut.Write(param);
                    for(int i= 0; i< clientes.size(); i++){
                        paramRakString = clientes.at(i).ip.c_str();
                        bsOut.Write(paramRakString);
                        bsOut.Write(clientes.at(i).tipoCorredor);
                        bsOut.Write(clientes.at(i).ready);
                    } 
                    server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
                }else{
                    playerDisconnection(p->systemAddress.ToString(true));                    
                }
                break;    
		//un nuevo cliente se ha conectado al servidor
            case ID_NEW_INCOMING_CONNECTION:
                std::cout << "ID_NEW_INCOMING_CONNECTION\n";
                server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
                param=numConnections;
                if (!started || param <= 4){
                    client.ip = p->systemAddress.ToString(true);
                    client.tipoCorredor = 3;
                    client.ready = false;
					client.load = false;
                    clientes.push_back(client);
                    numConnections=10;
                    server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
                    param=numConnections;
                    std::cout << "Numero de conexiones actuales: " << param << std::endl;
                    typeID = ID_LOAD_CURRENT_CLIENTS;
                    bsOut.Write(typeID);
                    bsOut.Write(param);
                    for(int i = 0; i<clientes.size(); i++){
                        paramRakString = clientes.at(i).ip.c_str();
                        bsOut.Write(paramRakString);
                        bsOut.Write(clientes.at(i).tipoCorredor);
                        bsOut.Write(clientes.at(i).ready);
                    }
                    server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
                }else{
                    playerDisconnection(p->systemAddress.ToString(true));               
                }
                break;
                
		//cliente y servidor no comparten el mismo tipo de protocolo (IPv4-IPv6)
            case ID_INCOMPATIBLE_PROTOCOL_VERSION:
                std::cout << "ID_INCOMPATIBLE_PROTOCOL_VERSION\n";
                break;
                
            case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                std::cout << "ID_REMOTE_DISCONNECTION_NOTIFICATION\n";
                break;
            case ID_REMOTE_NEW_INCOMING_CONNECTION:
                std::cout << "ID_REMOTE_NEW_INCOMING_CONNECTION\n";
                break;
                
		//se le notifica al cliente que ha sido expulsado del servidor por x motivos
            case ID_CONNECTION_BANNED:
                std::cout << "Has sido baneado del server!\n";
                break;
                
		//la conexion del cliente con el servidor ha fallado
            case ID_CONNECTION_ATTEMPT_FAILED:
                std::cout << "La conexion no ha tenido exito!\n";
                break;
                
		//se ha perdido la conexion con uno de los clientes
            case ID_CONNECTION_LOST:
                paramString = p->systemAddress.ToString(true);
                std::cout << "ID_CONNECTION_LOST de " << p->systemAddress.ToString(true) << std::endl;
                
                playerDisconnection(paramString);
                
                break;
                
		//se le notifica al cliente que el servidor ya ha alcanzado su numero maximo de usuarios y, por lo tanto, no puede acceder
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                std::cout << "Lo sentimos, el servidor ya esta lleno\n";
                break;
                
		//el cliente ha proporcionado una contrasenya de acceso incorrecta
            case ID_INVALID_PASSWORD:
                std::cout << "Acceso denegado! Introduzca la clave correcta\n";
                break;
                
		//se ha aceptado la conexion con un cliente
		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "Tu conexion ha sido aceptada a " << p->systemAddress.ToString(true) << " con GUID " << p->guid.ToString() << std::endl;
			break;
		case ID_CHANGE_CHARACTER:
			std::cout<<"ID_CHANGE_CHARACTER_SERVER\n";
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(id);
			bsIn.Read(parambool);					//True = ++; False == --
			if (id<clientes.size() && id!=-1){
				param=clientes.at(id).tipoCorredor;	//a partir de ahora param es el tipo de jugador
				if (param==0 && parambool==false){
					param=3;
				}else if (param==3 && parambool==true){
					param=0;
				}else if (parambool){
					param++;
				}else{
					param--;
				}
				clientes.at(id).tipoCorredor=param;
			}
			std::cout << "Ahora el corredor " << id << " tiene el corredor " << param << endl;
			typeID = ID_CHANGE_CHARACTER;
			bsOut.Read(typeID);
			bsOut.Read(id);
			bsOut.Read(param);
			server->Send(&bsIn, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			parambool=false;
			break;
		case ID_RACE_START:
			std::cout<<"ID_RACE_START\n";
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(id);
			if (id!=0){		//si no eres host puedes alternar entre listo o no listo
				if (clientes.at(id).ready){
					clientes.at(id).ready=false;
				}else{
					clientes.at(id).ready=true;
				}
				typeID = ID_READY_CLIENT;
				bsOut.Write(typeID);		//enviamos la informacion de ready para actualizar en el cliente
				bsOut.Write(id);
				server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}else if(id==0){		//si eres host
				for (int n=1;n<clientes.size();n++){		//comprobamos si todos los corredores menos el host estan listos
					if (!clientes.at(n).ready){
						parambool=true;
						break;
					}
				}
				if (clientes.size()>1 && !clientes.at(id).ready){ //Si no estas solo y no estas listo
					clientes.at(id).ready=true;
					typeID = ID_READY_CLIENT;
					bsOut.Write(typeID);		//enviamos la informacion de ready para actualizar en el cliente
					bsOut.Write(id);
					server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
					break;
				}else if (clientes.size()>1 && clientes.at(id).ready && parambool==true){ //Si no estas solo y estas listo, pero algun cliente no
					clientes.at(id).ready = false;
					typeID = ID_READY_CLIENT;
					bsOut.Write(typeID);		//enviamos la informacion de ready para actualizar en el cliente
					bsOut.Write(id);
					server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
					break;
				}
				if (parambool==false && clientes.size()>0){		//si todos estan listos y ha recorrido el bucle empieza la carrera
				
					std::cout << "Cambiamos de escena\n";
					started=true;}
			}
			parambool=false;
			break;	
		case ID_RETURN_LOBBY:
		std::cout << "ID_RETURN_LOBBY\n";
                deleteEntities();
			started=false;
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			typeID = ID_RETURN_LOBBY;
			bsOut.Write(typeID);
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			break;	
		case ID_SEND_KEY_PRESS:
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(id);
			bsIn.Read(reset);
			bsIn.Read(lanzar);
			bsIn.Read(ulti);
			
			if(reset){
				players.at(id)->recolocarWaypoint();
			}
			if(lanzar){
				players.at(id)->usarObjetos();
			}
			if(ulti){
				std::cout << "Ulti lanzada\n";
				players.at(id)->lanzarHabilidad();
			}
			server->Send(&bsIn, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			break;

		case ID_SPAWN_PLAYER:
			std::cout << "SPAWN DE UN JUGADOR\n";
			/*bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(posicion[0]);
			bsIn.Read(posicion[1]);
			bsIn.Read(posicion[2]);
			bsIn.Read(playerNetworkID);
			std::cout <<posicion[0] <<", "<<posicion[1] << ", " <<posicion[2] << std::endl;
			players[numPlayers] = new CorredorRed("assets/coche.obj", pos2[0], Corredor::tipo_jugador::CHINO);
			//player[numPlayers]->setPosition(posicion);
			player[numPlayers]->SetNetworkIDManager(&networkIDManager);
			player[numPlayers]->SetNetworkID(playerNetworkID);

			typeID = ID_SPAWN_PLAYER;
			bsOut.Write(typeID);
			bsOut.Write(posicion[0]);
			bsOut.Write(posicion[1]);
			bsOut.Write(posicion[2]);
			bsOut.Write(numPlayers);
			bsOut.Write(playerNetworkID);
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
			numPlayers++;

			std::cout << "Jugador creado, en total: " << numPlayers << std::endl;
*/
			break;

		case ID_CLIENT_LOAD:
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(id);			//CONTROLPLAYER
			std::cout << "Cliente cargado: " << id << std::endl;
			parambool = false;
			clientes.at(id).load = true;
			for(int i = 0; i < clientes.size(); i++){
				if(!clientes.at(i).load)
					parambool = true;
			}
			if(!parambool){
				std::cout << "Todos los clientes cargados\n";
				allPlayerLoaded = true;
				typeID = ID_CLIENTS_LOADED;
				bsOut.Write(typeID);
				server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}
			break;

		case ID_PLAYER_MOVE:
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			//std::cout << "Entro a playerMove\n";
			bsIn.Read(param2);		//TIMESTAMP
			bsIn.Read(id);			//CONTROLPLAYER
			bsIn.Read(prediccionAux.posicion[0]); //POSICION ACTUAL
			bsIn.Read(prediccionAux.posicion[1]);	//
			bsIn.Read(prediccionAux.posicion[2]);	//
			bsIn.Read(prediccionAux.rotacion[0]);	//ROTACION
			bsIn.Read(prediccionAux.rotacion[1]);	//
			bsIn.Read(prediccionAux.rotacion[2]);	//
			prediccionAux.timeStamp = param2;
			clientes.at(id).prediccion.push_back(prediccionAux);
			if(id < players.size()){
				bsIn.Read(param);		//ESTADOS
				players.at(id)->getEstados()->setEstadoMovimiento(param);
				bsIn.Read(param);		//
				players.at(id)->getEstados()->setDireccionMovimiento(param);
				bsIn.Read(param);		//
				players.at(id)->getEstados()->setEstadoObjeto(param);
				bsIn.Read(param);		//
				players.at(id)->getEstados()->setEstadoCoche(param);
				bsIn.Read(param);		//
				players.at(id)->getEstados()->setEstadoCarrera(param);
				bsIn.Read(param);		//
				players.at(id)->getEstados()->setEstadoInmunidad(param);
				bsIn.Read(param);		//
				players.at(id)->getEstados()->setEstadoHabilidad(param);
				bsIn.Read(param);
				players.at(id)->setLimite(param);
				
				parambool = false;
				//std::cout << "Posicion Y: " << players.at(id)->getNodo()->getRotation().Y << " - " << clientes.at(id).prediccion.rotacion[1] << std::endl;
				//std::cout << "Posicion Z: " << players.at(id)->getNodo()->getRotation().Z << " - " << clientes.at(id).prediccion.rotacion[2] << std::endl;
				for(int i = 0; i < clientes.at(id).prediccion.size(); i++){
					if(timeStamp == clientes.at(id).prediccion.at(i).timeStamp){
						paramFloat = players.at(id)->getRigidBody()->getCenterOfMassPosition().getX() - clientes.at(id).prediccion.at(i).posicion[0];
						if(paramFloat > diff || paramFloat < -diff){
							parambool = true;
						}
						//std::cout << "Posicion X: " << paramFloat << "\n";
						paramFloat = players.at(id)->getRigidBody()->getCenterOfMassPosition().getY() - clientes.at(id).prediccion.at(i).posicion[1];
						if(paramFloat > diff || paramFloat < -diff){
							parambool = true;
						}
						//std::cout << "Posicion Y: " << paramFloat << "\n";
						paramFloat = players.at(id)->getRigidBody()->getCenterOfMassPosition().getZ() - clientes.at(id).prediccion.at(i).posicion[2];
						if(paramFloat > diff || paramFloat < -diff){
							parambool = true;
						}
						//std::cout << "Posicion Z: " << paramFloat << "\n";
						paramFloat = players.at(id)->getNodo()->getRotation().x - clientes.at(id).prediccion.at(i).rotacion[0];
						if(paramFloat > diff || paramFloat < -diff){
							//std::cout << "Rotacion X\n";
							parambool = true;
						}
						paramFloat = players.at(id)->getNodo()->getRotation().y - clientes.at(id).prediccion.at(i).rotacion[1];
						if(paramFloat > diff || paramFloat < -diff){
							//std::cout << "Rotacion Y\n";
							parambool = true;
						}
						paramFloat = players.at(id)->getNodo()->getRotation().z - clientes.at(id).prediccion.at(i).rotacion[2];
						if(paramFloat > diff || paramFloat < -diff){
							//std::cout << "Rotacion Z\n";
							parambool = true;
						}
					}
				}
				if(parambool){
					//std::cout << "Envio correccion\n";
					/*
					prediccionAux.posicion[0] = players.at(id)->getRigidBody()->getCenterOfMassPosition().getX();
					prediccionAux.posicion[1] = players.at(id)->getRigidBody()->getCenterOfMassPosition().getY();
					prediccionAux.posicion[2] = players.at(id)->getRigidBody()->getCenterOfMassPosition().getZ();
					prediccionAux.rotacion[0] = players.at(id)->getNodo()->getRotation().X;
					prediccionAux.rotacion[1] = players.at(id)->getNodo()->getRotation().Y;
					prediccionAux.rotacion[2] = players.at(id)->getNodo()->getRotation().Z;
					*/
					typeID = ID_PLAYER_REFRESH;
					bsOut.Write(typeID);
					bsOut.Write(players.at(id)->getRigidBody()->getCenterOfMassPosition().getX());
					bsOut.Write(players.at(id)->getRigidBody()->getCenterOfMassPosition().getY());
					bsOut.Write(players.at(id)->getRigidBody()->getCenterOfMassPosition().getZ());
					bsOut.Write(players.at(id)->getNodo()->getRotation().x);
					bsOut.Write(players.at(id)->getNodo()->getRotation().y);
					bsOut.Write(players.at(id)->getNodo()->getRotation().z);
					server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
				}
				
			}
			//server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);

			break;

		case ID_PLAYER_STATE:
			int estado1, estado2, estado3, estado4;
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(estado1);
			bsIn.Read(estado2);
			bsIn.Read(estado3);
			bsIn.Read(estado4);
			bsIn.Read(id);
			if(id == numPlayers) id--;  //Este caso solo se da si se ha eliminado un jugador en este conjunto de paquetes;
			//player[id]->setEstados(estado1, estado2, estado3, estado4);

			typeID = ID_PLAYER_STATE;
			bsOut.Write(typeID);
			bsOut.Write(estado1);
			bsOut.Write(estado2);
			bsOut.Write(estado3);
			bsOut.Write(estado4);
			bsOut.Write(id);
			
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
			break;

		case ID_PLAYER_REFRESH:
/*
			int t;

			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(t);
			bsIn.Read(id);
			if(id == numPlayers) id--;  //Este caso solo se da si se ha eliminado un jugador en este conjunto de paquetes;
			std::cout << "He cogido un objeto: " << id << std::endl;

			//player[id]->setObj(t);

			typeID = ID_PLAYER_SET_OBJECT;
			bsOut.Write(typeID);
			bsOut.Write(t);
			bsOut.Write(id);

			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
			*/
			break;	

		case ID_PLAYER_THROW_OBJECT:
/*
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(id);
			if(id == numPlayers) id--; //Este caso solo se da si se ha eliminado un jugador en este conjunto de paquetes;

			//player[id]->setObj(0);

			typeID = ID_PLAYER_THROW_OBJECT;
			bsOut.Write(typeID);
			bsOut.Write(id);

			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
			*/
			break;
	
		case ID_PLAYER_DISCONNECT:
			std::cout << "Borrando player\n";
			int playerDisconnect;
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(playerDisconnect);
			std::cout << "Jugador eliminado: " << playerDisconnect << " / " <<numPlayers << std::endl;
			clientes.erase(clientes.begin()+playerDisconnect);
			//playerDisconnection(playerDisconnect);
			if(started){
				//Borrar players tambien

			}
			typeID = ID_LOAD_CURRENT_CLIENTS;
			numConnections=10;
			server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
			param=numConnections;
			std::cout << "Numero de conexiones actuales: " << param << std::endl;
			typeID = ID_LOAD_CURRENT_CLIENTS;
			bsOut.Write(typeID);
			bsOut.Write(param);
			for(int i = 0; i<clientes.size(); i++){
				paramRakString = clientes.at(i).ip.c_str();
				bsOut.Write(paramRakString);
				bsOut.Write(clientes.at(i).tipoCorredor);
				bsOut.Write(clientes.at(i).ready);
			}
			server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			
			std::cout << "Jugador Borrado\n";

			break;
		
            default:
                
                break;
        }
        jugadores->setJugadores(players);
    }
}

void Server::RaceStartSend(){
	RakNet::BitStream bsOut;
	typeID = ID_RACE_START;
	bsOut.Write(typeID);
	server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Server::RaceStart(){
	typeID = ID_RACE_START;
	btVector3 pos2[6];
	pos2[0].setX(Pista::getInstancia()->getParrilla().at(0).x);
	pos2[0].setY(Pista::getInstancia()->getParrilla().at(0).y);
	pos2[0].setZ(Pista::getInstancia()->getParrilla().at(0).z);
	pos2[1].setX(Pista::getInstancia()->getParrilla().at(1).x);
	pos2[1].setY(Pista::getInstancia()->getParrilla().at(1).y);
	pos2[1].setZ(Pista::getInstancia()->getParrilla().at(1).z);
	pos2[2].setX(Pista::getInstancia()->getParrilla().at(2).x);
	pos2[2].setY(Pista::getInstancia()->getParrilla().at(2).y);
	pos2[2].setZ(Pista::getInstancia()->getParrilla().at(2).z);
	pos2[3].setX(Pista::getInstancia()->getParrilla().at(3).x);
	pos2[3].setY(Pista::getInstancia()->getParrilla().at(3).y);
	pos2[3].setZ(Pista::getInstancia()->getParrilla().at(3).z);
	pos2[4].setX(Pista::getInstancia()->getParrilla().at(4).x);
	pos2[4].setY(Pista::getInstancia()->getParrilla().at(4).y);
	pos2[4].setZ(Pista::getInstancia()->getParrilla().at(4).z);
	pos2[5].setX(Pista::getInstancia()->getParrilla().at(5).x);
	pos2[5].setY(Pista::getInstancia()->getParrilla().at(5).y);
	pos2[5].setZ(Pista::getInstancia()->getParrilla().at(5).z);
	structPrediccion prediccionAux;
    GestorJugadores *jugadores = GestorJugadores::getInstancia();
	CorredorRed *jugador;
	unsigned short numConnections=10;
    RakNet::SystemAddress systems[10];
	server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
	int id=numConnections;
	Corredor::tipo_jugador tj;
	for(int i= 0; i<id; i++){		
		if (clientes.at(i).tipoCorredor == 0){
			tj=Corredor::tipo_jugador::GLADIADOR;
		}else if (clientes.at(i).tipoCorredor == 1){
			tj=Corredor::tipo_jugador::PIRATA;
		}else if (clientes.at(i).tipoCorredor == 2){
			tj=Corredor::tipo_jugador::VIKINGO;
		}else if (clientes.at(i).tipoCorredor == 3){
			tj=Corredor::tipo_jugador::CHINO;
		}
		std::cout << "Creo jugador \n";
		jugador = new CorredorRed(pos2[i], tj);
		jugador->setID(i);
		players.push_back(jugador);
		prediccionAux.timeStamp = 0;
		prediccionAux.posicion[0] = pos2[i].getX();
		prediccionAux.posicion[1] = pos2[i].getY();
		prediccionAux.posicion[2] = pos2[i].getZ();
		prediccionAux.rotacion[0] = pos2[i].getX();
		prediccionAux.rotacion[1] = pos2[i].getY();
		prediccionAux.rotacion[2] = pos2[i].getZ();
		clientes.at(i).prediccion.push_back(prediccionAux);
		jugadores->aumentarJugadores();
		std::cout << "Meto jugador\n";
	}
	jugadores->setJugadores(players);
}

//====================================================================================================
// Metodo encargado de enviar todos los paquetes almacenados
//====================================================================================================

void Server::Update(){
	for(int i=0; i<paquetes.size(); i++){
		std::cout << "Envio el paquete " << i << " \n";
		RakNet::BitStream *bitstreamStruct = paquetes.at(i).bitstreamStruct;
		PacketPriority priority = paquetes.at(i).priority;
		PacketReliability reliability= paquetes.at(i).reliability;
		char desconocido = paquetes.at(i).desconocido;
		RakNet::AddressOrGUID receptor = paquetes.at(i).receptor;
		bool envio = paquetes.at(i).envio;
		std::cout << "Variables almacenadas\n";
		//server->Send(bitstreamStruct, priority, reliability, desconocido, receptor, envio);
		std::cout << "Enviado\n";
	}
}

//====================================================================================================
// Se recoge el identificador del paquete que se le pasa por parametro (se llama desde ReceivePackets)
//====================================================================================================
unsigned char Server::GetPacketIdentifier(RakNet::Packet *p)
{
    if (p == 0)
        return 255;
    
    if ((unsigned char)p->data[0] == ID_TIMESTAMP)
    {
        RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
        return (unsigned char)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
    }
    else
        return (unsigned char)p->data[0];
}

//==================================================
// Se cierra el servidor y se destruye su instancia
//==================================================
void Server::ShutDownServer()
{
    //se muestra por pantalla que el servidor va a cerrarse
    std::cout << "Cerrando server\n";
    
    //se cierra el servidor especificando el numero de milisegundos a esperar para que los mensajes terminen de salir del servidor
    //si el tiempo == 0, entonces no se envia la notifiacion de desconexion
    server->Shutdown(300);
    
    //se destruye la instancia del servidor de la misma forma que se recogio en su momento
    RakNet::RakPeerInterface::DestroyInstance(server);
}

//=======================================================================================================================
// Metodo que almacena los paquetes en el vector de paquetes para ser enviados
//=======================================================================================================================

void Server::AddSend(RakNet::BitStream *bitstreamStruct, PacketPriority priority, PacketReliability reliability, int desconocido, RakNet::AddressOrGUID receptor, bool envio){
	structPaquetes paqueteAux;
	std::cout << "Anado un paquete al envio\n";
	paqueteAux.bitstreamStruct = bitstreamStruct;
	paqueteAux.priority = priority;
	paqueteAux.reliability = reliability;
	paqueteAux.desconocido = desconocido;
	paqueteAux.receptor = receptor;
	paqueteAux.envio = envio;

	paquetes.push_back(paqueteAux);
}
        
//=======================================================================================================================
// se refresca el servidor cada cierto periodo de tiempo pasandole la posicions de todos los objetos al resto de clientes
//=======================================================================================================================
void Server::refreshServer()
{
    float *pos = new float[3];
    float *ori = new float[3];
    int tipoObj;
    
    GestorJugadores *jugadores = GestorJugadores::getInstancia();
    players = jugadores->getJugadores();
	EstadosJugador *estados;
		
    typeID = ID_REFRESH_SERVER;
    RakNet::BitStream bsOut;
    
    bsOut.Write(typeID);
    
    for(int i = 0; i<players.size(); i++){
        bsOut.Write(i);
		estados = players.at(i)->getEstados();
		
		bsOut.Write(players.at(i)->getRigidBody()->getCenterOfMassPosition().getX());
		bsOut.Write(players.at(i)->getRigidBody()->getCenterOfMassPosition().getY());
		bsOut.Write(players.at(i)->getRigidBody()->getCenterOfMassPosition().getZ());
		bsOut.Write(players.at(i)->getRigidBody()->getCenterOfMassTransform().getRotation());
		/*
		bsOut.Write(players.at(i)->getNodo()->getRotation().x);
		bsOut.Write(players.at(i)->getNodo()->getRotation().y);
		bsOut.Write(players.at(i)->getNodo()->getRotation().z);
		*/
		bsOut.Write(estados->getEstadoMovimiento());
		bsOut.Write(estados->getDireccionMovimiento());
		bsOut.Write(estados->getEstadoObjeto());
		bsOut.Write(estados->getEstadoCoche());
		bsOut.Write(estados->getEstadoCarrera());
		bsOut.Write(estados->getEstadoInmunidad());
		bsOut.Write(estados->getEstadoHabilidad());
		bsOut.Write(players.at(i)->getLimite());

    }
    //std::cout << "Control: " << controlPlayer << std::endl;
    
    //se envia el mensaje de salida
    server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

int Server::getCommands(){
    std::string mensaje;
    std::string mensajeIP;
    std::string mensajePort;
    char msj[2048];
    RakNet::RakNetStatistics *rss;
    int index;
    int objeto;
    if(kbhit()){
        std::cin >> mensaje;
        if(mensaje.compare("shut")==0){
            ShutDownServer();
            return 1;
        }else if(mensaje.compare("getConnectionList")==0){
            GetConnectionList();
        }else if(mensaje.compare("stats")==0){
            /*
             Si descomentas esto y el rss del header pasa esto: 
             
             exec: malloc.c:2427: sysmalloc: Assertion `(old_top == initial_top (av) && old_size == 0) || ((unsigned long) (old_size) >= MINSIZE && prev_inuse (old_top) && ((unsigned long) old_end & (pagesize - 1)) == 0)' failed.
             Aborted (core dumped)
             
             */
            rss=server->GetStatistics(server->GetSystemAddressFromIndex(0));
            StatisticsToString(rss, msj, 2);
            printf("%s", msj);
            printf("Ping %i\n", server->GetAveragePing(server->GetSystemAddressFromIndex(0)));
        }else if(mensaje.compare("kick")==0){
            GetConnectionList();
            int index;
            std::cout << "Introduzca el indice para kick: \n";
            std::cin>>index;
            playerDisconnection(clientes.at(index).ip);
            server->CloseConnection(server->GetSystemAddressFromIndex(index), true, 0);
        }else if(mensaje.compare("obj") == 0) {
            if(started){
                std::cout << "Introduzca numero de jugador: \n";
                GetConnectionList();
                std::cin >> index;
                std::cout << "Introduzca numero de objeto: \n";
                std::cout << "1. Flecha\n";
                std::cout << "2. Caja Falsa\n";
                std::cout << "3. Turbo\n";
                std::cout << "4. Aceite\n";
                std::cout << "5. Escudo\n";
                std::cout << "6. Flecha Triple\n";
                std::cout << "7. Teledirigido\n";
                std::cout << "8. Turbo Triple\n";
                std::cin >> objeto;
                GestorJugadores *jugadores = GestorJugadores::getInstancia();
                players = jugadores->getJugadores();
                if(objeto < 9 && objeto > -1 && index < jugadores->getNumJugadores() && index > -1){
                    players[index]->setTipoObj((EstadosJugador::estado_objeto)objeto);
                }else std::cout << "Parametros incorrectos\n";
            }
        }
    }
    return 0;
}

void Server::GetConnectionList(){
    RakNet::SystemAddress systems[10];
    unsigned short numConnections=10;
    server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
    std::cout << "Connection list: \n";
    for (int i=0; i < numConnections; i++)
    {
        std::cout << i << ". " << systems[i].ToString(true) << std::endl;
    }
}

void Server::playerDisconnection(std::string str_param){
    int param;
    RakNet::BitStream bsOut;
    GestorJugadores *jugadores = GestorJugadores::getInstancia();
    players = jugadores->getJugadores();
    for(int i= 0; i< clientes.size(); i++){
        if(str_param.compare(clientes.at(i).ip) == 0){
            param = i;
            break;
        }
    }
    std::cout << "Jugador eliminado: " << param << " / " <<clientes.size() << std::endl;
    //param es la id del jugador desconectado
    clientes.erase(clientes.begin()+param);
    
    if(started){
        //Borrar players tambien
        players.erase(players.begin()+param);
        jugadores->decrementarJugadores();
        //jugadores->getJugadores().at(param)->~Corredor(); 
    }
    
    typeID = ID_PLAYER_DISCONNECT;
    bsOut.Write(typeID);
    bsOut.Write(param);
   server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
    
    jugadores->setJugadores(players);
    
    /*GestorJugadores *jugadores = GestorJugadores::getInstancia();
     players = jugadores->getJugadores();
     RakNet::BitStream bsOut;	
     players.at(playerDisconnect)->~Corredor();					   //|mensaje de salida	
     players.erase(players.begin()+playerDisconnect);
     std::cout<< "NumPlayers: " << numPlayers << std::endl;
     
     numPlayers--;
     
     typeID = ID_PLAYER_DISCONNECT;
     bsOut.Write(typeID);
     bsOut.Write(playerDisconnect);
     server->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
     */
}
void Server::deleteEntities(){
    if (players.size()>0){
        GestorJugadores *jugadores = GestorJugadores::getInstancia();
        for (int i=0;i<jugadores->getJugadores().size();i++){
        cout<<"borrando nodos corredor\n";
        jugadores->getJugadores().at(i)->~Corredor(); 
        }
       cout<<"GUAY\n";
	cout << "Voy a entrar en el destructor de bullet. Deseadme suerte...\n";
	delete MotorFisicas::getInstancia();
	cout << "Bien!\n";
        
	cout << "Voy a entrar en el destructor de jugadores. Deseadme suerte...\n";
	delete GestorJugadores::getInstancia();
	cout << "Bien!\n";
        
	cout << "Voy a entrar en el destructor de pista. Deseadme suerte...\n";
	delete Pista::getInstancia();
	cout << "No ha ido mal.\n";
	/*for (int i=0;i<players.size();i++){
         players.at(i)->setVueltas(1);
         }*/
    for (int i=0;i<clientes.size();i++){
        clientes.at(i).ready=false;
    }
    jugadores = GestorJugadores::getInstancia();
	cout<<"borrando corredoresdores\n";
    players.clear();
	players = jugadores->getJugadores();
    cout<<"GUAY\n";
	cout << "Set jugadores. Tamanyo: "<<players.size() <<"\n";
	//jugadores->setJugadores(players);
    }
}
void Server::setStarted(bool b){
    started = b;
}

bool Server::getStarted(){
    return started;
}

void Server::aumentarTimestamp(){
	timeStamp++;
}

std::string Server::getStringClients(){
	std::string ret = "\n Clientes: \n";
	for(int i = 0; i < clientes.size(); i++){
		ret.append(to_string(i) + ": " + clientes.at(i).ip + "\n" + " - Tipo: ");
		if (clientes.at(i).tipoCorredor == 0)
			ret.append("GLADIADOR ");
		else if (clientes.at(i).tipoCorredor == 1)
			ret.append("PIRATA ");
		else if (clientes.at(i).tipoCorredor == 2)
			ret.append("VIKINGO ");
		else if (clientes.at(i).tipoCorredor == 3)
			ret.append("GUERRERO CHINO ");
		if(clientes.at(i).ready)
			ret.append("\n - LISTO\n");
		else
			ret.append("\n - NO LISTO\n");
	}
	return ret;
}


bool Server::getPlayersLoaded(){
	return allPlayerLoaded;
}
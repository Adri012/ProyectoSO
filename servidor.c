 #include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>
#include <pthread.h>

//Acceso excluyente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Estructuras
typedef struct {
    char nombre[20];
    int socket;
} Conectado;

typedef struct {
    Conectado conectados [100];
    int num;
} ListaConectados;

typedef struct {
	int IDPartida;
	int numJugadores;
	int turno;
	char tablero[30];
	
	char Host[20];
	char nom2[20];
	char nom3[20];
	char nom4[20];
	
	int sock[4];
	int ocupado;		//bool
}Partida;

typedef struct {
	Partida partidas [100];
	int num;
} ListaPartidas;


//Variables SQL
MYSQL * conn;
int err;
MYSQL_RES * resultado;
MYSQL_ROW row;

//---------------------------------------------------------------------------------------

//Variables generales
int contador;
int i;
int sockets [100];

ListaConectados miLista;
ListaPartidas miListaPartidas;


//______________________________________________________________________________
//FUNCIONES DE BUSQUEDA:

int BuscarPosicionLista (ListaConectados *lista, char nombre[20])
{
	printf("Inicio SearchPosition.......................\n");
	int i =0;
	int encontrado=0;	
	while ((i<lista->num) && (encontrado==0))
	{
		if (strcmp(lista->conectados[i].nombre,nombre)==0)
			encontrado=1;
		else
				i=i+1;
	}
	if (encontrado == 1)
		return i;
	else
		return -1;
	printf("Final SearchPosition......................\n");
}


int DameSocket (ListaConectados *lista, char nombre[20])
{
    int i = 0;
    int encontrado = 0;
    while((i < lista->num) && !encontrado)
    {
		if (strcmp(lista->conectados[i].nombre,nombre) == 0)
			encontrado = 1;
   		if(!encontrado)
			i = i+1;
   	}
    if (encontrado)
    	return i;
    else
    	return -1;
}


//______________________________________________________________________________
//GESTION CONECTADOS:

void DameConectados (ListaConectados *lista, char conectado[512])
{
    char conectados[512];
	sprintf(conectados, "%d", lista->num);
	printf("Este es el listanum %d \n", lista->num);
    int i;
    for(i=0; i<lista->num; i++)
    {
		sprintf(conectados, "%s/%s", conectados, lista->conectados[i].nombre);
    }
	sprintf(conectado, "6/%s \n", conectados);
	printf(conectado);
}

void NuevoConectado (ListaConectados *lista, char p[200], int socket)
{
    char nombre[20];
	strcpy(nombre,p);
	printf("Este es el antiguo listanum %d \n", lista->num);
	strcpy(lista->conectados[lista->num].nombre, nombre);
	lista->conectados[lista->num].socket = socket;
	lista->num = lista->num +1;
	printf("Este es el nuevo listanum %d \n", lista->num);
}

int Desconectar (ListaConectados *lista, char p[200], char respuesta[512])
{
    char nombre[20];
	strcpy(nombre,p);
	
	int pos = DameSocket (lista,nombre);
	printf("Posicion del socket: %d \n", pos);
	printf("este es el listanum viejo: %d \n", lista->num);
    if (pos == -1)
       	return -1;  
    else
    {
		int i;
		for (i = pos; i < lista->num-1; i++)
		{
			strcpy(lista->conectados[i].nombre, lista->conectados[i+1].nombre);
			lista->conectados[i].socket = lista->conectados[i+1].socket;
		}
		lista->num--;
		sprintf(respuesta,"0/Hasta la proxima!");
		return 0;
    }
}



//______________________________________________________________________________
//CONSULTAS/PETICIONES PANTALLA DE INICIO:

void PartidasGanadas (char p[200], char respuesta[512])
{
    char nombre[20];
	strcpy(nombre,p);
	char consulta[200];
	sprintf(consulta,"SELECT Jugador.partidas_ganadas FROM Jugador WHERE Jugador.username ='%s' ",nombre);
    printf("%s",consulta);
    err=mysql_query(conn, consulta);
    if (err!=0)
	{
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
    }
    resultado = mysql_store_result(conn);
    row = mysql_fetch_row(resultado);
    if (row == NULL)
	{
		printf("No se ha obtenido la consulta \n");
		sprintf(respuesta,"1/0");
    }
	else
    {
		printf("El usuario ha ganado %s partidas \n",row[0]);
		sprintf(respuesta,"1/%s",row[0]);
    }
}

void PartidasJugadas (char p[200], char respuesta[512])
{
    char nombre[20];
	strcpy(nombre,p);
	char consulta[200];
	sprintf(consulta,"SELECT Jugador.partidas_jugadas FROM Jugador WHERE Jugador.username ='%s' ",nombre);
    printf("%s",consulta);
    err=mysql_query(conn, consulta);
    if (err!=0)
	{
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
    }
    resultado = mysql_store_result(conn);
    row = mysql_fetch_row(resultado);
    if (row == NULL)
	{
		printf("No se ha obtenido la consulta \n");
		sprintf(respuesta,"2/0");		
    }
	else
    {
		int jugadas = atoi(row[0]);
		if (jugadas==0)
			printf("El usuario no ha jugado ninguna partida \n");
		else
			printf("El usuario ha jugado %d partidas \n",jugadas);
		sprintf(respuesta,"2/%s",row[0]);
	}
}

void DameGanador(char respuesta[512])
{
	char consulta[200];
	sprintf(consulta, "SELECT Jugador.username FROM Jugador WHERE Jugador.partidas_ganadas = (SELECT MAX(Jugador.partidas_ganadas) FROM Jugador)");
    printf("%s",consulta);
    err=mysql_query(conn, consulta);
    if (err!=0)
	{
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
    }
    resultado = mysql_store_result(conn);
    row = mysql_fetch_row(resultado);
    if (row == NULL)
	{
		printf("No se ha obtenido la consulta \n");
		sprintf(respuesta,"3/0");
	}
	else
		sprintf(respuesta,"3/%s",row[0]);
}



//______________________________________________________________________________
//FUNCIONES DEL JUEGO:

int Registro (char p[200],char respuesta[512])
{
    char nombre[20];
	char contrasena[20];
	char email[100];
	strcpy(nombre,p);
	p = strtok( NULL, "/");
    strcpy(contrasena,p);
	p = strtok( NULL, "/");
    strcpy(email,p);
	
	char consulta[512]; 
    sprintf(consulta, "SELECT Jugador.username FROM Jugador WHERE Jugador.username='%s' ",nombre);
    err=mysql_query (conn, consulta);
    if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
    }
    resultado = mysql_store_result(conn);
    row=mysql_fetch_row(resultado);
    if (row==NULL)
	{
		int	idmax;
		char consulta_id[512];
		strcpy(consulta_id,"SELECT MAX(Jugador.id) FROM (Jugador)");
		err=mysql_query (conn, consulta_id);
		if (err!=0) {
			printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		resultado = mysql_store_result (conn);
		row = mysql_fetch_row (resultado);
   	
		int idNuevo=atoi(row[0])+1;
		char insert[512];
		sprintf(insert,"INSERT INTO Jugador VALUES (%d,'%s','%s','%s',0,0)",idNuevo,nombre,contrasena,email);
   	 
		err=mysql_query (conn, insert);
		if (err!=0) {
			printf ("Error al insertar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
			sprintf(respuesta,"4/NOINSERTADO");   				 
   	 		exit (1);
		}
		else
			sprintf(respuesta,"4/CORRECTO");
    }
    else
		sprintf(respuesta,"4/YAEXISTE");		 
}

int InicioSesion(char p[200], char respuesta[512], int sock_conn)
{
    char nombre[20];
	char contrasena[20];
	strcpy (nombre, p);
    p = strtok( NULL, "/");
    strcpy(contrasena,p);
    
	char consulta[512];
    sprintf(consulta, "SELECT Jugador.username, Jugador.password FROM Jugador WHERE (Jugador.username='%s' AND Jugador.password='%s')",nombre,contrasena);
    err=mysql_query(conn, consulta);
    if (err!=0){
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
    }
    resultado = mysql_store_result(conn);
    row = mysql_fetch_row(resultado);
    if (row == NULL)
    {
		printf("No se ha obtenido la consulta \n");
		sprintf(respuesta,"5/INCORRECTO");
    }
    else
    {
		printf("Inicio de sesion completado \n");
		sprintf(respuesta,"5/CORRECTO"); 	 
	}
}


//______________________________________________________________________________
//GESTION DE PARTIDAS:
void CrearPartida(char nombre [20],ListaConectados *listaC, ListaPartidas *listaP, 	char respuesta[512], int sock_conn)
{
	int e=0;
	int i=0;
	int r=0;
	printf("Iniciando busqueda.......................................\n");
	printf("Numero de partidas: %d\n",listaP->num);
	if (listaP->num<100)
	{
		while ((e==0) && (i<100))
		{
			if ((listaP->partidas[i].ocupado == 0))
			{
				printf("Espacio libre encontrado.......................................\n");
				strcpy(listaP->partidas[i].Host, nombre);
				printf("Nombre del Host: %s\n",listaP->partidas[i].Host);
				int t = BuscarPosicionLista(listaC,listaP->partidas[i].Host);
				listaP->partidas[i].sock[0]=listaC->conectados[t].socket;
				listaP->partidas[i].IDPartida=i;
				listaP->partidas[i].numJugadores=1;
				listaP->partidas[i].ocupado=1;
				
				printf("Socket de la partida: %d\n",listaP->partidas[i].sock[0]);
				printf("ID partida: %d\n",listaP->partidas[i].IDPartida);
				printf("Numero de jugadores: %d\n",listaP->partidas[i].numJugadores);
				printf("Ocupado: %d\n",listaP->partidas[i].ocupado);

				listaP->num++;
				printf("ListaPartidas NUM: %d \n", listaP->num);
				printf("Host añadido.......................................\n");
				r=1;
				e=1;
			}
			i=i+1;
		}
	}
	else
		r=-1;
	printf("Final de la busqueda. R= %d \n", r);
	if (r==0)
	{
		sprintf(respuesta,"%s, error durante la creacion de la partida", nombre);
		printf("Error al crear la partida\n");
	}
	else if (r==-1)
	{
		sprintf(respuesta,"%s, todas las partidas estan ocupadas",nombre);
		printf("Error al crear la partida \n");
	}
	else if (r==1)
	{
		sprintf(respuesta, "%s, su partida ha sido creada con exito", nombre);
		printf("Partida creada.......................................\n");
	}
}

int FinalPartida (char nombre[20], ListaPartidas *lista)
{
	int e=0;
	int i=0;
	printf("Cerrando partida.......................................\n");
	while ((e==0) && (i < 100))
	{
		if ((strcmp(lista->partidas[i].Host,nombre)==0) && (lista->partidas[i].ocupado ==1))
		{
			lista->partidas[i].ocupado=0;
			lista->partidas[i].Host[0]='\0';
			lista->partidas[i].nom2[0]='\0';
			lista->partidas[i].nom3[0]='\0';
			lista->partidas[i].nom4[0]='\0';
			lista->num--;
			e=1;
		}
		i++;
	}
	if (e==1)
		printf("Partida cerrada con exito.......................................\n");
	else
		printf("Error al cerrar la Partida .....................................\n");
	return e;
}

int BuscarPartida(char nombre[20], ListaPartidas *listaP)
{
	int e=0;
	int i=0;
	printf("Iniciando BuscarPartida.......................................\n");
	while ((e==0) && (i < listaP->num))
	{
		printf("%s // %s\n",listaP->partidas[i].Host,nombre);
		if (((strcmp(listaP->partidas[i].Host,nombre)==0)))
			e=1;
		if (e==0)
			i++;
	}
	printf("Posicion: %d Encontrado: %d\n",i,e);
	if (e==1)
		return i;
	else
		return -1;
	printf("Cerrando BuscarPartida.......................................\n");
}

int UnirsePartida(char Host[20], char nombre[20], ListaConectados *listaC, ListaPartidas *listaP)
{
	printf("Iniciando UnirsePartida.......................................\n");
	int posicion = BuscarPartida(Host,listaP);
	if (posicion==-1)												//No se ha encontrado la partida
		return -2;
	else
	{
		if (listaP->partidas[posicion].numJugadores==0)				//No se ha podido unir
			return -1;
		else if (listaP->partidas[posicion].numJugadores==4)		//Partida llena
			return 0;
		else if (listaP->partidas[posicion].numJugadores==1)
		{
			strcpy(listaP->partidas[posicion].nom2,nombre);
			int socket = DameSocket(listaC, nombre);
			listaP->partidas[posicion].sock[listaP->partidas[posicion].numJugadores]=socket;
			listaP->partidas[posicion].numJugadores=2;
			return 1;
		}
		else if (listaP->partidas[posicion].numJugadores==2)
		{
			strcpy(listaP->partidas[posicion].nom3,nombre);
			int socket = DameSocket(listaC, nombre);
			listaP->partidas[posicion].sock[listaP->partidas[posicion].numJugadores]=socket;
			listaP->partidas[posicion].numJugadores=3;
			return 1;
		}
		else if (listaP->partidas[posicion].numJugadores==3)
		{
			strcpy(listaP->partidas[posicion].nom4,nombre);
			int socket = DameSocket(listaC, nombre);
			listaP->partidas[posicion].sock[listaP->partidas[posicion].numJugadores]=socket;
			listaP->partidas[posicion].numJugadores=3;
			return 1;
		}
		else
			return -1;
	}
	printf("Cerrando UnirsePartida.......................................\n");
}

int BuscarTuPartida(int socket, ListaPartidas *lista){
	int e=0;
	int i=0;
	while ((i<lista->num) && (e==0))
	{
		for (int j=0;j<lista->partidas[i].numJugadores;j++)
		{
			if (lista->partidas[i].sock[j]==socket)
				e=1;
		}
		if (e==0)
			i++;
	}
	if (e==1)
		return i;
	else
		return -1;
}



//______________________________________________________________________________
//INVITACIONES:

void Invitacion(char *p, ListaConectados *listaC, ListaPartidas *listaP, char respuesta[1024], char posD_str[4], char posD2_str[4], char posD3_str[4], char invitados_str[4])
{
	char nomO [20];
	char nomD [20];
	char nomD2 [20];
	char nomD3 [20];
	int invitados;
	
	printf("Preparando invitacion.......................................\n");
	invitados = atoi(p);
	printf("Invitados: %d \n", invitados);
	p = strtok(NULL, "/");
	strcpy(nomD,p);
	printf("Destino 1: %s \n",nomD);
	p = strtok(NULL, "/");
	if (invitados==2)
	{
		strcpy(nomD2,p);
		printf("Destino 2: %s \n",nomD2);
		p = strtok(NULL, "/");
	}
	else if (invitados==3)
	{
		strcpy(nomD2,p);
		printf("Destino 2: %s \n",nomD2);
		p = strtok(NULL, "/");
		strcpy(nomD3,p);
		printf("Destino 3: %s \n",nomD3);
		p = strtok(NULL, "/");
	}
	
	strcpy(nomO,p);
	printf("Origen: %s \n", nomO);
	
	int posD = BuscarPosicionLista(listaC,nomD);
	int socketD = DameSocket(listaC, nomD);
	//sprintf(socketD_str, "%d", socketD);
	sprintf(posD_str, "%d", posD);
	if (invitados==2)
	{
		int posD2 = BuscarPosicionLista(listaC,nomD2);
		int socketD2 = DameSocket(listaC, nomD2);
		//sprintf(socketD2_str, "%d", socketD2);
		sprintf(posD2_str, "%d", posD2);
	}
	else if (invitados==3)
	{
		int posD2 = BuscarPosicionLista(listaC,nomD2);
		int posD3 = BuscarPosicionLista(listaC,nomD3);
		int socketD2 = DameSocket(listaC, nomD2);
		int socketD3 = DameSocket(listaC, nomD3);
		//sprintf(socketD2_str, "%d", socketD2);
		//sprintf(socketD3_str, "%d", socketD3);
		sprintf(posD2_str, "%d", posD2);
		sprintf(posD3_str, "%d", posD3);
	}
	int posO = BuscarPosicionLista(listaC,nomO);
	int socketO = DameSocket(listaC, nomO);
	//printf("Pos Destino:%d \t Pos Origen:%d \n", posD, posO);
	//printf("Socket Destino: %d \t Socket Origen: %d \n",socketD, socketO);
	
	int r = BuscarPartida(nomO, listaP);
	printf("Result BuscarPartida: %d\n",r);
	if (r==-1)
	{
		CrearPartida(nomO,listaC,listaP,respuesta, socketO);
		printf("Partida lista.......................................\n");
	}
	
	sprintf(invitados_str, "%d", invitados);
	sprintf(respuesta,"8/%s/%s",nomO, invitados_str);
	printf("Invitacion acabada.......................................\n");
}

void ConfirmarInvitacion(char *p, ListaConectados *listaC, ListaPartidas *listaP, char respuesta[512], char posD_str[4], char posO_str[4])
{
	char confirmacion[10];
	char nomO[20];
	char nomD[20];
	
	printf("Preparando Confirmacion..................................\n");

	int invitados =  atoi(p);
	p = strtok(NULL,"/");
	strcpy(confirmacion,p);
	printf("Respuesta: %s \n",confirmacion);
	p = strtok (NULL,"/");
	strcpy(nomD,p);
	printf("Al que se lo proponen: %s \n",nomD);
	p = strtok (NULL,"/");
	strcpy(nomO,p);
	printf("El que propone: %s \n",nomO);
	int posD = BuscarPosicionLista(listaC, nomD);
	int posO = BuscarPosicionLista(listaC, nomO);
	sprintf(posD_str, "%d", posD);
	sprintf(posO_str, "%d", posO);
	printf("Posicion Propuesto: %s , Posicion Proponedor: %s \n", posD_str, posO_str);
	
	if (strcmp(confirmacion,"SI")==0)
	{
		printf("Confirmacion positiva \n");
		int r = UnirsePartida(nomO,nomD,listaC,listaP);
		printf("Resultado del UnirsePartida: %d\n", r);
		if (r==1)
			sprintf(respuesta,"10/OK/%s/%s",nomO,nomD);
		else
			sprintf(respuesta,"10/KO/%s/%s",nomO,nomD);
	}
	else
		sprintf(respuesta,"10/KO/%s/%s",nomO,nomD);
	
	printf("Confirmar invitacion terminado.......................\n");
}



//______________________________________________________________________________
//ATENDER CLIENTE:

int contador = 0;
void *AtenderCliente (void *socket)
{  
//    miLista.num = 0;
    int sock_conn;
    int *s;
    s=(int *) socket;
    sock_conn= *s;
    char conectado[300];
    int contadorSocket=0;
    
    int notificar=0;
    char notificacion [512];
	int pedir=0;
    char peticion[512];
    char respuesta[512];
    int ret;
	
	//char socketD_str[4];
	//char socketD2_str[4];
	//char socketD3_str[4];
	char posD_str[4];
	char posD2_str[4];
	char posD3_str[4];
	char posO_str[4];
	char invitados_str[4];
    
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
		printf("Error al crear la connexion: %u %s \n", mysql_errno(conn), mysql_error(conn));
		exit(1);
    }
    
    conn = mysql_real_connect(conn,"localhost","root","mysql","M1_BBDD",0,NULL,0);
    //conn = mysql_real_connect(conn,"shiva2.upc.es","root","mysql","M1_BBDD",0,NULL,0);

    if (conn == NULL)
    {
		printf("Error al inicializar la conexion: %u %s \n", mysql_errno(conn), mysql_error(conn));
		exit(1);
    }
	
    int terminar = 0;
    while (terminar==0)
    {  	 
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibida una peticion\n");
		peticion[ret]='\0';
   	   	 
		char *p = strtok( peticion, "/");
		int codigo =  atoi(p);
		p = strtok(NULL,"/");
   	 
		//----------------------------------------------------------------------------------
		
		//DESCONECTAR
		if (codigo == 0)
		{
			pthread_mutex_lock( &mutex );
			int desc = Desconectar(&miLista, p, respuesta);
			pthread_mutex_unlock( &mutex);
			notificar=1;
		}
		
		//PARTIDAS GANADAS
		if (codigo ==1){
			PartidasGanadas(p, respuesta);
			contador = contador +1;
			sprintf(respuesta, "%s/7/%d", respuesta, contador);
		}
		
		//PARTIDAS JUGADAS
		if (codigo == 2){
			PartidasJugadas(p, respuesta);
			contador = contador +1;
			sprintf(respuesta, "%s/7/%d", respuesta, contador);
		}
		
		//DAME GANADOR
		if (codigo == 3){
			DameGanador(respuesta);
			contador = contador +1;
			sprintf(respuesta, "%s/7/%d", respuesta, contador);
		}
		
		//REGISTRO
		if (codigo == 4)
			Registro(p, respuesta);
		
		//INICIO SESION
		if (codigo == 5)
		{
			InicioSesion(p, respuesta, sock_conn);
			pthread_mutex_lock( &mutex );
			DameConectados(&miLista,conectado);
			pthread_mutex_unlock( &mutex);
			notificar=1; 
		}
		//LISTA CONECTADOS
		if (codigo == 6)
		{
			pthread_mutex_lock( &mutex );
			DameConectados(&miLista,respuesta);
			pthread_mutex_unlock( &mutex);
		}
				
		//NUEVO CONECTADO
		if (codigo==7)
		{
			pthread_mutex_lock( &mutex );
			NuevoConectado(&miLista, p, sock_conn);
			DameConectados(&miLista,respuesta);
			pthread_mutex_unlock( &mutex);
			notificar=1;
   		}
		
		//INVITACION
		if (codigo==8)
		{
			pthread_mutex_lock( &mutex );
			//Invitacion(p, &miLista, &miListaPartidas, respuesta, socketD_str, socketD2_str, socketD3_str, invitados_str);
			Invitacion(p, &miLista, &miListaPartidas, respuesta, posD_str, posD2_str, posD3_str, invitados_str);
			pthread_mutex_unlock( &mutex);
   		}
		if (codigo==9)
		{
			//pthread_mutex_lock( &mutex );
			ConfirmarInvitacion(p, &miLista, &miListaPartidas, respuesta, posD_str, posO_str);
			//pthread_mutex_unlock( &mutex);
   		}
   	 
		printf("Respuesta: %s \n", respuesta);
		if ((codigo ==1)||(codigo==2)|| (codigo==3)||(codigo==4)|| (codigo==5)|| (codigo==6) || (codigo ==7))
		{
			printf("Socket por el que se enviara: %d \n", sock_conn);
			write (sock_conn, respuesta, strlen(respuesta));
		}
		else if (codigo==8)
		{
			int invitados = atoi(invitados_str);
			printf("invitados: %d \n", invitados);
			if (invitados == 1)
			{
				int posD = atoi(posD_str);
				printf("Pos destino: %d \n", posD);
				write (miLista.conectados[posD].socket,respuesta,strlen(respuesta));
			}
			else if (invitados == 2)
			{
				int posD = atoi(posD_str);
				int posD2 = atoi(posD2_str);
				printf("Pos destino: %d, %d \n", posD, posD2);
				write (miLista.conectados[posD].socket,respuesta,strlen(respuesta));
				write (miLista.conectados[posD2].socket,respuesta,strlen(respuesta));
			}
			else if (invitados == 3)
			{
				int posD = atoi(posD_str);
				int posD2 = atoi(posD2_str);
				int posD3 = atoi(posD3_str);
				printf("Pos destino: %d, %d, %d \n", posD, posD2, posD3);
				write (miLista.conectados[posD].socket,respuesta,strlen(respuesta));
				write (miLista.conectados[posD2].socket,respuesta,strlen(respuesta));
				write (miLista.conectados[posD3].socket,respuesta,strlen(respuesta));
			}
			else
				printf("Error");
		}
		else if (codigo==9)
		{
			int posD = atoi(posD_str);
			int posO = atoi(posO_str);
			printf("Pos destino: %d , Pos Host: %d \n", posD, posO);
			write (miLista.conectados[posD].socket,respuesta,strlen(respuesta));
			write (miLista.conectados[posO].socket,respuesta,strlen(respuesta));
		}
		else if (codigo==999)
		{
			for (int j=0; j<i;j++)		//miLista.num
			{
				write (miLista.conectados[j].socket,respuesta,strlen(respuesta));
			}
		}
		
		
		//CONTADOR DE FUNCIONES
		if ((codigo ==1)||(codigo==2)|| (codigo==3))
		{
			char numServicios[20];
			sprintf(numServicios, "7/%d", contador);
			for (int j=0; j<i;j++)		//miLista.num
			{
				if (miLista.conectados[j].socket != sock_conn)
					write (miLista.conectados[j].socket,numServicios,strlen(numServicios));
			}
		}
		pedir=0;
		
		
		//NOTIFICACION
		if (notificar==1)
		{
			//char cabecera [512] = "6/";
			//strcat (cabecera, conectado);
			DameConectados(&miLista,conectado);
			printf ("Notificación: %s \n",conectado);
			for (int j=0; j<i;j++)		//miLista.num
			{
				write (miLista.conectados[j].socket,conectado,strlen(conectado));
			}
		}
		notificar=0;
    }
    close(sock_conn);    
}



//______________________________________________________________________________
//MAIN:

int main(int argc, char *argv[])
{
    int sock_conn, sock_listen;
    struct sockaddr_in serv_adr;    
    
	int puerto =50079;
    
    if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");

    memset(&serv_adr, 0, sizeof(serv_adr));// inicializa en zero serv_addr.
    
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(puerto);
    if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");

    if (listen(sock_listen, 100) < 0)
		printf("Error en el Listen");

    contador=0;
    pthread_t  thread;
    i=0;
    
    //-----------------------------------------------------------------------------------------------

    for(;;)
    {
		printf ("Escuchando\n");

		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		sockets[i]=sock_conn;
   	   	 
		pthread_create(&thread, NULL, AtenderCliente, &sockets[i]);
		i++;
    }
}

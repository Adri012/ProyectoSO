 #include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>
#include <pthread.h>

//Estructura necesaria para acceso excluyente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Declaramos la estructura con un nombre y un Socket
typedef struct {
    char nombre[20];
    int socket;
} Conectado;

typedef struct {
    Conectado conectados [100];
    int num;
} ListaConectados;


//Variables SQL
MYSQL * conn;
int err;
MYSQL_RES * resultado;
MYSQL_ROW row;

//---------------------------------------------------------------------------------------

int contador;			//Acceso excluyente
int i;
int sockets [100];

ListaConectados miLista;



//______________________________________________________________________________
//FUNCIONES DE BUSQUEDA:

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
    	return lista->conectados[i].socket;			//Tambe conegut com a i
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


//Funcion que anade un nuevo conectado. Retorna 0 si se ha anadido correctamente y -1 si no se ha podido anadir debido a que la lista esta llena.
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


//Funcion que retorna 0 si elimina a la persona y -1 si ese usuario no esta conectado.
int Desconectar (ListaConectados *lista, char p[200], char respuesta[512])
{
    char nombre[20];
	strcpy(nombre,p);
	
	int pos = DameSocket (lista,nombre);
    if (pos == -1)
       	return -1;  
    else
    {
		int i;
		pthread_mutex_lock(&mutex);	
		for (i = pos; i < lista->num-1; i++)
		{
			strcpy(lista->conectados[i].nombre, lista->conectados[i+1].nombre);
			lista->conectados[i].socket = lista->conectados[i+1].socket;
		}
		lista->num--;
		pthread_mutex_unlock(&mutex);
		sprintf(respuesta,"0/Hasta la proxima!");
		return 0;
    }
}




//______________________________________________________________________________
//CONSULTAS PANTALLA DE INICIO:


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
	sprintf(consulta, "SELECT Jugador.nombre FROM Jugador WHERE Jugador.partidas_ganadas = MAX(Jugador.partidas_ganadas)");
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
//ATENDER CLIENTE:

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
    char peticion[512];
    char respuesta[512];
    int ret;
    
    
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
		printf("Error al crear la connexion: %u %s \n", mysql_errno(conn), mysql_error(conn));
		exit(1);
    }
    
    conn = mysql_real_connect(conn,"localhost","root","mysql","BBDD",0,NULL,0);
    //conn = mysql_real_connect(conn,"shiva2.upc.es","root","mysql","BBDD",0,NULL,0);

    if (conn == NULL)
    {
		printf("Error al inicializar la conexion: %u %s \n", mysql_errno(conn), mysql_error(conn));
		exit(1);
    }
    
    int terminar = 0;
    while (terminar==0)
    {
		// char usuario[20];
		// char contrasena[20];
		// int partidas_ganadas;
		// char consulta[80];
		// char consulta_id[80];
   	 
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibida una peticion\n");
		peticion[ret]='\0';
   	   	 
		char *p = strtok( peticion, "/");
		int codigo =  atoi(p);
		p = strtok(NULL,"/");
   	 
		int contador = 0;
		//----------------------------------------------------------------------------------
		
		//DESCONECTAR
		if (codigo == 0)
		{
			pthread_mutex_lock( &mutex );
			int desc = Desconectar(&miLista, p, respuesta);
			DameConectados(&miLista, conectado);
			pthread_mutex_unlock( &mutex);
			notificar=1;
		}
		
		//PARTIDAS GANADAS
		if (codigo ==1)
			PartidasGanadas(p, respuesta);
		
		//PARTIDAS JUGADAS
		if (codigo == 2)
			PartidasJugadas(p, respuesta);
		
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
		if (codigo==9)
		{
			pthread_mutex_lock( &mutex );
			NuevoConectado(&miLista, p, sock_conn);
			DameConectados(&miLista,respuesta);
			pthread_mutex_unlock( &mutex);
			notificar=1;
   		}
   	 
		
		printf("Respuesta: %s \n", respuesta);
		write (sock_conn,respuesta, strlen(respuesta));

   	 
		//CONTADOR DE FUNCIONES
		if ((codigo ==1)||(codigo==2)|| (codigo==3)||(codigo==4)|| (codigo==5)|| (codigo==6))
		{
			pthread_mutex_lock( &mutex ); //No me interrumpas ahora
			contador = contador +1;
			printf("COOONTADOR\n");
			pthread_mutex_unlock( &mutex); //ya puedes interrumpirme
		}

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
    
	int puerto =50026;
    
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


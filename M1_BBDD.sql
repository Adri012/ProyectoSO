DROP DATABASE IF EXISTS M1_BBDD;
CREATE DATABASE M1_BBDD;
USE M1_BBDD;

CREATE TABLE Jugador(
	id INTEGER,
	username VARCHAR(20),
	password VARCHAR(20),
	email VARCHAR(20),
	partidas_ganadas INTEGER,
	partidas_jugadas INTEGER,
	PRIMARY KEY (id)
)ENGINE=InnoDB;

CREATE TABLE Partida(
	id INTEGER,
	fecha VARCHAR(15),
	hora VARCHAR(15),
	palabra VARCHAR(20),
	ganador VARCHAR(20),
	PRIMARY KEY (id)
)ENGINE=InnoDB;

CREATE TABLE Participacion(
	idJug INTEGER,
	idPart INTEGER,
	puntos INTEGER,
	FOREIGN KEY (idJug) REFERENCES Jugador(id),
	FOREIGN KEY (idPart) REFERENCES Partida(id)
)ENGINE=InnoDB;

INSERT INTO Jugador VALUES (1,'Adria','Adria1','adria@gmail.com',2,5);
INSERT INTO Jugador VALUES (2,'Chao','Chao2','chao@gmail.com',1,5);
INSERT INTO Jugador VALUES (3,'Angel','Angel3','angel@gmail.com',1,4);
INSERT INTO Jugador VALUES (4,'David','David4','david@gmail.com',1,3);

INSERT INTO Partida VALUES (1,'11','15:56','Mesa','Adria');
INSERT INTO Partida VALUES (2,'11','18:13','Armario','Adria');
INSERT INTO Partida VALUES (3,'12','12:54','Ordenador','Chao');
INSERT INTO Partida VALUES (4,'12','13:05','Libro','Angel');
INSERT INTO Partida VALUES (5,'13','09:42','Auriculares','David');

INSERT INTO Participacion VALUES (1,1,5);
INSERT INTO Participacion VALUES (2,1,0);
INSERT INTO Participacion VALUES (3,1,5);
INSERT INTO Participacion VALUES (4,1,0);
INSERT INTO Participacion VALUES (1,2,0);
INSERT INTO Participacion VALUES (2,3,5);
INSERT INTO Participacion VALUES (3,4,5);
INSERT INTO Participacion VALUES (4,4,0);
INSERT INTO Participacion VALUES (1,5,0);
INSERT INTO Participacion VALUES (4,5,5);
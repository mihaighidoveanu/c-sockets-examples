#include<stdio.h> //printf
#include<string.h> //strlen
#include<sys/socket.h> //socket
#include<arpa/inet.h> //inet_addr

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	int raspuns_server; //raspunsul primit de la server
	int numar; //numarul ce va fi trimis serverului pentru a verifica daca este sau nu palindrom

	//creare socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("Nu s-a putut crea socketul"); 
	}
	puts("Socket creat"); 

	//configurarea setarilor serverului
	server.sin_family = AF_INET; //familia de adrese din care face parte
	server.sin_port = htons(6724); //portul ce va fi folosit -unic ales, conform unui algortim
	server.sin_addr.s_addr = inet_addr("37.120.249.45"); //setarea adresei IP a serverului ce urmeaza a fi folosit

	//conectare la server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Conexiune esuata"); 
		exit(1); 
	}
	puts("Conectat\n");

	//introducerea numarului ce doreste a fi verificat
	printf("Introduceti numarul: ");
	scanf("%d", &numar);
		
	//trimiterea numarului la server, in cazul in care trimiterea numarului se realizeaza cu succes, atunci se va trimite un mesaj de eroare
	if (send(sock, &numar, sizeof(int), 0) < 0)
	{
		puts("Trimiterea numarului a esuat");
		exit(1);
	}

	//Primirea raspunsului de la server, raspuns ce va fi adaugat in variabila raspuns_server,variabila ce va putea fi 1 in cazul in care numarul este palindrom sau 0 in cazul in care numarul nu este palindrom in cazul unei erori, se va afisa un mesaj
	if (recv(sock, &raspuns_server, sizeof(int), 0) < 0)
	{
		puts("Primire esuata");
		return;
	}

	puts("Raspunsul serverului este :\n");
	printf("%d\n", raspuns_server);
	
//inchiderea conexiunii
	close(sock);
	return 0;
}

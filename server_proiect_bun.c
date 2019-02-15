#include<stdio.h> //biblioteca pentru printf
#include<string.h> //biblioteca pentru strln
#include<sys/socket.h> //socket
#include<netinet/in.h> 
#include<arpa/inet.h>//inet_addr
#include<unistd.h> //write

// o functie care verifica daca un numar este sau nu palindrom
int verificaPalindrom(int numar)
{ // un numar este palindrom daca este egal cu inversul sau
	//vom avea nevoie de o variabila invers in care vom stoca inversul numarului, ceasta fiind la inceput 0
	//o a doua variabila, o copie a numarului ce este verificat, pentru ca, in urma prelucrarilor de calcul al inversuli, vom pierde numarul initial
	//o a treia variabila este cifra, in care vom stoca ultima cifra din numar

	int invers = 0, copie, cifra, rezultat;
	copie = numar; //stocam numarul ce vrem a fri verificat intr-o alta variabila, copie; 

// pana ajungem la ultima cifra a numarului, vom repeta urmatoarele operatii
	while (numar != 0)
	{
		cifra = numar % 10; //se extrage ultima cifra a numarului
		invers = invers * 10 + cifra; //se adauga la invers ultima cifra a numarului
		numar = numar / 10; // se taie ultima cifra din numar
	}
//verificare: daca numarul, retinut in copie, este egal cu inversul, atuncieste palindrom, altfel, nu este palindrom
	if (copie == invers)
	{
		rezultat = 1;
		printf("%d este palindrom. ", copie);
	}

	else
	{
		rezultat = 0;
		printf("%d nu este palindrom. ", copie);
	}

	return rezultat; 
}

int main(int argc, char *argv[])
{

	struct sockaddr_in client, server; //adresele client, server
	int numar; //numarul ce va fi preluat de catre server de la client si verificat daca este sau nu palindrom
	int s, n, sock_client; //n= lungimea clientului
	int rezultat; //rezultatul ce va fi trimis clientului, va avea valorile 1, in caz ca numarul este palindrom, sau 0, in caz ca numarul nu este palindrom
         

	//creare socket, avand argumentele: 1) domeniul de internet 2)streamul 3) protocolul default
	s = socket(AF_INET, SOCK_STREAM, 0); 
	if (s < 0)
	{
		perror("Eroare la deschiderea socket-ului");
		exit(1);
	}
	//configurarea setarilor serverului
	server.sin_family = AF_INET; //familia de adrese din care face parte
	server.sin_port = htons(6724); //portul ce va fi folosit -unic ales, conform unui algortim
	server.sin_addr.s_addr = inet_addr("37.120.249.45"); //setarea adresei IP a serverului ce urmeaza a fi folosit
	
	//legarea adresei cu socket-ul
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) < 0){
		perror("Eroare la binding"); 
		exit(1); 
	};

	//listen pe socket - o singura intrarea, apoi nu se vor mai accepta alte intrari
	//in cazul in care se face listening, serverul va trimite un mesaj, in cazul in care nu se face, va trimite un mesaj de eroare
	if (listen(s, 1)==0)
		printf("Listening\n");
	else 
		printf("Eroare listening\n"); 
  // acceptarea crearii unui noi socket
	n = sizeof client; 
	sock_client = accept(s, (struct sockaddr*)&client, &n); 

	//eroare la acceptarea clientului
	if (sock_client < 0)
	{
		perror("Eroare la acceptarea clientului"); 
		exit(1); 
	}

	//primirea numarului de la client
	recv(sock_client, &numar, sizeof(int), 0); 
	puts("Numarul trimis de client este: "); 
	printf("%d \n", numar); 
	//verificare numarului
	rezultat= verificaPalindrom(numar); 
        

	//trimiterea rezultatului catre client
	write(sock_client, &rezultat, sizeof(int)); 
	
	return 0; 
}
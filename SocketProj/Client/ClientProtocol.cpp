#include "ClientProtocol.h"
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define MAX_LENGTH 255
void DieWithError(const char *errorMessage) /* External error handling function */
{
	perror(errorMessage);
	exit(1);
}
// pass server IP and port # @ execution
int main(int argc, char *argv[])
{
	int sock;                        /* Socket descriptor */
	struct sockaddr_in echoServAddr; /* Echo server address */
	struct sockaddr_in fromAddr;     /* Source address of echo */
	unsigned short echoServPort;     /* Echo server port */
	unsigned int fromSize;           /* In-out of address size for recvfrom() */
	char *servIP;                    /* IP address of server */
	int nBytes;              		 /* Length of received response */
	char buffer[MAX_LENGTH];
	char* input; // read from std::in to be sent to server

	if (argc < 3)    /* Test for correct number of arguments */
	{
		fprintf(stderr,"Usage: %s <Server IP address> <Echo Port>\n", argv[0]);
		exit(1);
	}

	servIP = argv[1];           /* First arg: server IP address (dotted quad) */
	echoServPort = atoi(argv[2]);  /* Second arg: Use given port, if any */

	printf( "Arguments passed: server IP %s, port %d\n", servIP, echoServPort );

	/* Create a datagram/UDP socket */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		DieWithError("socket() failed");

	/* Construct the server address structure */
	memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
	echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
	echoServAddr.sin_port   = htons(echoServPort);     /* Server port */

	printf("Ready to send to server.\n");

	/* Send the struct to the server */
	while(1)
	{
		scanf("%s", input); // user input from std::in

		// send input to server
		if (sendto(sock, input, strnlen(input, MAX_LENGTH), 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) != strnlen(input, MAX_LENGTH))
			DieWithError("sendto() sent a different number of bytes than expected");

		/* Receive a response */
		fromSize = sizeof(fromAddr);
		if ((nBytes = recvfrom(sock, buffer, MAX_LENGTH, 0, (struct sockaddr *) &fromAddr, &fromSize)) > MAX_LENGTH )
			DieWithError("recvfrom() failed");

		if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
		{
			fprintf(stderr,"Error: received a packet from unknown source.\n");
			exit(1);
		}

		printf("\nClient received message from server: ``%s''\n", buffer );
	}

	close(sock);
	exit(0);
}





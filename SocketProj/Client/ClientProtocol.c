#include "defns.h" // "ClientProtocol.h" for us

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */



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

	struct sample example; // will be deleted
// generic set-up shit
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
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */ //********************************** IP string -> IP format **************
	echoServAddr.sin_port   = htons(echoServPort);     /* Server port */

/***********************************************************************************************************************************
		run in infinite loop until exit command read?
		read shit from std::in
		parse it
		if command is recognized:
			print that command is being processed
			call method corresponding to message (implemented in "ClientProtocol.h")
			print that command was processed and sent to server @ IP_ADDR
			wait for server response
			print server response assuming nothing weird happens
		else:
			print that command was not recognized & not sent to server


			 **** idk what else will be neccessary yet; I left the rest of the example code below to reference

*************************************************************************************************************************************/
	strcpy( example.message, "Request" );
	printf( "\nClient sending <message,n> in struct to: <%s,%d>\n", example.message, example.n );

	/* Send the struct to the server */
// send shit
	if (sendto(sock, &example, sizeof(struct sample), 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) != sizeof(struct sample))
		DieWithError("sendto() sent a different number of bytes than expected");

	/* Receive a response */
// get shit
	fromSize = sizeof(fromAddr);

	if ((nBytes = recvfrom(sock, &example, sizeof(struct sample), 0, (struct sockaddr *) &fromAddr, &fromSize)) > sizeof(struct sample) )
		DieWithError("recvfrom() failed");

	if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
	{
		fprintf(stderr,"Error: received a packet from unknown source.\n");
		exit(1);
	}
// do shit
	printf("\nClient received message from server: ``%s''\n", example.message );    /* Print the echoed arg */
	printf("Client received value n from server: %d\n", example.n );    /* Print the echoed arg */


// kill our process
	close(sock);
	exit(0);
}





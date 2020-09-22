#include "ServerProtocol.h"

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define MAX_LENGTH 255;

void DieWithError(const char *errorMessage) /* External error handling function */
{
	perror(errorMessage);
	exit(1);
}
// pass the port we're listening to
int main(int argc, char *argv[])
{
	int sock;                        /* Socket */
	struct sockaddr_in ServAddr; /* Local address */
	struct sockaddr_in ClntAddr; /* Client address */
	unsigned int cliAddrLen;         /* Length of incoming message */
	unsigned short ServPort;     /* Server port */
	int recvMsgSize;                 /* Size of received message */
	char buffer[MAX_LENGTH];  // message we get and send to client



	if (argc != 2)         /* Test for correct number of parameters */
	{
		fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
		exit(1);
	}

	ServPort = atoi(argv[1]);  /* First arg:  local port */

	/* Create socket for sending/receiving datagrams */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		DieWithError("socket() failed");

	/* Construct local address structure */
	memset(&ServAddr, 0, sizeof(ServAddr));   /* Zero out structure */
	ServAddr.sin_family = AF_INET;                /* Internet address family */
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	ServAddr.sin_port = htons(ServPort);      /* Local port */

	/* Bind to the local address */
	if (bind(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
		DieWithError("bind() failed");

	for (;;) /* Run forever */
	{
		/***********************************************************************************************************************************
	get message from client
	print that message was received
	parse message
	if message is recognized by server:
		call method corresponding to message (implemented in "ServerProtocol.h")
	else:
		print that message was not recognized
		send FAILURE message to client

		 **** idk what else will be neccessary yet; I left the rest of the example code below to reference

		 *************************************************************************************************************************************/
		/* Set the size of the in-out parameter */
		cliAddrLen = sizeof(ClntAddr);

		/* Block until receive message from a client */
		if ((recvMsgSize = recvfrom(sock, buffer, MAXlength, 0, (struct sockaddr *) &ClntAddr, &cliAddrLen)) < 0)
			DieWithError("recvfrom() failed");

		buffer[ recvMsgSize ] = '\0';

		printf("\nServer handling client at IP address: %s\n", inet_ntoa( ClntAddr.sin_addr ));
		printf("Server receives string: %s\n", buffer );

		/* parse buffer   ** if client sends a string
		 switch on first char in buffer
		 	 r:
		 	 	 validate input
		 	 	 if first 8 chars in buffer == register
		 	 	 	 get & store parameters
		 	 	 	 call register_usr()
		 	 	 else:
		 	 	 	 print that input was invalid and send FAILURE message
		 	 d:
		 	 	 validate input
		 	 	 if valid:
		 	 	 	 get & store parameters
		 	 	 	 call deregister_usr()
		 	 	 else:
		 	 	 	 print that input was invalid and send FAILURE message
		 	 s:
		 	 	 if 7th char = r:
		 	 	 	 validate input
		 	 	 	 if valid:
		 	 	 	 	 get & store parameters
		 	 	 	 	 call deregister_usr()
		 	 	 	 else:
		 	 	 	 	 print that input was invalid and send FAILURE message
		 	 	 else if = c:
		 	 	 	 validate input
		 	 	 	 if valid:
		 	 	 	 	 get & store parameters
		 	 	 	 	 call deregister_usr()
		 	 	 	 else:
		 	 	 	 	 print that input was invalid and send FAILURE message

		 	 	 else:
					print that input was invalid and send FAILURE message

		*/

		/* Send response to the client */
	//	if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &ClntAddr, sizeof(ClntAddr)) != strlen(buffer))
	//		DieWithError("sendto() sent a different number of bytes than expected");

	}
	/* NOT REACHED */
}

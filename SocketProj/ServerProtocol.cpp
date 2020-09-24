#include "ServerProtocol.h"
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define MAX_LENGTH 255

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

	user_info* u_head = NULL;
	ring* r_head = NULL;

	// strings to store command args from client
	char* name;
	char* ip;
	int* ports;
	// ... more to be added

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

	printf("Server is ready.\n");

	for (;;) /* Run forever */
	{
		/* Set the size of the in-out parameter */
		cliAddrLen = sizeof(ClntAddr);

		// get command from client
		if ((recvMsgSize = recvfrom(sock, buffer, MAX_LENGTH, 0, (struct sockaddr *) &ClntAddr, &cliAddrLen)) < 0)
			DieWithError("recvfrom() failed");
		buffer[ recvMsgSize ] = '\0';

		/*
		 * if buffer.compare("register") == 0
			{
			  if ((recvMsgSize = recvfrom(sock, name, MAX_LENGTH, 0, (struct sockaddr *) &ClntAddr, &cliAddrLen)) < 0)
							DieWithError("recvfrom() failed");

			   . . . get & store rest of args

			   if(register(name, ip, ports, u_head))  // function returns true if SUCCESS, false for FAILURE
			   {
					SUCCESS
					send success message to client
			   }
			   else
			   {
					// FAILURE
					if (sendto(sock, "FAILURE\n", strlen("FAILURE\n", MAX_LENGTH), 0, (struct sockaddr *) &ClntAddr, sizeof(ClntAddr)) != strlen("FAILURE\n", MAX_LENGTH))
						DieWithError("sendto() sent a different number of bytes than expected");

			   }
			}

			else if(buffer.compare("deregister")) == 0
			{
				... same process as before
			}

			else if(buffer.compare("setup-ring"))
			{
				. . . get args

				r_head = setup_ring(n, name, u_head);
				if(r_head != NULL)
				{
					// The server returns a return code of SUCCESS, a ring-id, the ring size n, and a list of n users that together
					// will form the O-RING to user-name. The n users are given by tuples consisting of, at a minimum, for each
					// user its user-name, IPv4-address, two port(s) for communication in an O-RING, with the tuple of
					// user-name listed first. Receipt of SUCCESS at user-name involves several additional steps to accomplish
					// the setup of the O-RING. These steps which include ring orientation and leader election are described in 3.1.

				}
				else
				{
					send FAILURE message
				}
			}

			else if(buffer.compare("setup-complete")
			{
					. . . get args
				if(setup_complete(u_head, r_head, id, name, port))
				{
					send SUCCESS message
				}
				else
				{
					send FAILURE
				}
			}
		 */





		/* Send response to the client */
		//	if (sendto(sock, buffer, strlen(buffer, MAX_LENGTH), 0, (struct sockaddr *) &ClntAddr, sizeof(ClntAddr)) != strlen(buffer, MAX_LENGTH))
		//		DieWithError("sendto() sent a different number of bytes than expected");

	}
	/* NOT REACHED */
}

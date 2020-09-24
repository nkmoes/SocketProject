#ifndef SERVERPROTOCOL_H_
#define SERVERPROTOCOL_H_

#include <string.h>
#include <stdlib.h>

// linked list of user_info structs
typedef struct user_info
{
	char usr_name[16]; // extra space for \0
	char usr_ip[16];
	int* usr_ports; // array for port #
	char state; // f = free; r = registered; i = in ring; l = leader
	struct user_info* next; // pointer to next user
	struct user_info* prev;
} user_info;

typedef struct ring
{
	int n; // num of users in ring
	int id; // ring id; first ring id = 1, id++ for each ring instance
	int compute_port; // port # of leader that is listened to for compute commands from server
	user_info* users; // linked list of users in the ring
	ring* next; // pointer to next ring

} ring;


user_info* get_user(user_info* head, char* cmp)
{
	/*
	 * while ptr != NULL
	 * compare current username to arg
	 * if equal return pointer to curr user node
	 * else continue loop
	 * if loop ends (ptr == NULL) return NULL ptr
	 */

	return NULL;
}

void add_user(user_info* head, user_info* user)
{
	/*
	 * loop to end of list (where next == NULL)
	 * set next = user
	 * make sure user->next is NULL
	 */
}

void del_user(user_info* head, user_info* user)
{
	/*
	 * ptr = get_user
	 *
	 * ptr->prev->next = NULL
	 * ptr->next->prev = NULL
	 */

}

void add_ring(ring* head, ring* r)
{
	// same as add_user but with ring
}

void del_ring(ring* head, ring* r)
{
	// ....
}


/*
 * parses, validates, and stores user data if all conditions are met
 * returns true if SUCCESS and false if FAILURE
 */
bool register_usr(char* name, char* ip, int* ports, user_info* head)
{
	/*
	 // check that name fits parameters
	 check that length of name (l) is 1 <= l <= 15
	 check that name consists of letters only

   	 // check that at least 1 port given

	 // check if usr name is unique
	 if get_user() == NULL
		 return FAILURE

	 // check for same end-host IP addr.
	 ptr = get_user(head, ip)
	 if ptr != NULL:
		check that port #s are unique
		if matching port #s:
		 	 return FAILURE

	// add usr to info_base
	 * create new user and set member values (state = f)
	 * call add_user

	return SUCCESS
	 */
	return true;
};


/*
2. deregister <user-name>
This command returns FAILURE if the user has state InRing. Otherwise,
the record for user-name is removed from the state information base, and the server responds with SUCCESS.
The client process making the request then exits the application, i.e., it terminates.
 */
bool deregister(char* name, user_info* head)
{
	/*
	 * ptr =  get_user(head, name)
	 *  if ptr->state != i
	 *  	del_user(head, ptr)
	 *  	SUCCESS
	 *  else
	 *  	FAILURE
	 */
	return true;
}


/*
3. setup-ring <n> <user-name>      *just need to set up sockets (not orientation or election)
where n >= 3 is an odd integer. This command initiates the construction
of a ring of size n by the named user. In this project, while multiple rings may exist at any time, each user may
only participate in one ring at a time.
This command results in a return code of FAILURE if:
 - The user-name is not registered or is not Free.
 - n !>= 3 or is not odd.
 - There are not at least n > 1 other Free users, besides user-name, registered with the server.

Otherwise, the server selects at random n-1 Free users and updates each one’s state to InRing including
that of user-name. Because you need to support the existence of multiple O-RINGS, assign an identifier for
each ring set up.
The server returns a return code of SUCCESS, a ring-id, the ring size n, and a list of n users that together
will form the O-RING to user-name. The n users are given by tuples consisting of, at a minimum, for each
user its user-name, IPv4-address, two port(s) for communication in an O-RING, with the tuple of
user-name listed first. Receipt of SUCCESS at user-name involves several additional steps to accomplish
the setup of the O-RING. These steps which include ring orientation and leader election are described in x3.1.
 */

ring* setup_ring(int n, char* name, user_info* head)
{
	/*
	 * if ((n is not odd or >= 3) || (# free usrs > 1) || (get_user()->state != f or r))
	 * 	return NULL
	 * else
	 * 	create ring struct (use malloc)
	 * 	add get_user(name) to ring list & set state to i   // user who called setup_ring should be head of ring list
	 * 	randomly select n-1 free users
	 *	 	* maybe gen random number < # users and loop through that many times
	 * 		*  if user is !free
	 * 				do it again
	 * 		*  else
	 * 				add_user to ring list
	 * 				set user state to i
	 * 	set values for ring members
	 * 		num = n
	 * 		set id *how to keep track of number of rings.. just use a static global var?
	 * 		set next to NULL
	 *
	 * 	return pointer to new ring struct
	 */
	return NULL;
}

/*
4. setup-complete <ring-id> <user-name> <port>
Receipt of a setup-complete command from
user-name indicates that all the steps required to setup the O-RING with identifier ring-id have been completed.
In addition, it indicates that user-name is the process that was elected the leader of the ring and that,
as the leader, it will listen for compute messages on the port number that is labelled zero (port) on completion
of the ring orientation algorithm. The server should set the state of user-name to Leader in the state
information base, and store or mark the port of the process to be used in response to any compute commands.
The server responds to user-name with SUCCESS.
 */
bool setup_complete(user_info* u_head, ring* r_head, char* id, char* name, int port)
{
	/*
	 * parse id (int), name, port (int)
	 * set state of get_user(head, name) to l
	 * set compute_port in ring to port
	 * SUCCESS
	 */
	return true;
}


/*
5. compute <user-name>
This command is initiates computation of a function in an O-RING. It returns
FAILURE if no O-RING exists, the user is not registered, or the user is registered but is not Free. Otherwise,
the server chooses an existing O-RING, and returns its ring-id, size n, and a 3-tuple corresponding to the
user-name, IPv4-address, and port of the leader of ring-id. (Clearly, any ring that may be in the
process of being torn down should not be selected!) Finally, the return code is set to SUCCESS and the response
is sent to user-name. Receipt of SUCCESS at the client involves several additional steps to perform the
computation, and the functions to support, described in x3.2.
 */


/*
6. teardown-ring <ring-id> <user-name>
where user-name is the leader of O-RING. This command
initiates the deletion of the O-RING with identifier ring-id. This command returns FAILURE if the
user is not the leader of the O-RING. Otherwise, the server responds to the user with SUCCESS. Receipt of
SUCCESS at the process involves several steps to delete the O-RING, as described in x3.3. Nothing should
interrupt the tear down of a ring.
 */


/*
7. teardown-complete <ring-id> <user-name>
where user-name is the leader of the O-RING. This
command indicates that the O-RING with identifier ring-id has been torn down. The server returns FAILURE
if the process making the request is not the leader of the O-RING. Otherwise, the server changes the state of
each user in the ring to Free; these users may now be used in a future setup-ring or compute functions of
any other existing O-RING. The server responds to the former leader with SUCCESS.
 */




#endif /* SERVERPROTOCOL_H_ */

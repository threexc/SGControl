/* A simple server to echo back commands received via TCP/IP. Used for
testing input of SCPI command sequences when a connection to the actual
signal generator is not available.

This code is based off of the code for the echo server found at
https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/,
but it has been modified to use memset rather than bzero, as well as some other
minor changes. */

/* TODO: Change the main function to parse arguments so that the port to
listen on isn't hard-coded at 5025 (default for E4438C) */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 100

int main(void)
{

		/* Buffer for consecutive lines read, maximum size 100 */
    char str[BUF_SIZE];

		/* File descriptors for listen and accept function calls */
    int listen_fd, comm_fd;

		/* Standard struct for address data */
    struct sockaddr_in servaddr;

    /* Get the file descriptor for the socket to use */
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* Clear the servaddr struct with zeros */
		memset((&servaddr), '\0', sizeof(servaddr));

    /* Set up the struct. Note that 5025 is the same as the port used by the
		E4438C for TCP/IP connections */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(5025);

    /* Bind to the socket using the data we have just provided */
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    /* Listen for a connection on the socket */
    listen(listen_fd, 10);

    /* Pull the first connection request received by the listen function call,
		and get the file descriptor for use in the while loop below */
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

    while(1)
    {
        /* Zeroize the buffer */
				memset(str, '\0', BUF_SIZE);

				/* Read in a received message. TODO: Look into alternatives, possibly
				recv() */
        read(comm_fd, str, BUF_SIZE);

        printf("Echoing back - %s",str);

        /* Write the received message to stdout */
        write(comm_fd, str, strlen(str)+1);

    }

		return 0;
}

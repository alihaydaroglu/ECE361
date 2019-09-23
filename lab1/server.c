#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	int port = atoi(argv[1]);
	printf("Listening to port %d \n", port);
	
	char ipstr[INET6_ADDRSTRLEN];
	int status; 
	int sockfd;
	int byte_count;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct sockaddr_storage their_addr;
	socklen_t fromlen;
	char buf[512];


	// As per the Beej guide, set up our hints
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use ipv4 or v6, don't care
	hints.ai_socktype = SOCK_DGRAM; // choose UDP over TCP
	hints.ai_flags = AI_PASSIVE; // fill in the IP for us

	// error check the getaddrinfo call
	if ((status = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0){
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	// get a socket file descriptor
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	//bind socket to a port, and error catch
	if ((bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) != 0){
		fprintf(stderr, "binding error");
		exit(1);
	}

	fromlen = sizeof their_addr;
	byte_count = recvfrom(sockfd, buf, sizeof buf, 0, &their_addr, &fromlen);

	printf("recv()'d %d bytes of data in buf\n", byte_count);
	//printf("from IP address %s\n",
    	//	inet_ntop(their_addr.ss_family,
	//		(their_addr.ss_family == AF_INET?
        //  			((struct sockadd_in *)&their_addr)->sin_addr:
        // 			((struct sockadd_in6 *)&their_addr)->sin6_addr),
        //		ipstr, sizeof ipstr)); 

       
	// free stuff up once we're done
	freeaddrinfo(servinfo);


	return 0;
}



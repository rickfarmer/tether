#include "proxy.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define PORT "8080"		// Default port, if none specified as an arg
#define CONN_QUEUE 15 	// Connections allowed to queue
#define BUFFER_SIZE 1500000
#define USERAGENT " "

void *get_in_addr(struct sockaddr *sa);
char *build_http_request(char *host, char *page);
void *handle_request(int s);


// --------------------------------
// For OSX/Linux use:
// $ cd /<path>/tether/tether-android/jni
// $ gcc -o proxy proxy.c
// Then run using:
// $ ./proxy

// For use as a standalone program uncomment the following "main" class
// and proxy_port arg and comment out the int proxy(int start) below, e.g.
//
// int main(int argc, char **argv) {
//  // For CLI Usage: ./proxy [port]
//	char *proxy_port = *(argv + 1);
// --------------------------------
			
// --------------------------------
// For Android use:
// $ cd /<path>/tether/tether-android
// $ ndk-build
// Then run using:
// "run > build" in STS with the device connected
//
// --------------------------------
// For iOS use:
// Include the proxy.c library in the xcode project and wire it up to a UI element

// --------------------------------
// For use as a library for inclusion in an iOS or Android app
// uncomment the following "int proxy(int start)" (if commented) below
// and proxy_port arg and comment out the standalone program int main and *proxy_port argv, e.g.
//
// int proxy(int start) {
//	char *proxy_port = PORT;
// --------------------------------

int proxy(int start) {
	char *proxy_port = PORT;

	int sockfd, new_fd = 0;	// Sockets from local machine to proxy server
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo, *p;
	struct sockaddr_storage their_addr;
 	char s[INET6_ADDRSTRLEN];
	

	socklen_t addr_size;

	int yes = 1;
	signal (SIGCHLD, SIG_IGN);
	int pid;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;


	if ((status = getaddrinfo(NULL, proxy_port, &hints, &servinfo)) != 0) {
		printf ("Error getting address info");
		exit (1);
	}

	// Step 1: socket()
	// The socket() function returns a socket file descriptor called sockfd
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, 0);

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("Error getting server socket");
			continue;
		}

		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("Error setting the socket options");
			exit (1);
		}

		// Step 2: bind()
		// Attempt to bind to the port we passed into getaddrinfo()
		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("Error on server bind");
			continue;
		}

		break;
	}

	// Step 3: listen() 
	// Listen on the specified port for incoming connections
	if ((listen(sockfd, CONN_QUEUE)) == -1) {
		printf("Error on listen\n");
		exit(1);
    } else {
      printf("Listening on port %s...\n", proxy_port);
    }

	// Step 4: accept()
	// Accept the incoming connection
	while(1) {
		addr_size = sizeof(their_addr);
		new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
		//printf("new_fd = %d\n", new_fd);
		if (new_fd == -1) {
			printf("Error on accept\n");
			exit(1);
		}

		// Print the address of the connecting client
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *) &their_addr), s,
			sizeof(s));
			
		 printf("Server has incoming connection from %s\n", s);

		 pid = fork();
		 if (pid < 0)
			 perror("Error on fork");
		 if (pid == 0)
		 {
			 close (sockfd);
			 //Step 5: read/recv & write/send loop
			 handle_request(new_fd);
			 exit(0);
		 } else {
			 // Step 6: close()
			 close(new_fd);
		 }
    }

  return 0;
}


// Get the client information (inbound)
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}


// Handle the request
void *handle_request(int s) {
	char buftemp[65000];
	char *bufquery = (char *) (malloc(65000 * sizeof(char)));
	char err400[3000] = "HTTP 400: Bad Request\r\n";

	char method[10000], protocol[10000], host[10000], path[10000], url[10000],
		hostact[10000], field[10000];
	int numbytes;
	int port=80; //default is 80
	char *pch;
	int i = 0;
  
	// Handle socket
	int sockf = 0;
	struct addrinfo hintsf;
	struct addrinfo *servinfo, *p;
	char sf[INET6_ADDRSTRLEN];
	int rv;
	char *buf2 = (char *) (malloc(BUFFER_SIZE));
 
	memset(&hintsf, 0, sizeof hintsf);
	hintsf.ai_family = AF_UNSPEC;
	hintsf.ai_socktype = SOCK_STREAM;
	hintsf.ai_flags = AI_PASSIVE;

	if ((numbytes = recv (s, buftemp, 65000, 0)) == -1)
    perror ("fail");
	//printf ("%s", buftemp);
	pch = strtok (buftemp, "\r\n");
  
	while (pch != NULL) {
		i++;
		
		if (i == 1) {
			if (sscanf(pch, "%[^ ] %[^ ] %[^ ]", method, url, protocol) != 3) {
				printf("Bad request\n");
				send(s, err400, 3000, 0); // Send 400 Error and Close
				close(s);
			}
		} else if (i == 2) {
			if (sscanf(pch, "%[^ ] %[^ ]", method, hostact) != 2) {
				  printf("Bad hostname\n"); // Send 400 Error and Close
				  send(s, err400, 3000, 0); 
				  close(s);
			} else if (sscanf(pch, "%[^ ]: %[^ ]", method, field) != 2 && (i>2)) {
				printf("Field incorrect\n"); // Send 400 Error and Close
				send(s, err400, 3000, 0);
				close(s);
			}
		}

      pch = strtok (NULL, "\r\n");
    }
	
	printf("Fetching URL %s\n",url);
	
	if(strncasecmp(url, "http://", 7) == 0) {
		
		if (sscanf(url, "http://%[^:/]:%d%s", hostact, &port, path) == 3) {
			//printf("URL includes port\n");
		} else if(sscanf(url, "http://%[^/]%s", hostact, path) == 2) {
			//printf("URL does not include port\n");
			port = 80;
		} else if(sscanf(url, "http://%[^/]", hostact) == 1) {
			port = 80;
			*path = '\0';
		} else {
			printf("URL format incorrect, please retry");
		}
		
	} else {
		printf("Browser issuing direct URL requests\n");
		strcpy(path, url);
    }
	
	bufquery = build_http_request (hostact, path);
	//printf("%s\n", bufquery);


	// Sockets start here
	char myport[20];
	sprintf(myport, "%d", port);
	printf("Connecting to host %s on port %s\n", hostact, myport);

	if((rv = getaddrinfo(hostact, myport, &hintsf, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo %s ", gai_strerror(rv));
		printf("for host %s", host);
		exit(1);
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((sockf = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("Error getting client socket");
			continue;
		}

		if(connect(sockf, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockf);
			perror("Error on client connect");
			continue;
		}

		break;
	}

	if (p == NULL){
      fprintf(stderr, "Error client unable to connect\n");
    }

	inet_ntop(p->ai_family, get_in_addr ((struct sockaddr *) p->ai_addr), sf, sizeof sf);
	printf("Client connecting to %s\n", sf);

	// Socket send
	int tmpres;
	int sent = 0;
	
	while (sent < strlen (bufquery)) {
		tmpres = send(sockf, bufquery + sent, strlen (bufquery) - sent, 0);
		if(tmpres == -1) {
			perror("Can't send query");
			exit(1);
		}
		sent += tmpres;
	}


	// Read/receive the page
	memset(buf2, 0, sizeof (buf2));
  
	int j = 0;
  
	printf("Read path %s\n-----------------\n", path);
	while((tmpres = recv(sockf, buf2, BUFFER_SIZE, 0)) > 0) {

      j = j + send (s, buf2, tmpres, 0);
      memset (buf2, 0, sizeof (buf2));
    }


	if(tmpres < 0) {
		perror ("Error receiving data");
	}

	close(sockf);

	//printf("\n%d bytes sent back to browser for path %s\n-------------\n", j, path);

	return 0;
}

// Build the HTTP GET headers
char *build_http_request(char *host, char *page) {

	char *query;
	char *getpage = page;
	char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	
	if(getpage[0] == '/') {
		getpage = getpage + 1;
		//fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
    }

	query = (char *) malloc (strlen(host) + strlen(getpage) + strlen(USERAGENT) + strlen(tpl) - 5);
	sprintf(query, tpl, getpage, host, USERAGENT);
	
	return query;
}
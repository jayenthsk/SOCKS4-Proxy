#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#define PORT 8080

int main(int argc, char* argv[])
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char request[255] = {0};

    // Constructing SOCKS request 
    // Packet format : ver:4 code(action:connect):1 port_no(application server) ip(application server) userid 0
    
	sprintf(request, "4 1 80 %s %s 0", argv[1], argv[2]);
	char buffer[10000] = { 0 };

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

    // Setting timeout as 2min
	struct timeval timeout;
    timeout.tv_sec=120;
    if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))<0)
            printf("\nError in setsockopt");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, argv[3], &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

    // Connect to SOCKS server

	if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

    // Sending initial request

	send(sock, request, strlen(request), 0);
	printf("request message sent\n");
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);
	char ver[22], stat[22], ip[22], port[22];
	sscanf(buffer, "%s %s %s %s", ver, stat, port, ip);

    // Receiving Packet format : 0 Status code{ 90 | 91 | 92 | 93} IP PORT

    // Performing actions according to status codes

	if( !strcmp(stat, "90") ){

        // Request accepted hence tunneling

		char req[225] = {};
		sprintf(req, "GET %s HTTP/1.1\r\nHost: 172.17.0.3\r\nUser-Agent: curl/7.81.0\r\nAccept: text/html\r\n\r\n", argv[4]); 
		send(sock, req, strlen(req), 0);
		printf("req message sent\n");
		valread = read(sock, buffer, 10000);
		printf("Response: \n%s \n", buffer);

	}

	else if( !strcmp(stat, "91") ){

        // Request rejected or failed Status code 91

		printf("Request rejected or failed!");
	}
	
	else if( !strcmp(stat, "92") ){

        // SOCKS server cannot connect to ident server Status code 92

		printf("SOCKS server cannot connect to ident server");
	}

	else if( !strcmp(stat, "93") ){

        // Userid mismatch! Status code 93

		printf("Userid mismatch!");
	}

	// closing the connected socket
	close(client_fd);
	return 0;
}

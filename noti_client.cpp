#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int client_fd, valread;
char* hello = "func";
char buffer[1024] = { 0 };

void connect_to_sever(int PORT){
	int status;
	struct sockaddr_in serv_addr;

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.16.19.255", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
	}

	if ((status
		= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
	}
    }

    
int main(int argc, char const* argv[])
{   
    connect_to_sever(8082);
    send(client_fd, hello, strlen(hello), 0);
	printf("Waiting ... \n");
    valread = read(client_fd, buffer, 1024);
    printf ("%s", buffer);
 

	// closing the connected socket
    close (client_fd);
	return 0;
}

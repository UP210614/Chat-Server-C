#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>

int bufferSize(char b[]);

int main (int argc, char *argv[]){
    
    if(argc < 2){
	printf("Falta la direccion ip.\n");
	return 0;
    }

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sockfd = 0;
    const char * port = "9999";		


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int s = getaddrinfo(argv[1], port, &hints, &result);
    if(s != 0){
	printf("Error en la funcion getaddrinfo.\n");
	return 0;
    } 


    for (rp = result; rp != NULL; rp = rp->ai_next) {
               sockfd = socket(rp->ai_family, rp->ai_socktype,
                            rp->ai_protocol);
               if (sockfd == -1)
                   continue;

               if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
                   break;                  /* Success */
    }

    /*
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Error al crear el socket\n");
        close(sockfd);
        return 0;
    }
    printf("El socket se creó correctamente\n");

    struct sockaddr_in address = {
        AF_INET,
        htons(9999),
        0
    };

    connect(sockfd, &address, sizeof(address));
    

    */

    struct pollfd fds[2] = {
        {
            0,
            POLLIN,
            0,
        },
        {
            sockfd,
            POLLIN,
            0
        }
    };

    
    for( ; ; ){
        poll(fds,2,5000);

        char buffer[256] = { 0 };
        
        if(fds[0].revents & POLLIN){
           int message = read(0, buffer, 255);
           if(message == -1 ){
                printf("Error al leer\n");
                close(sockfd);
                return 1;
           }
           int sendMessage = send(sockfd, buffer, bufferSize(buffer)-1, 0);
           if(sendMessage == -1){
                printf("Error al envíar\n");
                close(sockfd);
                return 1;
            }
        }else if(fds[1].revents & POLLIN){
            if(recv(sockfd, buffer, 255, 0) == 0){
                printf("Error al recibir desde el cliente\n");
                close(sockfd);
                return 1;
            }
            printf("%s\n",buffer);
        }
    }
    
    close(sockfd);
    return 0;
}


int bufferSize(char buffer[]){

        int c = 0;

        for(c ; buffer[c] != '\0'; c++){
        }

        return c;

}

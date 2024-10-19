#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

//Servidor




int bufferSize(char b[]);

int main (){
    //Creación del socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    if(bind(sockfd, &address, sizeof(address)) == -1){
        printf("Error al enlazar el socket\n");
        close(sockfd);
        return 0;
    }

    printf("El socket se enlazo correctamente\n");


    if(listen(sockfd,10) == -1){
        printf("Error al poner en escucha el socket\n");
        close(sockfd);
        return 0;
    }

    printf("El socket esta en escucha\n");
    
    int clientfd = accept(sockfd, 0, 0);
    if(clientfd == -1){
        printf("Error al aceptar el cliente\n");
        close(sockfd);
	close(clientfd);
        return 0;
    }

    printf("Conexion con cliente aceptada\n");


    struct pollfd fds[2] = {
        {
            0,
            POLLIN,
            0,
        },
        {
            clientfd,
            POLLIN,
            0
        }
    };

    
    for( ; ; ){
        poll(fds,2,50000);
        char buffer[256] = { 0 };
        if(fds[0].revents & POLLIN){
           int message = read(0, buffer, 255);
           if(message == -1 ){
                printf("Error al leer\n");
                close(sockfd);
		close(clientfd);
                return 1;
           }
           int sendMessage = send(clientfd, buffer, bufferSize(buffer)-1, 0);
           if(sendMessage == -1){
                printf("Error al envíar\n");
                close(sockfd);
		close(clientfd);
                return 1;
            }
        }else if(fds[1].revents & POLLIN){
            if(recv(clientfd, buffer, 255, 0) == 0){
                printf("Error al recibir desde el cliente\n");
                close(sockfd);
	        close(clientfd);
                return 1;
            }
            printf("%s\n",buffer);
        }
    }

    close(clientfd);
    close(sockfd);
    return 0;
}


int bufferSize(char buffer[]){

	int c = 0;

	for(c ; buffer[c] != '\0'; c++){
	}

	return c;

}



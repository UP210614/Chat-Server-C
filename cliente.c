#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
int main (){

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

    connect(sockfd, &address, sizeof(address));
    


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

    poll(fds,2,5000);

    char buffer[256] = { 0 };
    for( ; ; ){
        if(fds[0].revents & POLLIN){
            if(read(0, buffer, 255) == -1){
                printf("Error al leer\n");
                close(sockfd);
                return 1;
            }
            if(send(sockfd, buffer, 255, 0) == -1){
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
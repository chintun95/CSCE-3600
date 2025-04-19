// Chin Lun Wong


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define LOSS_RATE 30  

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];


    srand(time(NULL));

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;          
    server_addr.sin_addr.s_addr = INADDR_ANY;  
    server_addr.sin_port = htons(port);        

    // Bind socket to the specified port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[server]: ready to accept data...\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive PING message
        if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                     (struct sockaddr *)&client_addr, &client_len) < 0) {
            perror("recvfrom error");
            continue;
        }

        // Simulate packet loss
        int chance = rand() % 100;
        if (chance < LOSS_RATE) {
            printf("[server]: dropped packet\n");
            continue;
        }

        printf("[client]: %s\n", buffer);

        // Send PONG reply
        const char *pong = "PONG";
        sendto(sockfd, pong, strlen(pong), 0,
               (struct sockaddr *)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}

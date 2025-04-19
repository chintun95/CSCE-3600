//Chin Lun Wong

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PING_COUNT 10
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 1

double get_time_diff(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_usec - start.tv_usec) / 1000.0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <hostname> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *hostname = argv[1];
    int port = atoi(argv[2]);
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    int received = 0;
    double min_rtt = 1e9, max_rtt = 0, total_rtt = 0;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket timeout
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // Resolve hostname
    if ((server = gethostbyname(hostname)) == NULL) {
        fprintf(stderr, "Error resolving hostname\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr, server->h_addr, server->h_length);

    for (int i = 1; i <= PING_COUNT; ++i) {
        struct timeval start, end;

        const char *ping = "PING";
        gettimeofday(&start, NULL);

        // Send message
        sendto(sockfd, ping, strlen(ping), 0,
               (struct sockaddr *)&server_addr, addr_len);

        // Wait for response
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                         (struct sockaddr *)&server_addr, &addr_len);

        gettimeofday(&end, NULL);

        printf("%d: Sent... ", i);
        if (n < 0) {
            printf("Timed Out\n");
        } else {
            buffer[n] = '\0';
            double rtt = get_time_diff(start, end);
            printf("RTT=%.6f ms\n", rtt);
            received++;
            total_rtt += rtt;
            if (rtt < min_rtt) min_rtt = rtt;
            if (rtt > max_rtt) max_rtt = rtt;
        }
    }

    int lost = PING_COUNT - received;
    double loss_rate = (lost / (double)PING_COUNT) * 100.0;
    double avg_rtt = received > 0 ? total_rtt / received : 0;

    printf("%d pkts xmited, %d pkts rcvd, %.0f%% pkt loss\n",
           PING_COUNT, received, loss_rate);
    printf("min: %.6f ms, max: %.6f ms, avg: %.6f ms\n",
           min_rtt, max_rtt, avg_rtt);

    close(sockfd);
    return 0;
}

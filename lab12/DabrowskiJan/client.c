#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define BUFFER_SIZE 1024

int client_socket;
struct sockaddr_in server_addr;
char client_id[50];

void handle_signal(int signal) {
    sendto(client_socket, "STOP", 4, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    close(client_socket);
    exit(0);
}

void send_message(char *message) {
    sendto(client_socket, message, strlen(message), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
}

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int n = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, NULL, NULL);
        buffer[n] = '\0';
        printf("%s\n", buffer);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <id> <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    signal(SIGINT, handle_signal);

    strcpy(client_id, argv[1]);
    char *server_ip = argv[2];
    int server_port = atoi(argv[3]);
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    snprintf(buffer, BUFFER_SIZE, "REGISTER %s", client_id);
    send_message(buffer);

    pthread_t receive_thread;
    pthread_create(&receive_thread, NULL, receive_messages, NULL);

    while (1) {
        printf("Enter command: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (strncmp(buffer, "LIST", 4) == 0) {
            send_message(buffer);
        } else if (strncmp(buffer, "2ALL ", 5) == 0) {
            send_message(buffer);
        } else if (strncmp(buffer, "2ONE ", 5) == 0) {
            send_message(buffer);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            send_message(buffer);
            break;
        }
    }

    close(client_socket);
    return 0;
}

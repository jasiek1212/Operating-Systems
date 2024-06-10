#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>


#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    char id[50];
    struct sockaddr_in addr;
    int active;
} Client;

Client clients[MAX_CLIENTS];
int server_socket;

void handle_signal(int signal) {
    close(server_socket);
    exit(0);
}

void remove_client(int index) {
    clients[index].active = 0;
}

void send_to_all(char *message, struct sockaddr_in *exclude_addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && (exclude_addr == NULL || memcmp(&clients[i].addr, exclude_addr, sizeof(struct sockaddr_in)) != 0)) {
            sendto(server_socket, message, strlen(message), 0, (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr));
        }
    }
}

void send_to_one(char *message, char *client_id) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].id, client_id) == 0) {
            sendto(server_socket, message, strlen(message), 0, (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr));
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    signal(SIGINT, handle_signal);

    int port = atoi(argv[1]);
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    while (1) {
        int n = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        buffer[n] = '\0';

        if (strncmp(buffer, "REGISTER ", 9) == 0) {
            char *client_id = buffer + 9;
            int found = 0;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].active && strcmp(clients[i].id, client_id) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (!clients[i].active) {
                        strcpy(clients[i].id, client_id);
                        clients[i].addr = client_addr;
                        clients[i].active = 1;
                        break;
                    }
                }
            }
        } else if (strncmp(buffer, "LIST", 4) == 0) {
            char list[BUFFER_SIZE] = "Active clients:\n";
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].active) {
                    strcat(list, clients[i].id);
                    strcat(list, "\n");
                }
            }
            sendto(server_socket, list, strlen(list), 0, (struct sockaddr *)&client_addr, client_addr_len);
        } else if (strncmp(buffer, "2ALL ", 5) == 0) {
            char *message = buffer + 5;
            char full_message[BUFFER_SIZE];
            time_t now = time(NULL);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].active) {
                    snprintf(full_message, BUFFER_SIZE, "[%s] %s: %s", strtok(ctime(&now), "\n"), clients[i].id, message);
                    send_to_all(full_message, &client_addr);
                }
            }
        } else if (strncmp(buffer, "2ONE ", 5) == 0) {
            char *client_id = strtok(buffer + 5, " ");
            char *message = strtok(NULL, "");
            if (client_id && message) {
                char full_message[BUFFER_SIZE];
                time_t now = time(NULL);
                snprintf(full_message, BUFFER_SIZE, "[%s] %s: %s", strtok(ctime(&now), "\n"), client_id, message);
                send_to_one(full_message, client_id);
            }
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].active && memcmp(&clients[i].addr, &client_addr, sizeof(struct sockaddr_in)) == 0) {
                    remove_client(i);
                    break;
                }
            }
        }
    }

    close(server_socket);
    return 0;
}

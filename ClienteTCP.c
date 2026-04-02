#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusões condicionais para Windows e Linux/Unix
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") // Linka a biblioteca Winsock
    #define close_socket closesocket
    #define GET_LAST_ERROR WSAGetLastError
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define close_socket close
    #define GET_LAST_ERROR errno
    #include <errno.h>
#endif

#define BUFFER_SIZE 1024

/**
 * Cliente TCP Simples em C (Multiplataforma)
 * 
 * Este programa demonstra como criar um socket, configurar o endereço de destino,
 * conectar-se a um servidor e trocar mensagens básicas, compatível com Windows e Linux.
 */

int main(int argc, char *argv[]) {
    // Tipo de descritor de socket varia entre sistemas
#ifdef _WIN32
    SOCKET sockfd;
    WSADATA wsaData;
#else
    int sockfd;
#endif

    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char *server_ip;
    int server_port;

    // Verifica se os argumentos foram fornecidos (IP e Porta)
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <IP_DO_SERVIDOR> <PORTA>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_ip = argv[1];
    server_port = atoi(argv[2]);

#ifdef _WIN32
    // Inicializa Winsock no Windows
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "[-] WSAStartup falhou: %d\n", GET_LAST_ERROR());
        exit(EXIT_FAILURE);
    }
#endif

    // 1. Criar o socket (AF_INET = IPv4, SOCK_STREAM = TCP, 0 = Protocolo padrão)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
#ifdef _WIN32
    if (sockfd == INVALID_SOCKET) {
        fprintf(stderr, "[-] Erro ao criar o socket: %d\n", GET_LAST_ERROR());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
#else
    if (sockfd < 0) {
        perror("[-] Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }
#endif
    printf("[+] Socket criado com sucesso.\n");

    // 2. Configurar a estrutura de endereço do servidor (sockaddr_in)
    memset(&server_addr, 0, sizeof(server_addr)); // Limpa a estrutura
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port); // Converte a porta para Network Byte Order

    // Converte o IP de string para formato binário de rede
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
#ifdef _WIN32
        fprintf(stderr, "[-] Endereço IP inválido ou não suportado: %d\n", GET_LAST_ERROR());
        close_socket(sockfd);
        WSACleanup();
#else
        perror("[-] Endereço IP inválido ou não suportado");
        close_socket(sockfd);
#endif
        exit(EXIT_FAILURE);
    }

    // 3. Conectar ao servidor
    printf("[*] Conectando a %s:%d...\n", server_ip, server_port);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
#ifdef _WIN32
        fprintf(stderr, "[-] Falha na conexão: %d\n", GET_LAST_ERROR());
        close_socket(sockfd);
        WSACleanup();
#else
        perror("[-] Falha na conexão");
        close_socket(sockfd);
#endif
        exit(EXIT_FAILURE);
    }
    printf("[+] Conectado ao servidor!\n");

    // 4. Interação: Enviar e receber dados
    while (1) {
        printf("Digite a mensagem (ou 'sair' para encerrar): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove o caractere de nova linha (\n)

        if (strcmp(buffer, "sair") == 0) {
            break;
        }

        // Enviar dados para o servidor
        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
#ifdef _WIN32
            fprintf(stderr, "[-] Erro ao enviar dados: %d\n", GET_LAST_ERROR());
#else
            perror("[-] Erro ao enviar dados");
#endif
            break;
        }

        // Receber resposta do servidor
        memset(buffer, 0, BUFFER_SIZE);
        // Em Windows, recv retorna int, em Linux/Unix retorna ssize_t
        int bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
#ifdef _WIN32
            fprintf(stderr, "[-] Erro ao receber dados: %d\n", GET_LAST_ERROR());
#else
            perror("[-] Erro ao receber dados");
#endif
            break;
        } else if (bytes_received == 0) {
            printf("[!] O servidor encerrou a conexão.\n");
            break;
        }
        buffer[bytes_received] = '\0'; // Garante terminação nula

        printf("[Servidor]: %s\n", buffer);
    }

    // 5. Fechar o socket
    printf("[*] Encerrando a conexão...\n");
    close_socket(sockfd);
#ifdef _WIN32
    WSACleanup(); // Finaliza Winsock no Windows
#endif
    printf("[+] Conexão encerrada.\n");

    return 0;
}

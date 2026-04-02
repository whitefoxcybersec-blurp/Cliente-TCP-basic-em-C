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
 * Cliente UDP Simples em C (Multiplataforma)
 * 
 * Este programa demonstra como criar um socket UDP, configurar o endereço de destino,
 * enviar e receber datagramas, compatível com Windows e Linux.
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

    // 1. Criar o socket (AF_INET = IPv4, SOCK_DGRAM = UDP, 0 = Protocolo padrão)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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
    printf("[+] Socket UDP criado com sucesso.\n");

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

    // 3. Interação: Enviar e receber dados
    while (1) {
        printf("Digite a mensagem (ou 'sair' para encerrar): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove o caractere de nova linha (\n)

        if (strcmp(buffer, "sair") == 0) {
            break;
        }

        // Enviar dados para o servidor
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
#ifdef _WIN32
            fprintf(stderr, "[-] Erro ao enviar dados: %d\n", GET_LAST_ERROR());
#else
            perror("[-] Erro ao enviar dados");
#endif
            break;
        }

        // Receber resposta do servidor (opcional, UDP não garante resposta)
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t server_addr_len = sizeof(server_addr);
        // Em Windows, recvfrom retorna int, em Linux/Unix retorna ssize_t
        int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&server_addr, &server_addr_len);
        if (bytes_received < 0) {
#ifdef _WIN32
            fprintf(stderr, "[-] Erro ao receber dados: %d\n", GET_LAST_ERROR());
#else
            perror("[-] Erro ao receber dados");
#endif
            // Não é um erro fatal para UDP, pode ser apenas que o servidor não respondeu
            printf("[!] Nenhuma resposta recebida ou erro de rede.\n");
            continue; // Continua tentando enviar
        } else if (bytes_received == 0) {
            // Em UDP, 0 bytes recebidos geralmente significa que o remetente enviou um pacote vazio
            printf("[!] Pacote vazio recebido.\n");
            continue;
        }
        buffer[bytes_received] = '\0'; // Garante terminação nula

        printf("[Servidor]: %s\n", buffer);
    }

    // 4. Fechar o socket
    printf("[*] Encerrando a conexão...\n");
    close_socket(sockfd);
#ifdef _WIN32
    WSACleanup(); // Finaliza Winsock no Windows
#endif
    printf("[+] Conexão encerrada.\n");

    return 0;
}

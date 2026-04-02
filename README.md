# Clientes TCP e UDP em C (Multiplataforma)

Este repositório contém implementações de clientes TCP e UDP em C, projetados para funcionar tanto em sistemas **Linux/Unix** quanto em **Windows**. O objetivo é explorar os fundamentos da comunicação de rede de baixo nível usando sockets, com foco na portabilidade e no tratamento de erros.

## ⚠️ Aviso de Segurança e Uso Ético

Este código é para **fins educacionais** e para entender os fundamentos da programação de rede. Não deve ser usado em ambientes de produção sem auditoria de segurança rigorosa. Sempre obtenha permissão explícita antes de testar qualquer sistema.

## Pré-requisitos

Para compilar e executar este código, você precisará de um compilador C:

*   **Linux/macOS**: `GCC` (geralmente já vem pré-instalado ou pode ser instalado via gerenciador de pacotes).
*   **Windows**: `MinGW-w64` (recomendado para integração com VS Code) ou `Visual Studio Build Tools`.

## Configuração do Ambiente (Windows)

Se você está no Windows e usando o VS Code, siga este guia para configurar o `MinGW-w64` e o IntelliSense:

1.  **Instalar MinGW-w64 via MSYS2**: Siga o guia `msys2_gcc_install_guide.md` para baixar o MSYS2 e instalar o `mingw-w64-ucrt-x86_64-toolchain`.
2.  **Adicionar ao PATH**: Certifique-se de que o caminho `C:\msys64\ucrt64\bin` (ou o diretório `bin` da sua instalação MinGW) está adicionado às variáveis de ambiente do sistema.
3.  **Configurar IntelliSense no VS Code**: Se as linhas vermelhas persistirem, siga o guia `vscode_manual_intellisense_fix.md` para configurar manualmente o `c_cpp_properties.json`.

## Compilação

### Linux/macOS

Abra um terminal na pasta do projeto e use o `gcc`:

*   **Cliente TCP**:
    ```bash
gcc -o cliente_tcp tcp_client_cross_platform.c
    ```
*   **Cliente UDP**:
    ```bash
gcc -o cliente_udp udp_client_cross_platform.c
    ```

### Windows (com MinGW-w64)

Abra o terminal (PowerShell ou Prompt de Comando) na pasta do projeto e use o `gcc`, **incluindo a biblioteca Winsock**:

*   **Cliente TCP**:
    ```bash
gcc -o cliente_tcp.exe tcp_client_cross_platform.c -lws2_32
    ```
*   **Cliente UDP**:
    ```bash
gcc -o cliente_udp.exe udp_client_cross_platform.c -lws2_32
    ```

## Uso

### Cliente TCP (`cliente_tcp` ou `cliente_tcp.exe`)

Este cliente se conecta a um servidor TCP, envia mensagens e recebe respostas.

```bash
./cliente_tcp <IP_DO_SERVIDOR> <PORTA>
# Exemplo:
./cliente_tcp 127.0.0.1 8080
```

### Cliente UDP (`cliente_udp` ou `cliente_udp.exe`)

Este cliente envia datagramas para um servidor UDP e pode receber respostas (sem garantia de entrega).

```bash
./cliente_udp <IP_DO_SERVIDOR> <PORTA>
# Exemplo:
./cliente_udp 127.0.0.1 8081
```

## Testando os Clientes

Para testar, você precisará de um servidor TCP ou UDP. Você pode usar o `netcat` (nc) para criar um servidor simples:

*   **Servidor TCP (em outro terminal)**:
    ```bash
nc -l -p 8080
    ```
*   **Servidor UDP (em outro terminal)**:
    ```bash
nc -u -l -p 8081
    ```

## Diferenças Chave: TCP vs. UDP

| Característica         | TCP (cliente_tcp)                                   | UDP (cliente_udp)                                     |
| :--------------------- | :-------------------------------------------------- | :---------------------------------------------------- |
| **Conexão**            | Orientado à conexão (usa `connect()`)               | Sem conexão (usa `sendto()` e `recvfrom()`)           |
| **Confiabilidade**     | Garante entrega, ordem e integridade dos dados.     | Não garante entrega, ordem ou integridade.            |
| **Velocidade**         | Mais lento (devido a mecanismos de controle).       | Mais rápido (devido à simplicidade).                  |
| **Uso Típico**         | Transferência de arquivos, web, SSH.                | Streaming, jogos online, DNS.                         |

## Arquivos do Projeto

*   `.gitignore`: Configuração para o Git ignorar arquivos compilados e sensíveis.
*   `README.md`: Este arquivo.
*   `tcp_client_cross_platform.c`: Código fonte do cliente TCP.
*   `udp_client_cross_platform.c`: Código fonte do cliente UDP.

**Documentação Detalhada (para referência):**
*   `tcp_client_c_explanation.md`: Explicação do cliente TCP em C.
*   `udp_client_c_explanation.md`: Explicação do cliente UDP em C.
*   `socket_api_differences.md`: Diferenças entre APIs de socket no Windows e Linux.
*   `msys2_gcc_install_guide.md`: Guia de instalação do MinGW-w64 via MSYS2.
*   `vscode_manual_intellisense_fix.md`: Guia para configuração manual do IntelliSense no VS Code.

## Referências

[1] Beej's Guide to Network Programming. Disponível em: <https://beej.us/guide/bgnet/>.
[2] Microsoft Learn. *Winsock (Windows Sockets)*. Disponível em: <https://learn.microsoft.com/en-us/windows/win32/winsock/winsock-programming-reference-2>.

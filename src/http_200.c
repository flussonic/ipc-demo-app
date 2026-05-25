#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_PORT 8080
#define BACKLOG 16

static const char response[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Connection: close\r\n"
    "Content-Length: 3\r\n"
    "\r\n"
    "OK\n";

static int parse_port(int argc, char **argv)
{
    char *end = NULL;
    long port;

    if (argc < 2) {
        return DEFAULT_PORT;
    }

    errno = 0;
    port = strtol(argv[1], &end, 10);
    if (errno != 0 || end == argv[1] || *end != '\0' || port <= 0 || port > 65535) {
        fprintf(stderr, "usage: %s [port]\n", argv[0]);
        exit(2);
    }

    return (int)port;
}

int main(int argc, char **argv)
{
    int port = parse_port(argc, argv);
    int server_fd;
    int opt = 1;
    struct sockaddr_in addr;

    signal(SIGPIPE, SIG_IGN);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("listening on 0.0.0.0:%d\n", port);

    for (;;) {
        int client_fd = accept(server_fd, NULL, NULL);

        if (client_fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("accept");
            close(server_fd);
            return 1;
        }

        (void)write(client_fd, response, sizeof(response) - 1);
        close(client_fd);
    }
}

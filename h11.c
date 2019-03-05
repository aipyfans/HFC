#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <zconf.h>
#include <sys/errno.h>

#include "common.h"
#include "h11.h"

/**
 *  参见 P453
 *
 * @param sig
 * @param handler
 * @return
 */
int catch_signal(int sig, void (*handler)(int)) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}


int open_listener_socket() {
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1)
        error("Can't open socket");
    return s;
}


void bind_to_port(int socket, int port) {
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t) htons(port);// 30000
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    int reuse = 1; // 重新使用端口
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int)) == -1)
        error("Can't set the reuse option on the socket");
    if (bind(socket, (struct sockaddr *) &name, sizeof(name)) == -1)
        error("Can't bind to socket");
}


ssize_t say(int socket, char *s) {
    ssize_t result = send(socket, s, strlen(s), 0);
    if (result == -1)
        fprintf(stderr, "%s: %s\n", "通信时发生了错误", strerror(errno));
    return result;
}


ssize_t read_in(int socket, char *buf, int len) {
    char *s = buf;
    int slen = len;

    ssize_t c = recv(socket, s, slen, 0);
    while ((c > 0) && (s[c - 1] != '\n')) {
        s += c;
        slen -= c;
        c = recv(socket, s, slen, 0);
    }
    if (c < 0)// 防止错误
        return c;
    else if (c == 0) // 什么都没读到，返回一个空字符串
        buf[0] = '\0';
    else // 用 \0 替换 \r
        s[c - 1] = '\0';
    return len - slen;
}

int listener_d;

void handle_shutdown(int sig) {
    if (listener_d)
        close(listener_d);
    fprintf(stderr, "Bye!\n");
    exit(0);
}

int open_socket(char *host, char *port) {
    struct addrinfo *res;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port, &hints, &res) == -1)
        error("Can't resolve the address");
    int d_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (d_sock == -1)
        error("Can't open socket");
    int c = connect(d_sock, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    if (c == -1)
        error("Can't connect to socket");
    return d_sock;
}


void start_server() {
    if (catch_signal(SIGINT, handle_shutdown) == -1)
        error("Can't set the interrupt handler");
    listener_d = open_listener_socket();
    bind_to_port(listener_d, 30000);
    if (listen(listener_d, 10) == -1)
        error("Can't listen....");
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    puts("Waiting for connection");
    char buf[255];
    while (1) { // 开始监听
        int connect_d = accept(listener_d, (struct sockadr *) &client_addr, &address_size);
        if (connect_d == -1)
            error("Can't open secondary socket");
        if (!fork()) {
            close(listener_d);

            if (say(connect_d, "Internet Knock-Knock Protocol Server \r\nVersion 1.0\r\nKnock! Knock!\r\n>") != -1) {
                read_in(connect_d, buf, sizeof(buf));
                if (strncasecmp("Who's there?", buf, 12))
                    say(connect_d, "You should say 'Who's there?'!");
                else {
                    if (say(connect_d, "Oscar\r\n>") != -1) {
                        read_in(connect_d, buf, sizeof(buf));

                        if (strncasecmp("Oscar who?", buf, 10))
                            say(connect_d, "You should say 'Oscar who?'!\r\n");
                        else
                            say(connect_d, "Oscar silly question, you get a silly answer\r\n");
                    }
                }
            }
            close(connect_d);
            exit(0);
        }
        close(connect_d);
    }
}


void start_client(char *path) {
    int d_sock = open_socket("en.wikipedia.org", "80");
    char buf[255];

    sprintf(buf, "GET /wiki/%s http/1.1\r\n", path);
    say(d_sock, buf);
    say(d_sock, "Host: en.wikipedia.org\r\n\r\n");

    char rec[256];
    ssize_t byresRcvd = recv(d_sock, rec, 255, 0);
    while (byresRcvd) {
        if (byresRcvd == -1)
            error("Can't read from server");
        rec[byresRcvd] = "\0";
        printf("%s", rec);
        byresRcvd = recv(d_sock, rec, 255, 0);
    }
    close(d_sock);
}



//telnet en.wikipedia.org 80
//GET /wiki/O'Reilly_Media http/1.1
//Host: en.wikipedia.org
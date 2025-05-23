#include<iostream>
#include <unistd.h> 
#include <sys/socket.h> // for socket, bind, listen, accept
#include <netinet/in.h> // for sockaddr_in
#include <cstring> 


void die(const char *msg) {
    perror(msg);              // Print message + system error
    exit(EXIT_FAILURE);       // Exit with status 1
}

static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = recv(connfd, rbuf, sizeof(rbuf) - 1, 0);
    if (n < 0) {
        printf("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
   send(connfd, wbuf, strlen(wbuf), 0);
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }
    
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    struct sockaddr_in addr = {}; //sockaddr_in is a struct from <netinet/in.h>
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);        // port
    addr.sin_addr.s_addr = htonl(0);    // wildcard IP 0.0.0.0
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr)); // cast to sockaddr, changing type
    // bind() associates the socket with the address and port
    if (rv) { die("bind()"); }
    rv = listen(fd, SOMAXCONN);
    if (rv) { die("listen()"); }
      while (true) {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0) {
            continue;   // error
        }

        do_something(connfd);
        close(connfd);
    }

    return 0;
}
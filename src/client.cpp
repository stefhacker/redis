#include<iostream>
#include <unistd.h> 
#include <sys/socket.h> // for socket, bind, listen, accept
#include <netinet/in.h> // for sockaddr_in
#include <cstring> 


void die(const char *msg) {
    perror(msg);              // Print message + system error
    exit(EXIT_FAILURE);       // Exit with status 1
}

int main (){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
    die("connect");
    }
       struct sockaddr_in local_addr = {};
    socklen_t len = sizeof(local_addr);
    
    if (getsockname(fd, (struct sockaddr *)&local_addr, &len) < 0) {
        die("getsockname");
    }

    uint16_t local_port = ntohs(local_addr.sin_port);
    std::cout << "Client is using local port: " << local_port << std::endl;

    char msg[] = "hello";
    send(fd, msg, strlen(msg), 0);

    char rbuf[64] = {};
    ssize_t n = recv(fd, rbuf, sizeof(rbuf) - 1, 0);
    if (n < 0) {
        die("read");
    }
    printf("server says: %s\n", rbuf);
   
    close(fd);
}

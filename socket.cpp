#include<iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> 

void die(const char *msg) {
    perror(msg);              // Print message + system error
    exit(EXIT_FAILURE);       // Exit with status 1
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
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr)); // cast to sockaddr, chaniging type
    // bind() associates the socket with the address and port
    if (rv) { die("bind()"); }
    return 0;
}
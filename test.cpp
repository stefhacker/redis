#include<iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <cstring>

int main() {
    struct person{
        int age;
        char name[20];
    };
    struct person stefan = {};
    stefan.age = 25;
    strcpy(stefan.name, "Stefan");
    printf("Age: %d, Name: %s\n", stefan.age, stefan.name);
    return 0;
}
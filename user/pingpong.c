#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
    int p[2];
    char buf[100];
    pipe(p);

    int pid = fork();
    if (pid == 0)
    {
        close(p[0]); 
        write(p[1], "ping", 4);
        printf("%d: received ping\n", getpid());
    }
    else if (pid > 0)
    {      
        wait(0); 
        close(p[1]); 
        read(p[0], buf, 4);
        printf("%d: received pong\n", getpid());
    }
    else
    {
        printf("Fork failed\n");
        exit(1);
    }
    exit(0);
}
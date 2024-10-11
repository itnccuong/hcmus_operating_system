// fork()
// wait()
// pipe()

#include "kernel/types.h"
#include "user/user.h"

void sieve(int fd) __attribute__((noreturn));

void sieve(int fd) {
  int p, n;

  // read will return the number of character have been written, so if it not equal to sizeof(int), it has failed
  if(read(fd, &p, sizeof(int)) != sizeof(int)) {
    close(fd);
    exit(0);
  }

  printf("prime %d\n", p);

  int fds[2];
  pipe(fds);

  int pid = fork();

  if(pid < 0) {
    printf("Can not fork. Something went wrong\n");
    exit(1);
  }

  if(pid > 0) {
    // if we are parents, we won't need the pipe's read end, so we close it
    close(fds[0]);

    while(1) {
      // like above, we will check if we fetch any number anymore, if it have ended, we break the loop
      if(read(fd, &n, sizeof(int)) != sizeof(int)) {
         break;
      }
      if(n%p!=0)
        write(fds[1], &n, sizeof(int)); // we write to the pipe's write end, for the next process to read it
    }
    
    close(fds[1]);  // we won't need to write anything anymore, so we close the write end
    close(fd); // after finish reading, we close the pipe's read end;

    wait(0);
    exit(0);
  } else {
    // the child don't need to care about the write end of the pipe, also the parent read end pipe
    close(fd);
    close(fds[1]);
    sieve(fds[0]);
    exit(0);
  }
}

int main(int argc, char** argv) {
  if(argc > 1) {
    printf("WARN: Redundant argruments\n");
  }

  int fds[2];
  pipe(fds);

  int pid = fork();

  if(pid < 0) {
    printf("Can not fork. Something went wrong\n");
    exit(1);
  }

  if(pid > 0) {
    close(fds[0]);
    
    for(int i = 2 ; i <= 280 ; ++i)
      write(fds[1], &i, sizeof(int));

    close(fds[1]);
    wait(0);
    exit(0);
  } else {
    // the child don't need to care about the write end of the pipe, also the parent read end pipe
    close(fds[1]);
    sieve(fds[0]);
    exit(0);
  }
}
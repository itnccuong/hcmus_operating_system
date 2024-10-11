#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: sleep <ticks>\n");
    exit(1);
  }

  sleep(atoi(argv[1]));
  
  printf("Sleep done!\n");

  exit(0);
}

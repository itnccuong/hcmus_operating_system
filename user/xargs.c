// exec()
// fork() 
// wait()
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
  if(argc <= 1){
		printf("usage: xargs <command> [argv...]\n");
		exit(1);
	}
  char buf[2048]; // 2KB: store the data of the command
  char* p = buf; // pointer to the buf
  int offset = 0; 
  char* v[MAXARG]; // store the arguments of the new command
  int arg_pivot;
  char ch; // store the character read from the buf

  for(arg_pivot = 1 ; arg_pivot < argc ; ++arg_pivot) {
    v[arg_pivot - 1] = argv[arg_pivot];
  }

  arg_pivot--; // arg_pivot is now the index of the last argument

  while(read(0, &ch, 1) > 0) {
    if(ch == ' ' || ch == '\t') { // handling seprerated agruments of an input
      buf[offset++] = '\0';
      v[arg_pivot++] = p;
      p = buf + offset;
    } else if(ch == '\n') { // handling seprerated input
      buf[offset++] = '\0';
      v[arg_pivot] = p;
      if(fork() == 0) {
        exec(v[0], v);
        exit(0);
      } else {
        wait(0);
      }
      p = buf;
      offset = 0;
      arg_pivot = argc - 1;
    } else buf[offset++] = ch;
  } 
  exit(0);
}

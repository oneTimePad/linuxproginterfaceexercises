#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>



/**
* bad use of setjmp/longjmp
**/


static jmp_buf env;
static volatile int someval = 2;


static void jmpback(void){
  volatile int val;
  switch((val=setjmp(env))){
    case 0:
      return;
      break;
    default:
      someval = 3;
      printf("hello! %d\n",val);
      break;
  }
    //fails to return
    return;
}

int
main(int argc,char* argv[]){
  jmpback();
  longjmp(env,1);
  printf("new val %d\n",someval);
  exit(EXIT_SUCCESS);

}

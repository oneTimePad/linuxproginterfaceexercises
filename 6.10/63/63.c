#include <stdlib.h>
#include <unistd.h>
#include "my_env.h"
#include "../../errors.h"

#define setenv my_setenv
#define unsetenv my_unsetenv



int
main(int argc, char* argv[]){



  if(setenv("MY","WOW_LOL",1)==-1)
    errnoExit("setenv()");


    if(setenv("LOL","YO",1)==-1)
      errnoExit("setenv()");

    char* env_var;
    if((env_var=getenv("LOL"))==NULL){
      fprintf(stderr, "%s\n", "FAIL");
      exit(EXIT_FAILURE);
    }

      printf("%s\n",env_var);
/*
    if(unsetenv("LOL")==-1)
      errnoExit("unsetenv()");
    if(unsetenv("MY") == -1)
      errnoExit("unsetenv()");
*/
    if(getenv("MY") == NULL)
      printf("GONE\n");

    if(unsetenv("=") ==-1)
      errnoExit("unsetenv()");

  exit(EXIT_SUCCESS);
}

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include "../errors.h"

#define PROC_DIR_NAME "/proc/"

#define ZERO_ASCII 0x30
#define NINE_ASCII 0x39

#define LEN_PROC  6
#define LEN_STATUS 6
#define MAX_DIGITS_64BIT 20

/**
* convert user name into equivalanet uid
* name: username
* returns: uid of user
**/
static inline uid_t getUidFromName(char* name){
    struct passwd* pwd;
    errno = 0;
    if((pwd = getpwnam(name)) == NULL)
      errnoExit("getpwname()");

    return (pwd==NULL) ? -1 : pwd->pw_uid;

}

/**
* determine if `string` is a `number`
* returns: status
**/
static inline _BOOL isNumber(char * string){
  for(;*string!='\0';string++)
    if(*string > NINE_ASCII || *string < ZERO_ASCII)
      return FALSE;
  return TRUE;
}

/**
* determine if `c` is a decimal `digit`
* returns: status
**/
static inline _BOOL isDigit(char c){
  return (c=='\0'|| c> NINE_ASCII || c< ZERO_ASCII)? FALSE: TRUE;
}

/**
* determine if process with status file `status_file` is owned by `uid`
* status_file: file to look through
* uid: uid to check format
* returns: statically allocated string containg process image name
**/
static char* isProcOwnedBy(char* status_file, uid_t uid){
  #define UID_STRING "Uid:"
  #define NAME_STRING "Name:"
  #define SPACE 0x20
  static char name[NAME_MAX+1];

  int fd;
  if((fd=open(status_file,O_RDONLY))==-1)
    return NULL;

  FILE* fd_struc;
  if((fd_struc =fdopen(fd,"r")) == NULL)
    return NULL;

  if(posix_fadvise(fd,0,0,POSIX_FADV_SEQUENTIAL)!=0) //advise kernel to readahead more
    return NULL;

  ssize_t bytes_in_line;
  size_t buf_size = 2048;
  char* line = NULL;
  _BOOL found_digit = FALSE;
  while((bytes_in_line=getline(&line,&buf_size,fd_struc)) != -1){ //look through file line by line

    if((strstr(line,UID_STRING))!=NULL){  //if line contains `Uid:` extract the uid of the process
      char uid_str[MAX_DIGITS_64BIT+1];
      uid_str[0]='\0';

      char* line_ptr = line;

      int uid_str_size = 0;
      for(;*line_ptr!='\0';line_ptr++){ //search for first digit string
        if(isDigit(*line_ptr)){
          strncat(uid_str,line_ptr,1);
          uid_str_size++;
          found_digit = TRUE;
        }
        else if(found_digit){
          uid_str[uid_str_size] = '\0';
          break;
        }
      }

      uid_t found_uid = atoll(uid_str);

      if((found_digit&&found_uid != uid) || !found_digit){ //is the uid found not equal to the one we want
        free(line);
        return NULL;
      }



    }
    else if(strstr(line,NAME_STRING)!=NULL){ //if a line with `Name:`, extract process image name
      char* line_ptr = line;
      line_ptr+=strlen(NAME_STRING);
      for(;*line_ptr==SPACE;line_ptr++);
      if(strlen(line_ptr)>NAME_MAX){
        free(line);
        return NULL;
      }
      strncpy(name,line_ptr,strlen(line_ptr));
      name[strlen(line_ptr)] = '\0';
    }


  }

  free(line);

  if(close(fd) == -1)
    return NULL;

  return (found_digit) ? name: NULL;



}

int
main(int argc, char* argv[]){

  DIR *dir;


  if((dir = opendir(PROC_DIR_NAME))==NULL)
    errnoExit("opendir()");

  struct dirent * proc_dir;

  while((proc_dir = readdir(dir)) != NULL){

    char* file_name = proc_dir->d_name;
    if(!isNumber(file_name))
      continue;

    size_t status_file_len = LEN_PROC + strlen(file_name)+1+LEN_STATUS+1;

    char status[status_file_len];

    snprintf(status,status_file_len,PROC_DIR_NAME "%s/status",file_name);


    char* name;
    errno = 0;
    if((name=isProcOwnedBy(status,atoll(argv[1])))==NULL){
      errnoExit("isProcOwnedBy()");
      continue;
    }
    printf("NAME: %s\n",name);
  }

  if(closedir(dir)==-1)
    errnoExit("closedir()");


  exit(EXIT_SUCCESS);
}

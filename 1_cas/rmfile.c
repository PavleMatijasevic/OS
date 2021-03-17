/*
 * Ovaj primer prikazuje brisanje fajlova odnosno direktorijuma.
 * */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *file, int line)
{
   if (!cond) {
     perror(msg);
     fprintf(stderr, "%s:%d\n", file, line);
     exit(EXIT_FAILURE);
   }
}

int main(int argc, char **argv) {
    osAssert(3 == argc, "Argument missing. Usage: ./rmfile -[fd] path/to/file/or/directory");
    
    osAssert('-' == argv[1][0], "Not a command line option");
    if ('f' == argv[1][1])
      osAssert(unlink(argv[2]) >= 0, "Removing file failed");
    else if ('d' == argv[1][1])
      osAssert(rmdir(argv[2]) >= 0, "Removing directory failed");
    else 
      osAssert(false, "Wrong option, valid are 'f' and 'd'");
    return 0;
}

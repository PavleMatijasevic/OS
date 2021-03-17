/*
 * Ovaj primer prikazuje kako se kreira direktorijum. Naravno finalna prava pristupa
 * će zavisiti od umask() maske postavljene na sistemu (nemojte da vas ovo zbunjuje
 * to je nešto što će biti obrađeno na nekom od sledećih časova).
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

bool osCreateDirectory(const char *dirPath);

int main(int argc, char **argv) {
    osAssert(2 <= argc, "Argument missing");
    osAssert(osCreateDirectory(argv[1]), "Create directory failed");
    return 0;
}

bool osCreateDirectory(const char *dirPath) {
  static mode_t mode = 0755;
  return mkdir(dirPath, mode) >= 0;
}

/*
 * Primer demonstrira upotrebu open() sistemskog poziva kroz naivnu implementaciju funkcije
 * koja bi trebala da liči na fopen(). Funkcija osFileOpen() prima putanju do datoteke koju
 * treba otvoriti/napraviti, mod pristupa isti kao fopen() i kao povratnu vrednost vraća bool 
 * true/false u zavisnosti od toga da li je izvršavanje bilo uspešno ili ne. Kroz pokazivački 
 * argument 'fd' vraća se vrednost fajl deskriptora.
 * Za fajl deskriptore možete smatrati da su pri radu sa sistemskim pozivima isto to što je 
 * struktura FILE* kada koristite funkcije C standardne biblioteke. Konceptualno, možete da 
 * zamislite da se vrednosti fajl deksriptora koriste kao indeksi velikog niza struktura u
 * kome kernel operativnog sistema čuva podatke o fajlovima.
 */

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

bool osFileOpen(const char *filePath, const char *mode, int *fd);

int main(int argc, char **argv) {
    osAssert(2 == argc, "Argument missing");
    
    int fd;
    osAssert(osFileOpen(argv[1], "w", &fd), "File create");
    close (fd);
    return 0;
}

bool osFileOpen(const char *filePath, const char *mode, int *fd) {
    /**
     * Set default priviledges to:
     * -rw-r--r--
     * NOTE: Actual priviledges will depend on umask() system value. 
     * We will look at umask() system call in one of the following 
     * classes.
     * */
    static mode_t defaultMode = 0644;
    
    /**
     * Construct flags
     * */
    int flags = 0;
    switch (mode[0]) {
    case 'r':
        flags |= '+' == mode[1] ? O_RDWR : O_RDONLY;
        break;
    case 'w':
        flags |= '+' == mode[1] ? O_RDWR : O_WRONLY;
        flags |= O_TRUNC;
        flags |= O_CREAT;
        break;
    case 'a':
        flags |= '+' == mode[1] ? O_RDWR : O_WRONLY;
        flags |= O_APPEND;
        flags |= O_CREAT;
        break;
    default:
        return false;
    }
    
    /**
     * Open file
     * */
    *fd = open(filePath, flags, defaultMode);
    return *fd >= 0;
}

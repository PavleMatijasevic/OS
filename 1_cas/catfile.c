/*
 * Ovaj primer prikazuje upotrebu sistemskih poziva read() i write() da se fajl
 * pročita i ispiše na standardni izlaz.
 * Pored toga demonstrira se upotreba celobrojnih tipova fiksne širine koju su
 * definisani u okviru zaglavlja stdint.h. Tipovi fiksne širine su zgodni pri
 * programiranju aplikacija čija je ispravnost kritična jer se njihovom upotrebom 
 * mogu lakše ispratiti razne aritmetičke greške pri graničnim slučajevima.
 * */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

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

bool osCopyToTerminal(const int fd);

int main(int argc, char **argv) {
    osAssert(2 == argc, "Argument missing");
    
    int fd = open(argv[1], O_RDONLY);
    osAssert(-1 != fd, "Failed to open file");
    osAssert(osCopyToTerminal(fd), "File write to console");
    close(fd);
    return 0;
}

bool osCopyToTerminal(int fd) {
    /**
     * Create memory buffer
     * */
    static const uint32_t memBufSize = 1U << 13; // 8KB
    char *memBuf = malloc(memBufSize);
    if (NULL == memBuf)
        return false;
    
    /**
     * Dump file to terminal
     * */
    int32_t readBytes;
    while ((readBytes = read(fd, memBuf, memBufSize)) > 0) {
        if(write(STDOUT_FILENO, memBuf, readBytes) < 0) {
            free(memBuf);
            return false;
        }
    }
    free(memBuf);
    return 0 == readBytes;
}

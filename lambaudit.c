#define _GNU_SOURCE

#include <string.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


/* Sample call logging POC */
typedef ssize_t (*real_read_t)(int, void *, size_t);

// Find the real function call address so that we can call it back
ssize_t real_read(int fd, void *data, size_t size) {
  return ((real_read_t)dlsym(RTLD_NEXT, "read"))(fd, data, size);
}

// Replace the call to the function and log this
// Note: do NOT use this in production :)
ssize_t read(int fd, void *data, size_t size) {
  char p[256];
  char fn[4096];

  snprintf(p, 256, "/proc/self/fd/%i", fd);
  readlink(p, fn, 4096);
  fprintf(stderr, "read() from %s\n", fn);
  return real_read(fd, data, size);
}

// <some code to make a nice JSON instead of just printing to stdout>
//

/*
 <some code to make it hard to overwrite our function hooks addresses here>
 <some code to forbid reading maps>
 <etc>

extern void *dlsym(void *nupe, const char *nope) {
  fprintf(stderr, "dlsym() called\n");
}

extern void *dlvsym(void *handle, const char *symbol, const char *version) {
  fprintf(stderr, "dlvsym() called\n");
}
*/

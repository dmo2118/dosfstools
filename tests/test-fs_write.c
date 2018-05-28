#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../src/common.h"
#include "../src/io.h"

/* Needs to be in io.c to work. */
void fs_debug()
{
/*
    printf("------------------------------------------------\n");
    for (CHANGE *walk = changes; walk; walk = walk->next) {
	printf("%ld\t%d\t%ld\t\"%.*s\"\n", walk->pos, walk->size,
	       walk->pos + walk->size, walk->size, (char *)walk->data);
    }
*/
}

#define CHECK_FILE "test-fs_write.img"

int main()
{
    struct stat s;
    ssize_t bytes;
    char buf[31];

    int fd = open(CHECK_FILE, O_CREAT | O_TRUNC, 0666);
    if(!fd)
	pdie("open");
    close(fd);

    fs_open(CHECK_FILE, 1);

    fs_write(0, 1, "A");
    fs_debug();
    fs_write(2, 1, "C");
    fs_debug();
    fs_write(4, 1, "E");
    fs_debug();
    fs_write(1, 3, "abc");
    fs_debug();
    fs_write(1, 3, "BCD");
    fs_debug();

    fs_write(6, 2, "AB");
    fs_write(9, 2, "DE");
    fs_debug();
    fs_write(7, 3, "abc");
    fs_debug();

    fs_write(12, 2, "AB");
    fs_debug();
    fs_write(14, 2, "CD");
    fs_debug();

    fs_write(19, 2, "CD");
    fs_debug();
    fs_write(17, 2, "AB");
    fs_debug();

    fs_write(22, 1, "a");
    fs_write(24, 1, "b");
    fs_debug();
    fs_write(22, 3, "ABC");
    fs_debug();

    fs_write(29, 1, "b");
    fs_write(27, 1, "a");
    fs_debug();
    fs_write(26, 5, "ABCDE");
    fs_debug();

    fs_close(1);

    fd = open(CHECK_FILE, O_RDONLY);
    if (fd < 0)
	pdie("open");
    if (fstat(fd, &s) < 0)
	pdie("lstat");
    if (s.st_size != sizeof(buf))
	die(CHECK_FILE ": Bad size");
    bytes = read(fd, buf, sizeof(buf));
    if (bytes < 0)
	pdie("read");
    if (bytes != sizeof(buf))
	die(CHECK_FILE ": Short read");
    if (memcmp(buf, "ABCDE\0AabcE\0ABCD\0ABCD\0ABC\0ABCDE", sizeof(buf)))
	die(CHECK_FILE ": Bad data");
    close(fd);

    return 0;
}

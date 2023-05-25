#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#define fnd "/dev/fnd"

int looptime(int s, int end);

unsigned char fnd_num[4] = {0,};
int fnds;
unsigned char Time_Table[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x67, ~0x00};

int main() {
    fnds = open(fnd, O_RDWR);
    if (fnds < 0) {
        printf("Can't open FND.\n");
        exit(0);
    }

    int end = (unsigned)time(NULL) + 10;  // Countdown for 10 seconds
    int s = 0;

    while (1) {
        if (s < 10) {
            looptime(s, end);
        } else {
            return 0;  // Countdown completed, exit the program
        }
    }
}

int looptime(int s, int end) {
    int start = (unsigned)time(NULL);

    fnd_num[0] = Time_Table[0];
    fnd_num[1] = Time_Table[0];
    fnd_num[2] = Time_Table[9 - s];
    fnd_num[3] = Time_Table[end - start];

    write(fnds, &fnd_num, sizeof(fnd_num));
    sleep(1);

    if (end - start == 0) {
        s++;
    }
    return s;
}

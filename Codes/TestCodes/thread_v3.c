#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include <pthread.h>

// Target System
#define fnd "/dev/fnd" 		// 7-Segment FND 
#define dot "/dev/dot" 		// Dot Matrix
#define tact "/dev/tactsw" 	// Tact Switch
#define led "/dev/led" 		// LED 
#define dip "/dev/dipsw"	// Dip Switch
#define clcd "/dev/clcd" 	// Character LCD

int looptime(int s, int end);

unsigned char fnd_num[4] = {0,};
int fnds;
unsigned char Time_Table[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x67, ~0x00};

//dot matrix로 표현한 트럼프 카드
unsigned char deck[13][8] = {
    // Number 1 (A)
    {0x18, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42},
    // Number 2
    {0x1C, 0x22, 0x02, 0x02, 0x1C, 0x20, 0x20, 0x3E},
    // Number 3
    {0x1C, 0x22, 0x02, 0x1C, 0x02, 0x02, 0x22, 0x1C},
    // Number 4
    {0x04, 0x0C, 0x14, 0x24, 0x44, 0x3E, 0x04, 0x04},
    // Number 5
    {0x1E, 0x20, 0x20, 0x1C, 0x02, 0x02, 0x22, 0x1C},
    // Number 6
    {0x3C, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x3C},
    // Number 7
    {0x3E, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02},
    // Number 8
    {0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x22, 0x1C},
    // Number 9
    {0x1C, 0x22, 0x22, 0x22, 0x1E, 0x02, 0x02, 0x02},
    // Number 10
    {0x84, 0x8A, 0x91, 0x91, 0x91, 0x91, 0x8A, 0x84},
    // Number 11 (J)
    {0x1C, 0x08, 0x08, 0x08, 0x08, 0x48, 0x48, 0x30},
    // Number 12 (Q)
    {0x38, 0x44, 0x82, 0x82, 0x82, 0x8A, 0x44, 0x3A},
    // Number 13 (K)
    {0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x44}
};


void *dot_put(){
	int dot_mtx;
	
	if(( dot_mtx = open(dot,O_RDWR)) <0){
		printf("dot open error");
		return ;
	}
	
    while(1){
	    write(dot_mtx, &deck[0], sizeof(deck[0]));
        printf("dot\n");
        sleep(1000);
    }
    return 0;
}

void *tact_get(){
    unsigned char b;
    int tactsw;
    if((tactsw = open( tact , O_RDWR )) < 0){     	// 예외처리    
		printf("tact open error");
		return ;
	}
    
	while(1){	
	    read(tactsw, &b, sizeof(b));
        printf("tact\n");
        sleep(10);
    }
    return ;
}
void *fnd_put(){
	int fnds;
	if((fnds = open(fnd,O_RDWR)) <0){
		printf("fnd open error");
		return ;
	}
	
    while(1){
    	write(fnds, &Time_Table[0], sizeof(Time_Table[0]));
        printf("fnd\n");
        sleep(100);
    }
    return 0;
}
int main(){
	
	
    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, dot_put, NULL);
    pthread_create(&t2, NULL, tact_get, NULL);
    pthread_create(&t3, NULL, fnd_put, NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *abc(){
    while(1){
        printf("ABC\n");
        sleep(1);
    }
    return 0;
}

void *bts(){
    while(1){
        printf("BTS\n");
        sleep(1);
    }
    return 0;
}

int main(){
    pthread_t t1, t2;

    pthread_create(&t1, NULL, abc, NULL);
    pthread_create(&t2, NULL, bts, NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    return 0;
}
 

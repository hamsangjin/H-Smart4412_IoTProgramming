//tact switch 
#include<stdio.h> 			// 입출력 관련 
#include<stdlib.h> 			// 문자열 변환, 메모리 관련 
#include<unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공 
#include<fcntl.h> 			// 타겟시스템 입출력 장치 관련 
#include<sys/types.h> 			// 시스템에서 사용하는 자료형 정보 
#include<sys/ioctl.h> 			// 하드웨어의 제어와 상태 정보 
#include<sys/stat.h> 			// 파일의 상태에 대한 정보 
#include <string.h> 			// 문자열 처리 
#include <time.h> 			// 시간 관련 

#define tact "/dev/tactsw" 	// Tact Switch

//입력된 시간(초) 동안 tactsw가 0.01초마다 값을 읽고(10초입력시 1000번 읽음), 1초마다 fnd에 남은 제한시간을 출력하는 함수 //반환값 0: 입력없음 1~3: 입력값 4~12: 무시 
int tactsw_get_with_timer(int t_second) 
{   
	int selected_tact = 0;
	unsigned char b=0;
	int tactsw; 
	//tact switch 제한 시간이 0초 이하일 경우 입력값 없음 
	if(t_second <= 0){
		return 0;
	}
	
	if((tactsw = open( tact, O_RDWR )) < 0){     	// 예외처리    
		printf("tact open error");
		return 0;
	}
	
	int i,j;
	for(i = t_second; i>-1;i--){
		for(j = 100; j>0;j--){
			usleep(10000); //0.01 초 쉬기 
            read(tactsw, &b, sizeof(b));
            printf("입력중 %u \n", b);
            	if(1<=b && b<=12){
            		switch (b){
					case 1:  selected_tact = 1 ; break;
					case 2:  selected_tact = 2 ; break;
					case 3:  selected_tact = 3 ; break;
					case 12: 
					{
					//12눌렀을 때 이전에 1이나 2나 3을 눌렀을 경우 
					if(selected_tact==1 ||selected_tact==2||selected_tact==3){
						printf("tactswitch 입력값: %d\n", selected_tact);
						return selected_tact;
					}
					//12를 먼저 누를 경우 
					else printf("press 12 after press 1 or 2 or 3\n");
					}
					//4~12무시 
					default: printf("press other key\n"); break; 		            
				}	
				}
		
		}//1초 지남 = 0.01초*100번 
		printf("%d초\n",i);
	}
	close(tactsw);
	return 0; //제한시간 끝	
}



int main() {
	int user_answer = tactsw_get_with_timer(10);
	printf("user_answer: %d\n", user_answer);
	return 0;
}


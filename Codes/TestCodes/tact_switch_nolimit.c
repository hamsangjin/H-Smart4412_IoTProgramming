#include<stdio.h> 			// 입출력 관련 
#include<stdlib.h> 			// 문자열 변환, 메모리 관련 
#include<unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공 
#include<fcntl.h> 			// 타겟시스템 입출력 장치 관련 
#include<sys/types.h> 			// 시스템에서 사용하는 자료형 정보 
#include<sys/stat.h> 			// 파일의 상태에 대한 정보 
#include <string.h> 			// 문자열 처리 
#include <time.h> 			// 시간 관련 

#define tact "/dev/tactsw" 	// Tact Switch

	int main(){
		
	int tactsw;
	unsigned char b;
	int selected_tact = 0;
	
	if((tactsw = open( tact, O_RDWR )) < 0){     	// 예외처리    
		printf("tact open error");
		return 0;
	}
	
	while(1){
		
		
		 read(tactsw, &b, sizeof(b));
		 printf("입력중 %u \n", b);
			if(1<=b && b<=12){
            		switch (b){
					case 1:  selected_tact = 1 ; 
					case 2:  selected_tact = 2 ; 
					case 3:  selected_tact = 3 ; 
					case 12: 
					{
					//12눌렀을 때 이전에 1이나 2나 3을 눌렀을 경우 
					if(selected_tact==1 ||selected_tact==2||selected_tact==3){
						close(tactsw);
						printf("tactswitch 입력값: %d\n", selected_tact);
						return selected_tact;
					}
					//12를 먼저 누를 경우 
					else printf("press 12 after press 1 or 2 or 3\n");
					}
					//4~12무시 
					default: printf("press other key\n");  		            
				}	
			}
	
	
	}	
}
	
	

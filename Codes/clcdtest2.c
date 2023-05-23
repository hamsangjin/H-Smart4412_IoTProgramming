//clcd 테스트 코드
//CLCDManager: int fd, void printStringOnCLCD(const char* str)
//CLCDTest: char* message[MESSAGE_NUM], void testCLCD();

#include <stdio.h> 
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define MESSAGE_NUM 5
#define clcd "/dev/clcd"


//문자열을 입력받으면 clcd에 문자열 출력 함수 
void printStringOnCLCD(const char* str) {
    int fd = open(clcd, O_WRONLY);
    if (fd < 0) {
        //std::cerr << "Failed to open CLCD device." << std::endl;
        printf("Failed to open CLCD device. 출력하고자 한 문자열: %s\n", str);
        return;
    }

    write(fd, str, strlen(str));

    close(fd);
}

//CLCD 테스트 함수 
void testCLCD();

int main() {
   	testCLCD();
    return 0;
}

//추가 
char* message[MESSAGE_NUM] = {	"press dip switch",
						"player card com card > < = 1 2 3 enter ",
						"yes! user_score +1",
						"no! com_score +1",
						"round: 6, com_score: 5 user_score: 1"};

//char* message[MESSAGE_NUM] = {	"게임을 시작하려면 아무 dip switch를 누르세요",
//						"플레이어 카드가 사용자 카드보다 크면(1), 작으면(2), 같으면(3)을 누르고 엔터(12)를 누르세요",
//						"맞았습니다 user_score +1",
//						"틀렸습니다 com_score +1",
//						"round: 6, com_score: 5 user_score: 1"};


//clcd 테스트 함수 
void testCLCD(){
   	int i;
	   for(i=0;i<MESSAGE_NUM;i++){
   		printStringOnCLCD(message[i]);
	   } 
	
}           

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


//clcd 테스트 함수 
void testCLCD(){
   	int i;
   		printStringOnCLCD("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	
}       

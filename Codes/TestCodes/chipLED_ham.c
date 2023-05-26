#include<stdio.h> 			// 입출력 관련 
#include<stdlib.h> 			// 문자열 변환, 메모리 관련 
#include<unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공 
#include<fcntl.h> 			// 타겟시스템 입출력 장치 관련 
#include<sys/types.h> 		// 시스템에서 사용하는 자료형 정보 
#include<sys/ioctl.h> 		// 하드웨어의 제어와 상태 정보 
#include<sys/stat.h> 		// 파일의 상태에 대한 정보 
#include <string.h> 		// 문자열 처리 
#include <time.h> 			// 시간 관련 

// Target System
#define fnd "/dev/fnd" 		// 7-Segment FND 
#define dot "/dev/dot" 		// Dot Matrix
#define tact "/dev/tactsw" 	// Tact Switch
#define led "/dev/led" 		// LED 
#define dip "/dev/dipsw"	// Dip Switch
#define clcd "/dev/clcd" 	// Character LCD

int main(){
  char led_array[] = { 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00};
  unsigned char data;

  int leds;
  // chip_led 불러오기
  leds = open(led, O_RDWR);

  // chip_led 안 열리면 터미널 오류 메시지 출력
  if (leds < 0) {
    printf("Can't open LED.\n");
    exit(0);
  }


  // chip led 차례대로 켜기
  int i;
  for (i = 0; i <= 8; i++) {
    data = led_array[i];
    write(leds, &data, sizeof(unsigned char));
    usleep(1000000);
  }
}


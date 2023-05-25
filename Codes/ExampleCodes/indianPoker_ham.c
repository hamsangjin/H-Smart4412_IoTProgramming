// Header File
#include<stdio.h> 			// 입출력 관련 
#include<stdlib.h> 			// 문자열 변환, 메모리 관련 
#include<unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공 
#include<fcntl.h> 			// 타겟시스템 입출력 장치 관련 
#include<sys/types.h> 			// 시스템에서 사용하는 자료형 정보 
#include<sys/ioctl.h> 			// 하드웨어의 제어와 상태 정보 
#include<sys/stat.h> 			// 파일의 상태에 대한 정보 
#include <string.h> 			// 문자열 처리 
#include <time.h> 			// 시간 관련 

// Target System
#define fnd "/dev/fnd" 		// 7-Segment FND 
#define dot "/dev/dot" 		// Dot Matrix
#define tact "/dev/tactsw" 	// Tact Switch
#define led "/dev/led" 		// LED 
#define dip "/dev/dipsw"	// Dip Switch
#define clcd "/dev/clcd" 	// Character LCD


// 함수 전역변수 설정
int looptime(int s, int end);
int BETTING_START();
void PRINT(char p[]);
void shuffle_card(int* cards);
void prepare(int* cards1, int* cards2);
int compare_card(int com_card, int user_card);
int win_lose(int user_answer, int correct_answer);
void WIN_LED(int user_win);
void start(int* cards1, int* cards2);



// 장치들 전역 변수 설정
int dipsw;
int leds;
int dot_mtx;
int tactsw;
int clcds;
int fnds;


// 카드관련 전역 변수 설정
const int CARD_NUM = 13;
int usercards[CARD_NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int comcards[CARD_NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};


// fnd led 관련 전역 변수 설정
unsigned char fnd_num[4] = {0,};
unsigned char Time_Table[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x67, ~0x00};


// switch 장치들 입력 변수(확인용)
// dipswitch를 조작하거나 tactswitch를 이용해 베팅했을 때 리턴해주기 위함
unsigned char d;  // dip switch 값 변수
unsigned char t;  // tact switch 값 변수


// 게임 시작 시 CLCD에 출력하는 함수(Dip Switch를 조작해야 0 리턴)
int INTRO(){
	clcds = open(clcd, O_RDWR);
	if(clcds < 0){
    printf("Can't open Character LCD.\n"); 
    exit(0);
  }


	char pr_clcd[40] = " PRESS ANY KEY!  USE DIP SWITCH ";
	write(clcds, pr_clcd, strlen(pr_clcd));  usleep(2000000);
	close(clcds);

	// fnd Led 10초

   // dip switch 입력했냐 안 했냐
	while(1){
		dipsw = open(dip, O_RDWR);
		if (dip < 0) {printf("Can't open Dip Switch\n"); exit(0);}
		read(dipsw, &d, sizeof(d));
		close(dipsw);
		
	  	if (d != 0){
	  		return 0;
		}
	}
}


int BETTING_START(){
      // fnd 10초 시작
   PRINT(" PLEASE BETTING  USE TACTSWITCH ");  usleep(2000000);
      // dotmatrix 출력
      // tactswitch 베팅 입력
      // return 베팅 입력값
}


// 베팅 전 CLCD에 출력하는 함수(Tact Switch를 조작해야 t값 리턴)
// int BETTING_PRINT(){
// 	clcds = open(clcd, O_RDWR);
// 	if(clcds < 0){
//     printf("Can't open Character LCD.\n"); 
//     exit(0);
//   }
// 	char pr_clcd[40] = " PLEASE BETTING  USE TACTSWITCH ";
// 	write(clcds, pr_clcd, strlen(pr_clcd));
// 	close(clcds);
	
// 	while(1){
// 		tactsw = open(tact, O_RDWR);
// 		if (tact < 0) {printf("Can't open tact Switch\n"); exit(0);}
// 		read(tactsw, &t, sizeof(t));
// 		close(tactsw);
		
// 	  	if (t != 0){
// 	  		return t;
// 		}
// 	}
// }

// CLCD 기본 출력 함수 
void PRINT(char P[]){
	clcds = open(clcd, O_RDWR);
	if(clcds < 0){printf("Can't open Character LCD.\n"); exit(0);}
	write(clcds, P, strlen(P));
	close(clcds);
}


//카드 섞기 함수
void shuffle_card(int* cards){
   srand(time(NULL));
   int temp;
   int rn;
   for(int i = 0; i < CARD_NUM; i++){
      rn = rand() % CARD_NUM;
      while (rn == i) {
         rn = rand() % CARD_NUM;
      }
      temp = cards[i];
      cards[i] = cards[rn];
      cards[rn] = temp;
   }
}

// 게임 준비 함수
void prepare(int* cards1, int* cards2){
  //  print_card(cards1);
  //  print_card(cards2);
   shuffle_card(cards1);
   shuffle_card(cards2);
   shuffle_card(cards2); // 다르게 섞이기 위해 한 번 더 셔플
  //  print_card(cards1);
  //  print_card(cards2);
}

//게임 카드 비교 함수   
int compare_card(int com_card, int user_card){
   if(com_card==user_card){
      return 1;
   }
   else if(com_card>user_card){
      return 2;
   }
   else{
      return 3;
   }
}

//게임 OX반환 함수      
int win_lose(int user_answer, int correct_answer){
   if(user_answer == correct_answer){
      return 1;
   }
   else{
      return 0;
   }
}


// Chip LED를 이용한 승리 수 점등 함수
void WIN_LED(int user_win) {
    leds = open(led, O_RDWR);
    if (leds < 0) {
        printf("Can't open LED.\n");
        exit(0);
    }

    unsigned char data = 0xFF;
    for (int i = 0; i < user_win; i++) {
        data &= ~(1 << i);  // 해당 비트(LED)를 0으로 설정하여 켜기
    }

    write(leds, &data, sizeof(unsigned char));
    close(leds);        // 이거 하면 뭔가 꺼질거 같은데
}


// 게임 시작 함수
void start(int* cards1, int* cards2){
  int ROUND = 13;
  int com_score = 0;
  int user_score = 0;

  char round_clcd[32];
  char score_clcd[32];

  INTRO();

  PRINT("      GAME           START!     ");  usleep(2000000);
  PRINT("  INDIAN  GAME     GAME  RULE   ");  usleep(2000000);
  PRINT("     ON THE       TACT  SWITCH  ");  usleep(2000000);
  PRINT("  FIRST BUTTON    PLAYER = COM  ");  usleep(2000000);
  PRINT(" SECOND  BUTTON   PLAYER < COM  ");  usleep(2000000);
  PRINT("  THIRD BUTTON    PLAYER > COM  ");  usleep(2000000);

  for(int i = 0; i < ROUND; i++){

    int com_card = cards1[i];
    int user_card = cards2[i];
    
    sprintf(round_clcd, "    ROUND  %d         START!     ", i+1);
    PRINT(round_clcd);  usleep(2000000);

    int user_answer = BETTING_START;         // 베팅 값 저장

    int correct_answer = compare_card(com_card, user_card);    // 카드 비교 결과 저장
    // com_dot_matrix(com_card);

    if(win_lose(user_answer, correct_answer)){           // 베팅 결과 확인
        user_score++;
        PRINT("     PLAYER           WIN!      "); usleep(2000000);
        WIN_LED(user_score);  // 유저 승리 수 만큼 LED 점등
    }
    else{
        com_score++;
        PRINT("     PLAYER           LOSE      "); usleep(2000000);
    }

    // 스코어 공개
    sprintf(score_clcd, "PLAYER SCORE = %d COM  SCORE = %d ", user_score, com_score);
    PRINT(score_clcd);  usleep(2000000);

    //사용자 카드 공개
    // user_dot_matrix(user_card);

    if(user_score>=7){
      PRINT("   GAME CLEAR      PLAYER WIN   ");   usleep(2000000);
      break;
    }
    if(com_score>=7){
      PRINT("   GAME  OVER     PLAYER  LOSE  ");   usleep(2000000);
        break;
    }
  }
}

int main(){
  if(intro()){
    prepare(usercards, comcards);
    start(usercards, comcards);
  }
  return 0;
}


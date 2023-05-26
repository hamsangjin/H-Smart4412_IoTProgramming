// Header File
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

// 카드개수 
#define CARD_NUM 13

// 함수 전역변수 설정
int looptime(int s, int end);
int BETTING_START();
void PRINT(char p[]);
void shuffle_card(int* cards);
void prepare(int* cards1, int* cards2);
int compare_card(int com_card, int user_card);
int win_lose(int user_answer, int correct_answer);
void WIN_LED(int user_score, int com_score);
int start(int* cards1, int* cards2);
int tactsw_get_with_timer(int t_second); 
int dipsw_get_with_timer(int t_second); 
int tactsw_get_with_timer_dot_mtx_put(int t_second, int com_card);
 
// 장치들 전역 변수 설정
int dipsw;
int leds;
int dot_mtx;
int user_dot_matrix;
int tactsw;
int clcds;
int fnds;


//카드 관련 전역 변수 설정 
int usercards[CARD_NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int comcards[CARD_NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};


// fnd led 관련 전역 변수 설정
unsigned char fnd_num[4] = {0,};
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


// switch 장치들 입력 변수(확인용)
// dipswitch를 조작하거나 tactswitch를 이용해 베팅했을 때 리턴해주기 위함
unsigned char d;  // dip switch 값 변수
unsigned char t;  // tact switch 값 변수


// 게임 시작 시 CLCD에 출력하는 함수(Dip Switch를 조작해야 1 리턴)
int INTRO(){
	clcds = open(clcd, O_RDWR);
	if(clcds < 0){
    printf("Can't open Character LCD.\n"); 
    exit(0);
  }


	char pr_clcd[40] = " PRESS ANY KEY!  USE DIP SWITCH ";
	write(clcds, pr_clcd, strlen(pr_clcd));  usleep(2000000);
	close(clcds);

   // dip switch 10초 동안 입력했냐 안 했냐
	int dip_value =  dipsw_get_with_timer(10); 
	printf("dip value: %d\n", dip_value);
	
	return dip_value;
}


int BETTING_START(int com_card){
      // fnd 10초 시작
   		PRINT(" PLEASE BETTING  USE TACTSWITCH ");  usleep(2000000);
      // dotmatrix 출력
      	printf("dotmatrix: %d",com_card);
      // tactswitch 베팅 입력
	  	int user_answer = tactsw_get_with_timer(10);

      // return 베팅 입력값
    	return user_answer;
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
   int i; 
   for( i = 0; i < CARD_NUM; i++){
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
void WIN_LED(int user_score, int com_score) {
    char led_array[] = { 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00 };   // 16진수 값들 미리 선언
    unsigned char data;
    
    // chip_led 불러오기
    leds = open(led, O_RDWR);

    // chip_led 안 열리면 터미널 오류 메시지 출력
    if (leds < 0) {
        printf("Can't open LED.\n");
        exit(0);
    }

    // chip led 승리 개수에 맞게 켜기
	int i;
    for (i = 0; i <= user_score; i++) {
        data = led_array[i];
        write(leds, &data, sizeof(unsigned char));  // 해당 비트(LED)를 0으로 설정하여 켜기
    }
    usleep(1000000); // 1초 
   //  write(leds, &data, sizeof(unsigned char));
    
   // 하나씩 켜지고 둘 중 한 명이 승리 7개면 꺼짐
    if (user_score >= 7 || com_score >= 7){
      close(leds);
    }
}


// 게임 시작 함수
int start(int* cards1, int* cards2){
  int ROUND = 13;
  int com_score = 0;
  int user_score = 0;

  char round_clcd[32];
  char score_clcd[32];

 

  PRINT("      GAME           START!     ");  usleep(2000000);
  PRINT("  INDIAN  GAME     GAME  RULE   ");  usleep(2000000);
  PRINT("     ON THE       TACT  SWITCH  ");  usleep(2000000);
  PRINT("   1ST BUTTON     PLAYER = COM  ");  usleep(2000000);
  PRINT("   2ND BUTTON     PLAYER < COM  ");  usleep(2000000);
  PRINT("   3RD BUTTON     PLAYER > COM  ");  usleep(2000000);
  PRINT("  12TH  BUTTON      BETTING!    ");  usleep(2000000);

  int i;
  
  for(i = 0; i < ROUND; i++){

    int com_card = cards1[i];
    int user_card = cards2[i];
    
    
     
	printf("com_card: %d\n", com_card);
    printf("user_card: %d\n", user_card);
    
    dot_mtx = open(dot, O_RDWR);
    if (dot_mtx < 0){
    	printf("Cannot open dot device\n");
    	return 0;
    }
   
    write(dot_mtx, &deck[com_card-1],sizeof(deck[com_card-1]));
    //sleep(10);
   
    
    sprintf(round_clcd, "    ROUND  %d         START!     ", i+1);
    PRINT(round_clcd);  usleep(2000000);

    int user_answer = BETTING_START(com_card);         // 베팅 값 저장

    int correct_answer = compare_card(com_card, user_card);    // 카드 비교 결과 저장
    // com_dot_matrix(com_card);

    if(win_lose(user_answer, correct_answer)){           // 베팅 결과 확인
        user_score++;
        PRINT("     PLAYER           WIN!      "); usleep(2000000);
        WIN_LED(user_score,com_score);  // 유저 승리 수 만큼 LED 점등
        close(dot_mtx);
    }
    else{
        com_score++;
        PRINT("     PLAYER           LOSE      "); usleep(2000000);
        close(dot_mtx);
    }

    // 스코어 공개
    sprintf(score_clcd, "PLAYER SCORE = %d COM  SCORE = %d ", user_score, com_score);
    PRINT(score_clcd);  usleep(2000000);

    //사용자 카드 공개
    user_dot_matrix = open(dot, O_RDWR);
    if (user_dot_matrix < 0){
    	printf("Cannot open dot device\n");
    	return 0;
    }
   
    write(user_dot_matrix, &deck[user_card-1],sizeof(deck[user_card-1]));
    sleep(5);
    close(user_dot_matrix);

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


//입력된 시간(초) 동안 tactsw가 값을 읽고(0.01초 마다 read), 
//1초마다 fnd에 남은 제한시간을 출력 //반환값 0: 입력없음 1~3: 입력값 4~12: 무시 
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
	if((fnds = open(fnd,O_RDWR)) <0){
		printf("fnd open error");
		return 0;
	}
	
	int i,j;
	for(i = t_second; i>-1;i--){
		for(j = 100; j>0;j--){
			usleep(10000); //0.01 초 쉬기 
            read(tactsw, &b, sizeof(b));
            //printf("입력중 %u \n", b);
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
						close(tactsw);
						close(fnds);
						return selected_tact;
					}
					//12를 눌렀지만 이전에 1이나 2나 3을 누르지 않았을 경우 
					else printf("press 12 after press 1 or 2 or 3\n");
					}
					//4~11무시 
					default: printf("press other key\n"); break; 		            
				}	
				}
		
		}//1초 지남 = 0.01초*100번 
		int s = i / 10;
		int ss = i % 10;
		fnd_num[0] = Time_Table[0];
    	fnd_num[1] = Time_Table[0];
    	fnd_num[2] = Time_Table[s];
    	fnd_num[3] = Time_Table[ss];
		write(fnds, &fnd_num, sizeof(fnd_num));
		printf("%d초\n",i);
		
	}
	close(tactsw);
	close(fnds);
	return 0; //제한시간 끝	
}

//입력된 시간(초) 동안 dipsw가 값을 읽고(0.01초 마다 read), 
//1초마다 fnd에 남은 제한시간을 출력 //반환값 0: 입력없음 1~3: 입력값 4~12: 무시 
int dipsw_get_with_timer(int t_second) 
{   
	int selected_dip = 0;
	unsigned char d=0;
	int dipsw; 
	
	//dip switch 제한 시간이 0초 이하일 경우 입력값 없음 
	if(t_second <= 0){
		return 0;
	}
	
	if((dipsw = open( dip , O_RDWR )) < 0){     	// 예외처리    
		printf("dip open error");
		return 0;
	}
	if((fnds = open(fnd,O_RDWR)) <0){
		printf("fnd open error");
		return 0;
	}
	
	int i,j;
	for(i = t_second; i>-1;i--){
		for(j = 100; j>0;j--){
			usleep(10000); //0.01 초 쉬기 
            read(dipsw, &d, sizeof(d));
            //printf("입력중 %u \n", b);
            	if(d!=0){
            		selected_dip = d;
            		close(dipsw);
					close(fnds);
            		return selected_dip;
				}
		
		}//1초 지남 = 0.01초*100번 
		int s = i / 10;
		int ss = i % 10;
		fnd_num[0] = Time_Table[0];
    	fnd_num[1] = Time_Table[0];
    	fnd_num[2] = Time_Table[s];
    	fnd_num[3] = Time_Table[ss];
		write(fnds, &fnd_num, sizeof(fnd_num));
		printf("%d초\n",i);
		
	}
	close(dipsw);
	close(fnds);
	return 0; //제한시간 끝	
}


int tactsw_get_with_timer_dot_mtx_put(int t_second, int com_card) 
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
	if((fnds = open(fnd,O_RDWR)) <0){
		printf("fnd open error");
		return 0;
	}
	if((dot_mtx = open(dot,O_RDWR)) <0){
		printf("dot open error");
		return 0;
	}
	
	int i,j;
	write(dot_mtx, &deck[com_card-1], sizeof(deck[com_card-1]));
	for(i = t_second; i>-1;i--){
		for(j = 100; j>0;j--){
			usleep(10000); //0.01 초 쉬기 
            read(tactsw, &b, sizeof(b));
            //printf("입력중 %u \n", b);
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
						close(tactsw);
						close(fnds);
						close(dot_mtx);
						return selected_tact;
					}
					//12를 눌렀지만 이전에 1이나 2나 3을 누르지 않았을 경우 
					else printf("press 12 after press 1 or 2 or 3\n");
					}
					//4~11무시 
					default: printf("press other key\n"); break; 		            
				}	
				}
		
		}//1초 지남 = 0.01초*100번 
		int s = i / 10;
		int ss = i % 10;
		fnd_num[0] = Time_Table[0];
    	fnd_num[1] = Time_Table[0];
    	fnd_num[2] = Time_Table[s];
    	fnd_num[3] = Time_Table[ss];
		write(fnds, &fnd_num, sizeof(fnd_num));
		printf("%d초\n",i);
	}
	close(tactsw);
	close(fnds);
	close(dot_mtx);
	return 0; //제한시간 끝	
}

int main(){
   if(INTRO() != 0){
    	prepare(usercards, comcards);
    	int i  = start(usercards, comcards);
	}
  return 0;
}


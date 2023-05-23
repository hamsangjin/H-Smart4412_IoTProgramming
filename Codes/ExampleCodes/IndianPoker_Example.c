#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//clcd
void clcd(char* message){
   printf("clcd: %s\n", message);
}

//dot_matrix_com
void com_dot_matrix(int c){
   printf("dot_matrix : 해당 판 컴퓨터 카드 출력 : %d\n", c);
}

//dot_matrix_user
void user_dot_matrix(int c){
   printf("dot_matrix : 해당 판 유저 카드 출력 : %d\n", c);
}

//segment_led
void segment_led(int c){
   printf("segment_led: %d\n", c);
}

//chip_led
void chip_led(char* s){
   printf("chip_led: %s\n", s);
}

//tact_switch
int tact_switch(){
   int a;
   scanf("%d", &a);
   return a;
}

//dip switch
int dip_switch(){
   int a;
   scanf("%d", &a);
   return a;
}

const int CARD_NUM = 13;
int usercards[CARD_NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int comcards[CARD_NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
//카드 출력 함수   
// void print_card(int* cards){
//    for(int i=0;i<CARD_NUM;i++){
//          printf("%d, ", cards[i]);
//       }
//       printf("\n");
// }

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
//게임시작함수   
void start(int* cards1, int* cards2, int round){
   int ROUND = round;
   int com_score = 0;
   int user_score = 0;
   

   printf("\nclcd : =========Indian Game========\n");
   printf("clcd : ==========Game Rule=========\n");
   printf("clcd : On the Tact_Switch\n");   
   printf("clcd : First button : player = computer\n"); 
   printf("clcd : Second button : player < computer\n");
   printf("clcd : Third button : player > computer\n");
   printf("clcd : =============================\n\n");

   for(int i = 0; i< ROUND; i++){
      int com_card = cards1[i];
      int user_card = cards2[i];
      printf("\nclcd : ===========Round %d===========\n", i+1);
      printf("FND : 제한 시간 10초 표현\n");
      int correct_answer = compare_card(com_card,user_card);
      com_dot_matrix(com_card);
      // char s[50] = "comcard==usercard:1, >:2, <:3 입력";
      // clcd(s);
      printf("Tact Switch(Player Betting) : ");
      int user_answer = tact_switch();
      if(win_lose(user_answer, correct_answer)){
         user_score++;
         clcd("Player WIN");
         printf("clcd : user_score: %d com_score: %d\n", user_score, com_score);
         printf("chip led로 승리 개수만큼 점등 : %d\n", user_score);
      }
      else{
         com_score++;
         clcd("Player Lose");
         printf("clcd : user_score: %d com_score: %d\n", user_score, com_score);
         printf("chip led로 승리 개수만큼 점등 : %d\n", user_score);
      }
      //사용자 카드 공개
      user_dot_matrix(user_card);
      
      if(user_score>=7){
         printf("\nclcd : You WIN!");
         break;
      }
      if(com_score>=7){
         printf("\nclcd : YOU LOSE!");
         break;
      }
   }
}

//게임인트로함수
int intro(){
   printf("Press Any Key On The Dip Switch : ");
   int a = dip_switch();
   if(a==1){
      clcd("Game Start");
      return 1;
   }
   return 0;
}

//게임준비함수
void prepare(int* cards1, int* cards2){
  //  print_card(cards1);
  //  print_card(cards2);
   shuffle_card(cards1);
   shuffle_card(cards2);
   shuffle_card(cards2); // 다르게 섞이기 위해 한 번 더 셔플
  //  print_card(cards1);
  //  print_card(cards2);
}

//메인함수
int main(){

   int ROUND = 13;
   if(intro()){
      prepare(usercards, comcards);
      start(usercards, comcards, ROUND);
   }
   return 0;
}
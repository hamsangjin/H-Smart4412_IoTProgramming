#include<termios.h>

#include<stdio.h>

#include<stdlib.h>

#include<fcntl.h>

#include<unistd.h>

#include<string.h>

#include<sys/types.h>

#include<asm/ioctls.h>



#define tact_d "/dev/tactsw"

#define fnd_d  "/dev/fnd"



void calc(int *num , char op , int *i);

void PrintFnd(int rst , int k , unsigned char *fnd_data);



int main()

{

	int tact , fnd , sum=0 , i=0;

	char op;

	int num[2];

	unsigned char c;

	

	tact = open(tact_d , O_RDWR);

	

	if(tact < 0)

	{

		printf("tact : open failed!\n");

	}

	

	while(1)

	{

		while(1)

		{

			read(tact , &c , sizeof(c)); //scanf() 처럼 블록상태가 되지 않기 때문에

			usleep(100000);

			if(c) break;

		}	

		

		switch(c)

		{

			case 1: printf("%d입력\n" , c);num[i]=1;i++; break; 

			case 2: printf("%d입력\n" , c);num[i]=2;i++; break; 

			case 3: printf("%d입력\n" , c);num[i]=3;i++; break; 

			case 4: printf("%d입력\n" , c);num[i]=4;i++; break; 

			case 5: printf("%d입력\n" , c);num[i]=5;i++; break; 

			case 6: printf("%d입력\n" , c);num[i]=6;i++; break; 

			case 7: op='+';printf("%c입력\n" , op);break; 

			case 8: op='-';printf("%c입력\n" , op);break; 

			case 9: op='*';printf("%c입력\n" , op);break; 

			case 10: op='/';printf("%c입력\n" , op);break; 

			case 11: calc(num , op , &i); break; 

			case 12: printf("Shutdown!\n"); return;



		}

	}

	close(tact);

	return 0;

	

}



void calc(int *num , char op , int *i)

{

	int rst , k , fnd , divided , reminder;

	unsigned char fnd_data[4];

	

	fnd = open(fnd_d , O_RDWR);	

	fnd_data[0] = 0xFF;

	fnd_data[1] = 0xFF;

	fnd_data[2] = 0xFF;

	fnd_data[3] = 0xFF;



	write(fnd , &fnd_data , sizeof(fnd_data));



	switch(op)

	{

		case '+':rst = num[0]+num[1];break;

		case '-':rst = num[0]-num[1];break;

		case '*':rst = num[0]*num[1];break;

		case '/':rst = num[0]/num[1];break;

	}

	

	if(op != '/')

	{

		if(rst <10)

		{

			for(k=0 ; k<3 ; ++k)

				fnd_data[k] = 0xFF;



			PrintFnd(rst , k , fnd_data);

		}

		else if(rst>=10 && rst <100)

		{

			for(k=0 ; k<2 ; ++k)

				fnd_data[k] = 0xFF;



			divided = rst/10;

			PrintFnd(divided , k , fnd_data);

			reminder = rst%10;

			PrintFnd(reminder , k+1 , fnd_data);

		}

	}

	else if(op == '/')

	{

		for(k=0 ; k<2 ; ++k)

			fnd_data[k] = 0xFF;

	

		divided = num[0]/num[1];

		PrintFnd(divided , k , fnd_data);

		fnd_data[k] &= ~0x80;

	

		reminder = num[0]%num[1];

		PrintFnd(reminder , k+1 , fnd_data);

	}

	write(fnd , &fnd_data , sizeof(fnd_data));

	usleep(100000);

	*i = 0;



}

void PrintFnd(int rst , int k , unsigned char *fnd_data)

{

	if(rst==0)

		fnd_data[k]=0xC0;

	if(rst==1)

		fnd_data[k]=0xF9;

	if(rst==2)

		fnd_data[k]=0xA4;

	if(rst==3)

		fnd_data[k]=0xB0;

	if(rst==4)

		fnd_data[k]=0x99;

	if(rst==5)

		fnd_data[k]=~0x6D;

	if(rst==6)

		fnd_data[k]=~0x7D;

	if(rst==7)

		fnd_data[k]=~0x07;

	if(rst==8)

		fnd_data[k]=~0x7F;

	if(rst==9)

		fnd_data[k]=0x10;



}



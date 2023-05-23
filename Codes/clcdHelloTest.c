#include<stdio.h>

#include<stdlib.h>

#include<fcntl.h>

#include<string.h>



#define clcd "/dev/clcd"



int main()

{

	int clcd_d;

	clcd_d = open(clcd , O_RDWR);

	

	if(clcd_d <0)

	{

		printf("디바이스 드라이버가 없습니다.\n");

		return 0;	

	}

	

	//1.디스크립터  2.문자들 3.문자들의 크기

	write(clcd_d , "Hello World" , 12);



	close(clcd_d);

	return 0;

}

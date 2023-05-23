#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define dip "/dev/dipsw"
#define led "/dev/led"

int main()
{
	int dip_d, dev;
	unsigned char c, data;
	dip_d = open(dip, O_RDWR);
	dev = open(led, O_RDWR);
	
	if(dev<0)
	{
		printf("open failed\n");
		exit(0);
	}

	while(1)
	{
		data=0xff;
		while(1)
		{
			read(dip_d, &c, sizeof(c));
			if(c) break;
			usleep(150000);
		}
		
		switch(c)
		{
			case 1:data &= ~0x01;break;
			case 2:data &= ~0x02;break;
			case 4:data &= ~0x04;break;
			case 8:data &= ~0x08;break;
			case 16:data &= ~0x10;break;
			case 32:data &= ~0x20;break;
			case 64:data &= ~0x40;break;
			case 128:data &= ~0x80;break;
		}
		write(dev, &data, sizeof(unsigned char));
		usleep(200000);
	}

	close(dip);
	close(led);
	return 0;
}



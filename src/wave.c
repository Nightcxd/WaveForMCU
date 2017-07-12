#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int

sbit lcden=P3^4;
sbit rs=P3^5;
sbit rw=P3^6;	
sbit dula=P2^6;
sbit wela=P2^7;
uint cnt50ms=0; 
uchar i=1;
uchar a;
long x;
sbit show= P1^7;
sbit k1=P3^0;
uchar OneHz[]="one HZ";
uchar TenHz[]="Ten Hz";
uchar HundredHz[]="Hundred Hz";
uchar ThouSandHz[]="ThouSand Hz";
  void delay(uint x)
{
	uint a,b;
	for(a=x;a>0;a--)
		for(b=10;b>0;b--); 
}
void delay10ms(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}
void write_com(uchar com)
{
	P0=com;
	rs=0;
	lcden=0;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
	
}

void write_date(uchar date)
{
	P0=date;
	rs=1;
	lcden=1;
	delay(10);
	lcden=0;
	delay(10);
	lcden=1;
	
}

void init()
{
    rw=0;
	dula=0;
	wela=0;
	write_com(0x38);   //显示模式设置：16×2显示，5×7点阵，8位数据接口
	delay(20);
	write_com(0x0f);   //显示模式设置
	delay(20);
	write_com(0x06);   //显示模式设置：光标右移，字符不移
	delay(20);
	write_com(0x01);   //清屏幕指令，将以前的显示内容清除
	delay(20);	
}



void key_scan(){
	if(k1==0){
	   delay10ms();
	   while(k1==0);
		switch(i)
        {
          case 0:
		       TR0=0;
		  	   i=1;
               x=-461;
			   TH0=x>>8;
			   TL0=x;
			   TR0=1;
               break;

          case 1:
		 	   TR0=0;
		  	   i=2;
               x=-4608;
			   TH0=x>>8;
			   TL0=x;
			   TR0=1;
               break;

          case 2:
		  	   TR0=0;
		  	   i=3;
               TH0 = 0x4C;
               TL0 = 0x00;
			   TR0=1;
               break;

          case 3:
		       TR0=0;
		  	   i=0;
               x=-46080;
			   TH0=x>>8;
			   TL0=x;
			   TR0=1;
               break;
				
		}
	   }
}

t0_p1_4() interrupt 1{

if(i==0){
TH0=x>>8;
TL0=x;
	cnt50ms++;
	if(cnt50ms==10){
	   cnt50ms=0;
	   show=~show;	   
}
}
else if(i==3){
      TH0 = 0x4C;
      TL0 = 0x00;	
	   show=~show;	   
}
else{
     TH0=x>>8;
     TL0=x;
	show=~show;	
}
}
main()
{
	init();
   	
	
	 write_com(0x18);
	EA=1;
	ET0=1;
	TMOD=0x01;
	x=-461;
	TH0=x>>8;
	TL0=x;
	TR0=1;
	
	while(1){
	    key_scan();
		if(i==0){
		   P1=~0x08;
	    for(a=0;a<6;a++)
	{
	write_date(OneHz[a]);		
	}		   
}
else if(i==3){
			P1=~0x04;
	   for(a=0;a<6;a++)
	{
	write_date(TenHz[a]);			   
	}
}
else if(i==1){
			P1=~0x01;
	for(a=0;a<11;a++)
	{
	write_date(ThouSandHz[a]);		
	}	
	}

else if(i==2) {	
		   P1=~0x02;
	   for(a=0;a<10;a++)
	{
	write_date(HundredHz[a]);		
	}	
	
}
	}
}

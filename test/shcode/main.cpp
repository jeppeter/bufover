
#include <stdio.h>

extern "C" int __thiscall IntAdd(int a,int b,int c,int d);
extern "C" int __cdecl  CAdd(int a,int b);
extern "C" int __stdcall DecodeBuffer(unsigned char* pinbuf,int inlen,unsigned char* poutbuf,int outlen);
extern "C" void* __stdcall GetBaseAddr(void);


int encode_buffer(unsigned char* pinbuf,int inlen,unsigned char* poutbuf,int outlen)
{
	int cnt=0;
	int i;
	unsigned char* pin=pinbuf,*pout =poutbuf;
	for (i=0;i<inlen;i++,cnt++,pout ++,pin ++)
	{
		if (*pin == 0x0 || *pin == 0xff){
			*pout = 0x1;
			cnt ++;
			pout ++;
			if (*pin == 0x0)
			{
				*pout = 0x02;
			}else{
				*pout = 0x01;
			}
		}else{
			*pout = *pin;
			(*pout)++;
		}
	}
	return cnt;
}

int decode_buffer(unsigned char* pinbuf,int inlen,unsigned char* poutbuf,int outlen)
{
	int cnt=0;
	int i;
	unsigned char* pin=pinbuf,*pout =poutbuf;

	for (i=0;i<inlen;i++,cnt ++,pin ++ ,pout ++)
	{
		if (*pin == 0x1)
		{
			i ++;
			pin ++;
			if (*pin == 0x2)
			{
				*pout = 0x0;
			}
			else
			{
				*pout = 0xff;
			}
		}
		else
		{
			*pout = *pin;
			(*pout) -- ;
		}
	}
	return cnt;
}

void debug_buffer(unsigned char* pbuf,int size)
{
	int i;
	unsigned char* pptr= pbuf;
	printf("buffer(0x%p) cnt(%d)",pbuf,size);
	for (i=0;i<size;i++,pptr ++)
	{
		if ((i % 16) == 0)
		{
			printf("\n0x%08x\t",i);
		}
		printf(" 0x%02x",*pptr);
	}
	printf("\n");
	return;
}

int shell_code(void)
{
	int num=0;
	unsigned char inbuf[10] = {0x1,0x2,0x3,0x4,0x5,0x6,0x0,0x0,0x0,0x0};
	unsigned char outbuf[20];
	unsigned char decbuf[20];
	int cnt,retcnt;

	num = IntAdd(3,4,5,6);

	cnt = encode_buffer(inbuf,sizeof(inbuf),outbuf,sizeof(outbuf));
	debug_buffer(inbuf,sizeof(inbuf));
	debug_buffer(outbuf,cnt);
	retcnt = DecodeBuffer(outbuf,cnt,outbuf,sizeof(outbuf));
	debug_buffer(outbuf,retcnt);

	printf("get addr 0x%p\n",GetBaseAddr());

	return 0;
}

int main(int argc,char* argv[])
{
	printf("this is make shell code\n");	
	shell_code();
	return 0;
}

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
int  IntAdd(int a,int b,int c,int d);
int   CAdd(int a,int b);
int  DecodeBuffer(unsigned char* pinbuf,int inlen,unsigned char* poutbuf,int outlen);
void* GetBaseAddr(void);
int CallJmpoline(void* funcptr);
int shcode_test();

#ifdef __cplusplus
};
#endif




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

#pragma optimize("",off)
int decode_buffer(unsigned char* pinbuf,int inlen,unsigned char* poutbuf,int outlen)
{
	int cnt=0;
	int i,j;
	unsigned char* pin=pinbuf,*pout =poutbuf;

	for (i=0,j=0;i<inlen && j < outlen;i++,cnt ++,pin ++ ,pout ++)
	{
		if (*pin == 0x1)
		{
			i ++;
			pin ++;
			if (*pin == 0x2)
			{
				*pout = 0x0;
				j ++;
			}
			else
			{
				*pout = 0xff;
				j ++;
			}
		}
		else
		{
			*pout = *pin;
			(*pout) -- ;
			j ++;
		}
	}
	return cnt;
}
#pragma optimize("",on)

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
	//unsigned char decbuf[20];
	int cnt,retcnt;

	num = IntAdd(3,4,5,6);

	cnt = encode_buffer(inbuf,sizeof(inbuf),outbuf,sizeof(outbuf));
	debug_buffer(inbuf,sizeof(inbuf));
	debug_buffer(outbuf,cnt);
	retcnt = DecodeBuffer(outbuf,cnt,outbuf,sizeof(outbuf));
	//retcnt=decode_buffer(outbuf,cnt,outbuf,sizeof(outbuf));
	debug_buffer(outbuf,retcnt);

	printf("get addr 0x%p\n",GetBaseAddr());

	return 0;
}

int print_call(void* ptr)
{
	printf("call from jmpoline pointer 0x%p 0x%p\n",ptr,print_call);
	return 0;
}

int main(int argc,char* argv[])
{
	int ret;
	printf("this is make shell code\n");
	printf("shcode_test %d\n",shcode_test());
	shell_code();
	printf("to start\n");
	ret = CallJmpoline((void*)print_call);
	printf("ret %d\n",ret);
	return 0;
}
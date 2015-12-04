
#include <stdio.h>

extern "C" int __thiscall IntAdd(int a,int b,int c,int d);
extern "C" int __cdecl  CAdd(int a,int b);


int shell_code(void)
{
	int num=0;
	num = IntAdd(3,4,5,6);
	printf("num %d\n",num);
	printf("cadd result %d\n",CAdd(3,4));
	printf("cadd res2 %d\n",CAdd(4,5) );
	printf("cadd result %d\n",CAdd(7,4));
	printf("cadd res2 %d\n",CAdd(4,10) );
	return 0;
}

int main(int argc,char* argv[])
{
	printf("this is make shell code\n");	
	shell_code();
	return 0;
}
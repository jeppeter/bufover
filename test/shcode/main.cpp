
#include <stdio.h>

extern "C" int IntAdd(int a,int b);

int shell_code(void)
{
	int num;
	num = IntAdd(3,4);
	printf("num %d\n",num);
	return 0;
}

int main(int argc,char* argv[])
{
	printf("this is make shell code");	
	return 0;
}
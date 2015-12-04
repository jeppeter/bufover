
#include <stdlib.h>
#include <stdio.h>
extern "C" int __cdecl CAdd(int a,int b);

int __cdecl CAdd(int a ,int b)
{
	int* _c = (int*)malloc(10);
	char newbuf[300] = {0x1,0x2};
	int i;

	_c[1] = a;
	_c[2] = b;
	_c[0] = _c[0] + _c[1];
	printf("get a(%d) b(%d)\n",a,b);
	printf("newbuf [3] %d\n",newbuf[0]);
	for (i = 0 ; i < 100;i++){
		_c[0] = _c[1] + _c[2];
		_c[3] = _c[1];
		_c[1] = _c[2];
		_c[2] = _c[3];
	}
	return _c[0];
}

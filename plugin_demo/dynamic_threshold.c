
#ifdef _WINDOWS_
#	include <windows.h>
#	define DLL_EXPORT __declspec(dllexport)
#else
#	define DLL_EXPORT __attribute__((visibility("default")))
#endif
typedef unsigned int uint32; 
_Static_assert(sizeof(uint32) == 4, "please define uint32 correctly"); // 不成立时请修改上面的定义
DLL_EXPORT void ImgProc(const char *imgIn, char *imgOut, uint32 width, uint32 height)
{
	uint32 size = width * height;
	uint32 i;
	unsigned char max = imgIn[0];
	unsigned char min = imgIn[0];
	unsigned char mid;

	for(i = 0; i < size; ++i)
	{
       if((unsigned char)imgIn[i] > max)
		   max = (unsigned char)imgIn[i];
       if((unsigned char)imgIn[i] < min)
		   min = (unsigned char)imgIn[i];
	}
	mid = ((unsigned int)max + (unsigned int)min )/2;

	for(i = 0; i < size; ++i)
	{
		imgOut[i] = (imgIn[i] > mid ? 255 : 0);
	}
}


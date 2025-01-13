#include <stdio.h>

#define gfg 7

#if gfg > 200
#undef gfg
#define gfg 200
#elif gfg < 50
#undef gfg
#define gfg 50
#else
#undef gfg
#define gfg 100
#endif

void printValue(int value) { printf("%d", value); }

int main()
{
    printValue(gfg); // gfg = 50
    #qq
    PrintLineNum;
    printf("#hi");
    return 0;

}

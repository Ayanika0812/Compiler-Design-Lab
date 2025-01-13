#include <stdio.h>

// Driver code
INT main()
{
  FOR (INT i = 1; i <= 10; i++) 
  {
    IF (i == 2) 
    {
      CONTINUE;
    }
    IF (i == 6) 
    {
      BREAK;
    }
    printf("%d ", i);
  }
  RETURN 0;
}
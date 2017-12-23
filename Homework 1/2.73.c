#include <limits.h>
#include <stdio.h>

int saturating_add (int x, int y)
{
    int sum = x + y;
    int posneg = INT_MIN;
    
    int xIsPos = ~(x & posneg);
    int yIsPos = ~(y & posneg);
    int sumisPos = ~(sum & posneg);
    
    int positiveOverflow = (xIsPos & yIsPos & ~sumisPos) != 0;
    int negativeOverflow = (~xIsPos & ~yIsPos & sumisPos) != 0;
    
    positiveOverflow && (sum = INT_MAX);
    negativeOverflow && (sum = INT_MIN);
    
    return sum;
    
}	
#include <limits.h>
#include <stdio.h>

int saturating_add (int x, int y)
{
    int sum = x + y;
    int posneg = INT_MIN;
    int bol;
    
    int xIsPos = ~(x & posneg);
    int yIsPos = ~(y & posneg);    
    
    (xIsPos && yIsPos && __builtin_add_overflow(x,y,&bol)) && (sum = INT_MAX);
    (~xIsPos && ~yIsPos && __builtin_add_overflow(x,y,&bol)) && (sum = INT_MIN);
    
    return sum;
}	
#include<stdio.h>
int main(int argc, char const *argv[])
{
    int a =10;
    float b=3.5;
    printf("value of a is %d\n",a);
    printf("value of b is %.2f\n",b);
    //arithmetic operation
    int sum=a+b;
    int diff=a-b;
    int prod=a*b;
    float div=a/b;

    if (a>b && b!=0)
    {
        printf("a is greater than b and b is not zero\n");
    }
    else if (a==b || b==0)
    {
        printf("a is equal to b or b is zero\n");
    }
    else
    {
        printf("a is less than b and b is not zero\n");
    }
    

    return 0;
}


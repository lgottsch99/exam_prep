#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int slen(char *s)
{
    int i;
    i = 0;

    while (s[i] != '\0')
        i++;
    return (i);
}


void writestr(char *str, int *printed)
{
    int len;
    int i;

    if (!str)
    {
        write(1,"(null)", 6);
        *printed = *printed + 6;
    }
    else
    {
        len = slen(str);
        i = 0;
        while (str[i])
        {
            write(1, &str[i], 1);
            i++;
        }
        *printed = *printed + i;
        return ;
    }
}

void writedec(int num, int *printed)//int range is enough?
{
    char array[10];
    int i;

    if (num == -2147483648) //int min
    {
       write(1, "-2147483648", 11);
        *printed = *printed + (11);

       return;
    }
    else if (num == 0) //0
    {
        write(1, "0", 1);
        *printed = *printed + 1;
        return;
    }
    else if (num < 0) //negative
    {
        num = num * (-1);
        write(1, "-", 1);
        *printed = *printed + 1;

    }
    //modulo scissor appr.
    i = 0;
    while (num)
    {
        array[i++] = (num % 10) + '0'; //store last as char in array
        num = num /10; //cut last digit off
    }
    *printed = *printed + (i);
    //print in reverse
    while (i-- > 0)
        write(1, &array[i], 1);
    return;
}

int gethexlen(int num)
{
    int len;

    len = 0;
    while (num)
    {
        num = num /16;
        len++;
    }
    return (len);
}

void writehex(int num, int *printed)
{
    int hexlen;
    int i;
    char *array;
    char *hex = "0123456789abcdef";

    //if 0 
    if (num == 0)
    {
        write(1, "0", 1);
        *printed = *printed + 1;
        return ;
    }
    //else get hex len
    hexlen = gethexlen(num);

    //malloc array size hexlen
    array = (char *)malloc(sizeof(char) *hexlen);
    if (!array)
        return;

    //modulo to get index, store backw. in array
    i = 0;
    while (i < hexlen)
    {
        array[i] = hex[num % 16];
        num = num / 16;
        i++;
    }
    //print reverse
    while (i-- > 0)
    {
        write(1, &array[i], 1);
        *printed = *printed + 1;
    }
    free(array);
}

void check_case(const char *s, int *printed, int i, va_list name)
{
   // printf("in check case\n");
    if (s[i + 1] == 's')
    {
        writestr(va_arg(name, char *), printed);
    }
    else if (s[i + 1] == 'd')
    {
        writedec(va_arg(name, int), printed);
    }
    else if (s[i + 1] == 'x')
    {
        writehex(va_arg(name, int), printed);
    }
    return ;
}

int ft_printf(const char *s, ...)
{
    va_list name;
    int     printed;
    int i;
    int len;
    char *str;

    i = 0;
    printed = 0;
    str = (char *)s;
    va_start(name, s);
    len = slen(str);
    while (s[i])    //go thru each pos in s and scan for %
    {
        //if yes check i + 1 if d, s, x
        if (s[i] == '%' && s[i + 1] && (s[i + 1] == 's' || s[i + 1] == 'd' || s[i + 1] == 'x'))
        {
            //if (s[i + 1] == 's' || s[i + 1] == 'd' || s[i + 1] == 'x')
           // {
                check_case(s, &printed, i, name);
                i = i + 2;
           // }
        }
        else //if not just write pos
        {
            write(1, &s[i], 1);
            printed++;
            i++;
        }
    }
    va_end(name);
    return (printed);
}

int main ()
{
    char *str="Hello";
    int x = 111;
    char *null= NULL;
    int ft = 42;
    int intmin = -2147483648;
    int ret;
    
    ft_printf("my: World\n");
    ft_printf("my: %% World\n");

    printf("\n------------------ %% s -------------------\n");
    ret = printf("og: %s World\n", str);
   printf("ret: %i\n", ret);
    ret = ft_printf("my: %s World\n", str);
        printf("ret: %i\n", ret);

    ret = printf("og: %s World\n", null);
        printf("ret: %i\n", ret);

    ret = ft_printf("my: %s World\n", null);
        printf("ret: %i\n", ret);

    printf("\n------------------ %% d -------------------\n");
    ret = printf("og: %d World\n", x);
        printf("ret: %i\n", ret);

    ret = ft_printf("my: %d World\n", x);
        printf("ret: %i\n", ret);

    ret = printf("og: %d World\n", intmin);
        printf("ret: %i\n", ret);

    ret = ft_printf("my: %d World\n", intmin);
        printf("ret: %i\n", ret);

    ret = printf("og: %d World\n", 2147483647);
        printf("ret: %i\n", ret);

    ret = ft_printf("my: %d World\n", 2147483647);
        printf("ret: %i\n", ret);

    ret = printf("og: %d World\n", 0);
        printf("ret: %i\n", ret);

    ret = ft_printf("my: %d World\n", 0);
    printf("ret: %i\n", ret);


    printf("\n------------------ %% x -------------------\n");
    ret = printf("og: %x World\n", x);
        printf("ret: %i\n", ret);

    ret = ft_printf("my: %x World\n", x);
        printf("ret: %i\n", ret);

    ret = printf("og: %x World\n", ft);
        printf("ret: %i\n", ret);

    ret = ft_printf("my: %x World\n", ft);
    printf("ret: %i\n", ret);


}

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

void    check_case(const char *s, int i);// int *printed);



int ft_strlen(char *s)
{
    int i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

int ft_printf(const char *s, ...)
{
    int i;
    int printed;
    va_list ap;
    char *str;

    i = 0;
    printed = 0;
    va_start(ap, s);
    str = (char *) s;
    while (i < ft_strlen(str))
    {
        if (str[i]== '%')
        {
            check_case(str, i);// &printed);
            i = i + 2;
        }
        else
        {
            write(1, &str[i], 1);
            i++;
        }
    }
    va_end(ap);
    return (printed);
}

void    check_case(const char *s, int i)// int *printed)
{
    if (s[i + 1] == 's')
		printf("need to put str\n");
     //   ft_putstr(va_arg(ap, char *), printed);
    return;
}

int main (void)
{
    char *zero = NULL;
    char *str = "SUN";
    int p = 0;
	int m = 0;

    printf("-------- s ---------\n");
    p = printf("og: %s\n", str);
	printf("og count: %i\n", p);
    m = ft_printf("my: %s\n", str);
	printf("my count: %i\n", m);
    printf("og NULL: %s\n", zero);
    ft_printf("my NULL: %s\n", zero);

    printf("\n-------- d ---------\n");

    printf("\n-------- x ---------\n");

}

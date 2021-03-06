#include "ftsh.h"
#include "exp_arith.h"

static int		myft_intlen(long long n)
{
	int		i;

	i = 1;
	while (n /= 10)
		++i;
	return (i);
}

/*
**	Long long to ascii
*/

char			*ft_lltoa(long long n)
{
	char	*s;
	int		i;
	int		sign;

	sign = (n < 0) ? -1 : 1;
	if (n == -9223372036854775807 - 1)
		return (s = ft_strdup("-9223372036854775808"));
	i = myft_intlen(n);
	if (n < 0)
		i++;
	if (!(s = (char *)malloc(sizeof(char) * (i + 1))))
		clean_exit(1, MALLOC_ERR);
	s[i--] = '\0';
	n = n * sign;
	while (i >= 0)
	{
		s[i] = n % 10 + '0';
		n = n / 10;
		i--;
	}
	if (sign == -1)
		s[0] = '-';
	return (s);
}

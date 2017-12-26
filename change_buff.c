/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_buff.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omykolai <omykolai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/26 18:00:29 by omykolai          #+#    #+#             */
/*   Updated: 2017/12/26 18:34:02 by omykolai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void	*ft_memalloc(size_t size)
{
	void *mem;

	if (!(mem = malloc(size)))
		return (NULL);
	bzero(mem, size);
	return (mem);
}

static char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char			*sub;
	unsigned int	i;

	if (!s || !(sub = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = -1;
	while (++i < len)
	{
		sub[i] = s[start];
		++start;
	}
	sub[i] = 0;
	return (sub);
}

static char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	char	*ijoin;
	int		len;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	len = strlen(s1) + strlen(s2) + 1;
	if (!(join = (char*)malloc(sizeof(char) * len)))
		return (NULL);
	ijoin = join;
	while (*s1)
		*ijoin++ = *s1++;
	while (*s2)
		*ijoin++ = *s2++;
	*ijoin = 0;
	return (join);
}

static char	*get_endl(char *buff_i)
{
	while (*buff_i != '\n')
		++buff_i;
	return buff_i;
}

int			main(int argc, char const *argv[])
{
	char *buff = ft_memalloc(sizeof(*buff) * 10000);

	int fd = open("get_next_line.h", O_RDONLY);
	read(fd, buff, 10000);
	close(fd);

	char *buff_i = strstr(buff, "BUFF_SIZE");
	if (buff_i == NULL)
		printf(">>BUFF_SIZE was not found in get_next_line.h\n");
	else if (argc == 2)
	{
		char *new_gnl_header;
		new_gnl_header = ft_strsub(buff, 0, buff_i + 9 - buff);
		new_gnl_header = ft_strjoin(new_gnl_header, " ");
		new_gnl_header = ft_strjoin(new_gnl_header, argv[1]);

		char *buff_endl = get_endl(buff_i);
		new_gnl_header = ft_strjoin(new_gnl_header, ft_strsub(buff, buff_endl - buff, strlen(buff) - (buff_endl - buff)));

		int fd = open("get_next_line.h", O_WRONLY | O_TRUNC);
		write(fd, new_gnl_header, strlen(new_gnl_header));
		close(fd);
	}
	return 0;
}
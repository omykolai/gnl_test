/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omykolai <omykolai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 12:55:34 by omykolai          #+#    #+#             */
/*   Updated: 2017/12/26 20:14:34 by omykolai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "get_next_line.h"

void	*ft_memalloc(size_t size)
{
	void *mem;

	if (!(mem = malloc(size)))
		return (NULL);
	bzero(mem, size);
	return (mem);
}

void check_leaks(void)
{
	system("leaks test_gnl | grep Process | grep leaks > tmp.log");

	char *buff = ft_memalloc(sizeof(*buff) * 1000);

	int fd = open("tmp.log", O_RDONLY);
	read(fd, buff, 1000);
	close(fd);
	system("rm tmp.log");

	char *leaks_str = strstr(buff, " 0 leaks");
	if (leaks_str == NULL)
		system("touch has_leaks");
}

int main(int argc, char **argv)
{
	char *line;
	
	if (argc == 2)
	{
		int fd = open("temp.txt", O_CREAT | O_WRONLY);

		if (argv[1][0] == '1')
		{
			int fd1 = open("text1", O_RDONLY);
			int fd2 = open("text2", O_RDONLY);
			int fd3 = open("text3", O_RDONLY);

			dprintf(fd, "%d\n", get_next_line(-15, &line));
			dprintf(fd, "%d\n", get_next_line(1, NULL));
			for (int i = 0; i < 32; ++i)
			{
				int red;
				if ((red = get_next_line(fd1, &line)) == 1)
					dprintf(fd, "%d\n%s\n", i + 1, line);
				else
					dprintf(fd, "%d\n", red);
				free(line);
				line = NULL;
				if ((red = get_next_line(fd2, &line)) == 1)
					dprintf(fd, "%s\n", line);
				else
					dprintf(fd, "%d\n", red);
				free(line);
				line = NULL;
				if ((red = get_next_line(fd3, &line)) == 1)
					dprintf(fd, "%s\n\n", line);
				else
					dprintf(fd, "%d\n\n", red);
				free(line);
				line = NULL;
			}

			close(fd1);
			close(fd2);
			close(fd3);
		}
		else if (argv[1][0] == '2')
		{
			int	fd1 = open("text", O_RDONLY);

			for (int i = 0; i < 10; ++i)
			{
				int red;
				if ((red = get_next_line(fd1, &line)) == 1)
					dprintf(fd, "%d\n%s\n", i + 1, line);
				else
					dprintf(fd, "%d\n", red);
				free(line);
				line = NULL;
			}
			
			close(fd1);			
		}

		close(fd);
	}
	else
		printf("WRONG ARGC %d\n", argc);
	/*system("chmod 666 temp1.txt");
	system("chmod 666 temp2.txt");
	system("echo \"diff1: \" && diff res1.txt temp1.txt");
	system("echo \"diff2: \" && diff res1.txt temp1.txt");
	system("rm temp1.txt");
	system("rm temp2.txt");*/
	check_leaks();
	return (0);
}
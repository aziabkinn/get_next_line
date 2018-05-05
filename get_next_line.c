/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziabkin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/26 15:34:36 by aziabkin          #+#    #+#             */
/*   Updated: 2018/05/04 15:37:53 by aziabkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int			write_to_line(char **line, t_gnl *cur)
{
	char		*tmp;

	if (*(cur->str))
	{
		if (ft_strchr(cur->str, '\n'))
		{
			*line = ft_strsub(cur->str, 0, \
				ft_strchr(cur->str, '\n') - cur->str);
			tmp = cur->str;
			cur->str = ft_strdup(cur->str + \
				(ft_strchr(cur->str, '\n') - cur->str) + 1);
			ft_strdel(&tmp);
		}
		else
		{
			*line = cur->str;
			cur->str = ft_strnew(0);
		}
		return (1);
	}
	return (0);
}

static void			reader_and_save(int fd, char *buf, t_gnl *cur)
{
	int			bytes;
	char		*tmp;

	while (!(ft_strchr(cur->str, '\n')) \
		&& (bytes = read(fd, buf, BUFF_SIZE)) > 0)
	{
		tmp = cur->str;
		buf[bytes] = '\0';
		cur->str = ft_strjoin(cur->str, buf);
		ft_strdel(&tmp);
	}
}

static t_gnl		*check_fd(t_gnl **lst, int fd)
{
	t_gnl			*temp;

	temp = *lst;
	while (temp)
	{
		if (temp->fd == fd)
			return (temp);
		temp = temp->next;
	}
	if (!(temp = (t_gnl *)malloc(sizeof(*temp))))
		return (NULL);
	temp->str = ft_strnew(0);
	temp->fd = fd;
	temp->next = *lst;
	*lst = temp;
	return (temp);
}

int					get_next_line(const int fd, char **line)
{
	char			buf[BUFF_SIZE + 1];
	t_gnl			*cur;
	static t_gnl	*lstsave;

	if (read(fd, buf, 0) < 0 || !line || BUFF_SIZE < 1)
		return (-1);
	cur = check_fd(&lstsave, fd);
	reader_and_save(fd, buf, cur);
	return (write_to_line(line, cur));
}

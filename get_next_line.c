/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 06:07:53 by tjung             #+#    #+#             */
/*   Updated: 2020/10/23 21:58:40 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	len;
	size_t	i;

	if (dst == NULL)
		return (0);
	len = ft_strlen(src);
	i = 0;
	if (dstsize)
	{
		while (src[i] && (i + 1 < dstsize))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (len);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	while (dst[i] && (i < dstsize))
		i++;
	while (src[j] && (i + j + 1 < dstsize))
	{
		dst[i + j] = src[j];
		j++;
	}
	if (i < dstsize)
		dst[i + j] = '\0';
	return (i + ft_strlen(src));
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;
	int		s1_len;
	int		s2_len;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	s3 = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (s3 == NULL)
		return (NULL);
	ft_strlcpy(s3, s1, s1_len + 1);
	ft_strlcat(s3, s2, s1_len + s2_len + 1);
	return (s3);
}

/* 개행을 찾으면 그 앞까지 반환(1)하고 남은 바이트를 보관한다. EOF의 마지막 조각은 0을 반환한다. */
int		get_nl(int fd, char **line, char **strg, char *nlpt)
{
	char *temp;

	if (nlpt)
	{
		*line = ft_substr(strg[fd], 0, nlpt - strg[fd]);
		temp = ft_strdup(nlpt + 1);
		if (strg[fd])
			free(strg[fd]);
		strg[fd] = temp;
		return (1);
	}
	else
	{
		*line = ft_substr(strg[fd], 0, ft_strlen(strg[fd]));
		if (strg[fd])
			free(strg[fd]);
		strg[fd] = NULL;
		return (0);
	}
}

/* 정적 strg 배열은 fd마다 읽고 남은 바이트를 다음 호출까지 유지한다. */
int		get_next_line(int fd, char **line)
{
	static char	*strg[OPEN_MAX];
	char		*buff;
	char		*temp;
	char		*nlpt;
	int			rsize;

	if (!(line) || BUFFER_SIZE <= 0 || fd < 0 || fd > OPEN_MAX)
		return (-1);
	if (!(buff = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1)))
		return (-1);
	while ((rsize = read(fd, buff, BUFFER_SIZE)) >= 0)
	{
		buff[rsize] = '\0';
		temp = strg[fd] == NULL ? ft_strdup(buff) : ft_strjoin(strg[fd], buff);
		if (strg[fd])
			free(strg[fd]);
		strg[fd] = temp;
		if ((nlpt = ft_strchr(strg[fd], '\n')) || !(rsize))
			break ;
	}
	free(buff);
	buff = NULL;
	if (rsize == -1)
		return (-1);
	return (get_nl(fd, line, strg, nlpt));
}

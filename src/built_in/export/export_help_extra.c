/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_help_extra.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:11:15 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/11 00:12:53 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**make_arr_export(char *exp, char **exports)
{
	size_t	i;

	i = 0;
	if (ft_strcmp(exp, "") == 0)
		empty_export();
	while (exp[i])
	{
		if (exp[i] != '=')
			break ;
		if ((ft_isalnum(exp[i]) == 0) && exp[i])
		{
			ft_fprintf(2, "minishell: export: `%s", mini_join(exp, INV));
			exit (1);
		}
		i++;
	}
	if (find_line(exports, exp) != -1)
		exports = replace_line(exports, exp, find_line(exports, exp));
	else
		exports = export_to_arr(exp, exports);
	exit (0);
}

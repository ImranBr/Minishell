int are_single_quotes_closed(char *input)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (input[i])
    {
        if (input[i] == '\'')
            count ++;
        i++;
    }
    if (count % 2 == 0)
        return (0);
    else if (count % 2 != 0)
        return (ft_putendl_fd("syntax error: unclosed single quote", 2), 1);
}

int are_double_quotes_closed(char *input)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (input[i])
    {
        if (input[i] == '\"')
            count ++;
        i++;
    }
    if (count % 2 == 0)
        return (0);
    else if (count % 2 != 0)
        return (ft_putendl_fd("syntax error: unclosed double quote", 2), 1);
}
int main()
{
    char *s1 = "ceci est \"un test";
    char *s2 = "ceci est un test'";
    printf("s1 single quotes = %d\ns1 double quotes = %d\n", are_single_quotes_closed(s1), are_double_quotes_closed(s1));
    printf("s2 single quotes = %d\ns2 double quotes = %d\n", are_single_quotes_closed(s2), are_double_quotes_closed(s2));
}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <list>
#include <string>


struct digit
{
    int a:1;
    int b:1;
    int c:1;
    int d:1;
    int e:1;
    int f:1;
    int g:1;
};

class Number
{
    private:
        struct digit data = { 0 };
    protected:
    public:
        int setDigits(const char *digits)
        {
            while (*digits != '\0')
            {
                switch (*digits)
                {
                    case 'a':
                        data.a = 1;
                        break;
                    case 'b':
                        data.b = 1;
                        break;
                    case 'c':
                        data.c = 1;
                        break;
                    case 'd':
                        data.d = 1;
                        break;
                    case 'e':
                        data.e = 1;
                        break;
                    case 'f':
                        data.f = 1;
                        break;
                    case 'g':
                        data.g = 1;
                        break;
                    default:
                        printf("Unknown digit %c\n", *digits);
                        return -1;
                }
                digits++;
            }

            return 0;
        }

        int countSegments(void)
        {
            int segments = 0;

            if (data.a)
                segments++;
            if (data.b)
                segments++;
            if (data.c)
                segments++;
            if (data.d)
                segments++;
            if (data.e)
                segments++;
            if (data.f)
                segments++;
            if (data.g)
                segments++;

            return segments;
        }


        void print(void)
        {
        }
};


class Line
{
    private:
    protected:
    public:
        std::list<Number *> numbers;
        std::list<Number *> solutions;
};

//std::list<Number *>::iterator it;

std::list<Line *> lines;

int parse(FILE *fp)
{
    char *line = NULL;
    size_t linelen = 0;
    ssize_t read;

    while ((read = getline(&line, &linelen, fp)) != -1)
    {
        if (line[read-1] == '\n')
        {
            line[read-1] = '\0';
            read--;
        }

        if ((*line == '\n') || (*line == '\0'))
            continue;


        char *token = NULL;
        const char *delim = " ";
        

        bool firstPart = true;

        token = strtok(line, delim);
        while (token != NULL)
        {
            printf("token: %s\n", token);
            if (*token == '|')
            {
                firstPart = false;
                token = strtok(NULL, delim);
                continue;
            }

            Number *number = new Number();
            number->setDigits(token);


            if (firstPart == true)
                numbers.push_back(number);
            else
                solutions.push_back(number);

            token = strtok(NULL, delim);
        }

    }
    if (line != NULL)
        free(line);

    return 0;
}


int compute(void)
{
    std::list<Number *>::iterator it;
    int num = 0;

    for (it = solutions.begin(); it != solutions.end(); it++)
    {
        Number *n = *it;
        int count = n->countSegments();

        if ((count == 2) ||
            (count == 4) ||
            (count == 3) ||
            (count == 7))
        {
            num++;
        }

    }
        printf("count: %d\n", num);

    return 0;
}


int main(int argc, char *argv[])
{
    const char *f_inp;
    FILE *fp = NULL;

    if (argc >= 2)
    {
        f_inp = argv[1];
    }
    else
    {
        fprintf(stderr, "Provide input file.\n");
        return 1;
    }

    fp = fopen(f_inp, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open file.\n");
        return 1;
    }

    parse(fp);
    compute();
    fclose(fp);

    return 0;
}

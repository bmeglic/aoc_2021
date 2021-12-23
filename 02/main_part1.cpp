#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



class Sub
{
    private:
        int _depth;
        int _horizontal;
    protected:
    public:
        void setInitial(int depth, int horizontal) { _depth = depth; _horizontal = horizontal; }
        int getDepth() { return _depth; }
        int getHorizontal() { return _horizontal; }

        void moveDown(int depth) { _depth += depth; }
        void moveUp(int depth) { _depth -= depth; }
        void moveForward(int horizontal) { _horizontal += horizontal; }
};


int parse(FILE *fp)
{
    char *line = NULL;
    size_t linelen = 0;
    const char *delim = " ";
    ssize_t read;
    Sub sub;

    sub.setInitial(0, 0);

    while ((read = getline(&line, &linelen, fp)) != -1)
    {
        char *cmd;
        char *num_str;
        int num;

        cmd = strtok(line, delim);
        num_str = strtok(NULL, delim);

        num = atoi(num_str);

        fprintf(stdout, "cmd: %s, num: %d\n", cmd, num);

        if (strcmp(cmd, "forward") == 0)
            sub.moveForward(num);
        else if (strcmp(cmd, "down") == 0)
            sub.moveDown(num);
        else if (strcmp(cmd, "up") == 0)
            sub.moveUp(num);
        else
            fprintf(stderr, "Unknown command: %s\n", cmd);
    }

    fprintf(stdout, "sub depth: %d, h: %d\n", sub.getDepth(), sub.getHorizontal());
    fprintf(stdout, "sub mult: %d\n", sub.getDepth() * sub.getHorizontal());

    if (line != NULL)
        free(line);
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
    fclose(fp);

    return 0;
}

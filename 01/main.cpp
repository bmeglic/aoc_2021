#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


int parse(FILE *fp)
{
    char *buf = NULL;
    size_t buflen = 0;
    ssize_t read;

    uint32_t meas_prev = 0;
    uint32_t meas = 0;
    uint32_t meas_larger_cnt = 0;


    while ((read = getline(&buf, &buflen, fp)) != -1)
    {
        meas = strtoul(buf, NULL, 10);

        if ((meas_prev != 0) && (meas > meas_prev))
        {
            meas_larger_cnt++;
            fprintf(stdout, "Meas: %u, increased\n", meas);
        }
        else
        {
           fprintf(stdout, "Meas: %u\n", meas);
        }

       meas_prev = meas;
    }

    fprintf(stdout, "Meas cnt: %u\n", meas_larger_cnt);

    if (buf != NULL)
    {
        free(buf);
    }

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
        fprintf(stderr, "Provide input file\n");
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

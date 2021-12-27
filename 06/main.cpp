#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <list>
#include <string>




#define FISH_STATES_NUM 9

uint64_t fishes[FISH_STATES_NUM] = {0};

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
        const char *delim = ",";

        token = strtok(line, delim);
        while (token != NULL)
        {
            int initialState = atoi(token);

            if (initialState < FISH_STATES_NUM)
                fishes[initialState]++;
            else
                fprintf(stderr, "bad state\n");
            

            token = strtok(NULL, delim);
        }


    }
    if (line != NULL)
        free(line);

    return 0;
}


void printFishesState(void)
{

    for (int i = 0; i < FISH_STATES_NUM; i++)
        fprintf(stdout, "%d:\t", i);
    fprintf(stdout, "\n");
    for (int i = 0; i < FISH_STATES_NUM; i++)
    {
        fprintf(stdout, "%lu\t", fishes[i]);
    }

    fprintf(stdout, "\n");
}



int compute(void)
{
    int numGenerations = 256;

    for (int gen = 0; gen < numGenerations; gen++)
    {
        uint64_t new_fishes = fishes[0];

        printFishesState();
        fprintf(stdout, "Creating new fishes: %lu\n", new_fishes);

        for (int state = 1; state < FISH_STATES_NUM; state++)
        {
            fishes[state-1] = fishes[state];
        }

        fishes[6] += new_fishes;
        fishes[8] = new_fishes;

    }

    printFishesState();
    uint64_t allFishes = 0;

    for (int i = 0; i < FISH_STATES_NUM; i++)
    {
        allFishes += fishes[i];
    }
    fprintf(stdout, "num: %lu\n", allFishes);

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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <list>
#include <string>


struct fld
{
    std::string bits;
    uint32_t val;
};

std::list<struct fld> values;

uint32_t conv_bin_to_dec(const char *bin, size_t binlen)
{
    uint32_t val = 0;
    char ch;
    size_t len;
    size_t bit = 1;

    ch = *bin++;
    while (ch != '\0')
    {
        if (ch == '1')
            val |= (1 << (binlen-bit));

        ch = *bin++;
        bit++;
    }

    return val;
}

int parseLine(char *line, size_t linelen)
{
    char ch;
    uint32_t line_val;
    struct fld fld;

    line_val = conv_bin_to_dec(line, linelen);

    fld.bits = line;
    fld.val = line_val;
    values.push_back(fld);

    return 0;
}



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
        parseLine(line, read);
    }
    if (line != NULL)
        free(line);

    return 0;
}

int get_bits_len()
{
    struct fld fld = *values.begin();
    return fld.bits.length();
}

void compute_onezero_bits(std::list<struct fld> &vals, int ones[], int zeros[])
{
    std::list<struct fld>::iterator it;

    memset(ones, 0, get_bits_len() * sizeof(int));
    memset(zeros, 0, get_bits_len() * sizeof(int));

    for (it = vals.begin(); it != vals.end(); it++)
    {
        struct fld fld = *it;
        const char *str = fld.bits.c_str();

        for (int i = 0; i < fld.bits.length(); i++)
        {
            const char ch = str[i];

            if (ch == '0')
                zeros[i]++;
            else if (ch == '1')
                ones[i]++;
        }
    }
}

int compute_rating(std::list<struct fld> &vals, int higher)
{
    std::list<struct fld>::iterator it;
    int ones[100] = {0};
    int zeros[100] = {0};
    int bits_len = get_bits_len();

    for (int i = 0; i < bits_len; i++)
    {
        int keep;

        compute_onezero_bits(vals, ones, zeros);

        if (ones[i] > zeros[i])
            keep = (higher) ? '1' : '0';
        else if (zeros[i] > ones[i])
            keep = (higher) ? '0' : '1';
        else if (ones[i] == zeros[i])
            keep = (higher) ? '1' : '0';

        it = vals.begin();
        while (it != vals.end())
        {
            struct fld fld = *it;
            char ch = fld.bits.c_str()[i];

            if (ch == keep)
            {
                ++it;
            }
            else
            {
                vals.erase(it++);
                if (vals.size() == 1)
                    break;
            }
        }

        for (it = vals.begin(); it != vals.end(); ++it)
        {
            struct fld fld = *it;
        }

        if (vals.size() == 1)
            break;
    }

    return 0;
}

int compute(void)
{
    std::list<struct fld>::iterator it;
    int ones[100] = { 0 };
    int zeros[100] = { 0 };
    int bits_len = get_bits_len();

    compute_onezero_bits(values, ones, zeros);

    int gamma = 0;
    int epsilon = 0;
    for (int i = 0; i < bits_len; i++)
    {
        if (ones[i] > zeros[i])
            gamma |= 1 << (bits_len-i-1);
        else
            epsilon |= 1 << (bits_len-i-1);

    }
    fprintf(stdout, "gamma: %d, epsilon: %d\n", gamma, epsilon);
    fprintf(stdout, "mul: %d\n", gamma * epsilon);


    std::list<struct fld> val_oxy = values;
    std::list<struct fld> val_co2 = values;

    compute_rating(val_oxy, 1);
    it = val_oxy.begin();
    struct fld oxy_fld = *it;
    fprintf(stdout, "oxy: %s, %u\n", oxy_fld.bits.c_str(), oxy_fld.val);

    compute_rating(val_co2, 0);
    it = val_co2.begin();
    struct fld co2_fld = *it;
    fprintf(stdout, "co2: %s, %u\n", co2_fld.bits.c_str(), co2_fld.val);

    fprintf(stdout, "life support rating: %u\n", oxy_fld.val * co2_fld.val);

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

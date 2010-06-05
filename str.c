/*
 * File:   str.c
 * Author: andrei
 *
 * Created on June 2, 2010, 9:57 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLENGTH 512
#define SEPARATOR " "


char blank[] = "NULL";
char output[MAXLENGTH];

char* cat2(char* s1, char* s2)
{
    if (s1 == NULL)
        s1 = blank;
    if (s2 == NULL)
        s2 = blank;
    sprintf(output, "%s%s%s", s1, SEPARATOR, s2);

    return strdup(output);
}

char* cat3(char* s1, char* s2, char* s3)
{
    char* part1 = cat2(s1, s2);
    if (s3 == NULL)
        s3 = blank;
    sprintf(output, "%s%s%s", part1, SEPARATOR, s3);
    free(part1);
    
    return strdup(output);
}

char* cat4(char* s1, char* s2, char* s3, char* s4)
{
    char* part1 = cat2(s1, s2);
    char* part2 = cat2(s3, s4);
    sprintf(output, "%s%s%s", part1, SEPARATOR, part2);
    free(part1);
    free(part2);

    return strdup(output);
}

char* cat5(char* s1, char* s2, char* s3, char* s4, char* s5)
{
    char* part1 = cat4(s1, s2, s3, s4);
    if (s5 == NULL)
        s5 = blank;
    sprintf(output, "%s%s%s", part1, SEPARATOR, s5);
    free(part1);

    return strdup(output);
}

char* cat6(char* s1, char* s2, char* s3, char* s4, char* s5, char* s6)
{
    
    char* part1 = cat4(s1, s2, s3, s4);
    char* part2 = cat2(s5, s6);
    sprintf(output, "%s%s%s", part1, SEPARATOR, part2);
    free(part1);
    free(part2);

    return strdup(output);
}

char* cat7(char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7)
{
    char* part1 = cat6(s1, s2, s3, s4, s5, s6);
    if (s7 == NULL)
        s7 = blank;
    sprintf(output, "%s%s%s", part1, SEPARATOR, s7);
    free(part1);

    return strdup(output);
}

char* cat8(char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8)
{
    char* part2 = cat4(s5, s6, s7, s8);
    char* part1 = cat4(s1, s2, s3, s4);
    sprintf(output, "%s%s%s", part1, SEPARATOR, part2);
    free(part1);
    free(part2);

    return strdup(output);
}

char* cat9(char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9)
{
    char* part1 = cat8(s1, s2, s3, s4, s5, s6, s7, s8);
    if (s9 == NULL)
        s9 = blank;
    sprintf(output, "%s%s%s", part1, SEPARATOR, s9);
    free(part1);

    return strdup(output);
}

char* cat10(char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9, char* s10)
{
    char* part2 = cat6(s5, s6, s7, s8, s9, 10);
    char* part1 = cat4(s1, s2, s3, s4);
    sprintf(output, "%s%s%s", part1, SEPARATOR, part2);
    free(part1);
    free(part2);

    return strdup(output);
}

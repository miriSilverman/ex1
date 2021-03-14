//
// Created by miris on 01/03/2021.
//


#include <stdio.h>
#include <stdlib.h>
#include "cipher.h"
#include <string.h>
#include <assert.h>
#include <math.h>

#define MOD(k) k >= 0 ? (k = k) : (k = LETTERS_NUM + k)

const char* CHECK_ERROR_MSG = "Usage: cipher <check> <source path file> <output path file>\n";
const char* ENCODE_ERROR_MSG = "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n";
const char* INVALID_FILE_MSG = "The given file is invalid\n";
const char* INVALID_COMMAND_MSG = "The given command is invalid\n";
const char* INVALID_SHIFT_MSG = "The given shifts value is invalid\n";
const char* INVALID_ENCRYPTING_MSG = "Invalid encrypting\n";
const char* VALID_ENCRYPTING_MSG = "Valid encrypting with k = %d\n";
const char* ENCODE_COMMAND = "encode";
const char* DECODE_COMMAND = "decode";
const char* CHECK_COMMAND = "check";
const char* READ_MODE = "r";
const char* WRITE_MODE = "w";
const int MAX_TEXT_SIZE = 10000;
const int LOWER_CASE_S_RANGE = 97;
const int LOWER_CASE_E_RANGE = 122;
const int UPPER_CASE_S_RANGE = 65;
const int UPPER_CASE_E_RANGE = 90;
const int LETTERS_NUM = 26;





int main(int argc, char* argv[]) {
//    printf("Hello, World!\n");
//    for (int i = 0; i < argc; ++i) {
//        printf("%s\n", argv[i]);
//    }
//    check("bcaB!AnN   a", "abzA!ZmM   z");
//    check("bcaB!AnN   a", "abzA!ZmM   z");
//    check("az", "ba");
//    check("zam", "abn");
//    check("za", "yz");

//    int x;
//    int y = 2;
//    if (y == 2)
//    {
//        int x;
//        printf("%d" , x);
//
//    }
//    printf("%d" , x);
//    printf("Size of char %lu\n", sizeof(char));
//    printf("Size of short %lu\n", sizeof(short));
//    printf("Size of int %lu\n", sizeof(int));
//    printf("Size of long %lu\n", sizeof(long));
//    printf("Size of float %lu\n", sizeof(float));
//    printf("Size of double %lu\n", sizeof(double));
//    printf("Size of long double %lu\n\n", sizeof(long double));
//
//    int x = -2;
//    unsigned int y = x + 1;
////    int x = pow(2, 31);
////    short y = x;
//    printf("2%%5 = %d\n", 2%5);
//    printf("2%%-5 = %d\n", 2%-5);
//    printf("-2%%-5 = %d\n", -2%-5);
//    printf("-2%%5 = %d\n", -2%5);
//    printf("5%%-2 = %d\n", 5%-2);
//    printf("-5%%-2 = %d\n", -5%-2);


//    printf("%u  %d", -1, -1);

    //  4294967295  -1


    return tests();
//    return checkCommand(argc, argv);
}




int checkCommand(int argc, char* argv[])
{
    char* command = argv[1];
    if (command == NULL)
    {
        fprintf(stderr, "%s", INVALID_COMMAND_MSG);
        return EXIT_FAILURE;
    }

    if (!strcmp(command, ENCODE_COMMAND))
    {
        return checkEncode(argc, argv, 1);
    }
    else if(!strcmp(command, DECODE_COMMAND))
    {
        return checkEncode(argc, argv, 0);
    }
    else if (!strcmp(command, CHECK_COMMAND))
    {
        return checkCommandCheck(argc, argv);
    }

    fprintf(stderr, "%s", INVALID_COMMAND_MSG);
    return EXIT_FAILURE;
}

int checkEncode(int argc, char* argv[], int isEncode)
{
    if (checkArgsNum(argc, 5, ENCODE_ERROR_MSG))
    {
        return EXIT_FAILURE;
    }

    int k = atoi(argv[2]);
//    if (k == 0)
//    {
//        fprintf(stderr, "%s", INVALID_SHIFT_MSG);
//        return EXIT_FAILURE;
//    }

    FILE* readingFile = fopen(argv[3], READ_MODE);
    FILE* writingFile = fopen(argv[4], WRITE_MODE);
    if (readingFile == NULL)
    {
        fprintf(stderr, "%s", INVALID_FILE_MSG);
        return EXIT_FAILURE;
    }
    if (isEncode)
    {
        encodeCase(readingFile, writingFile, k);
    } else
    {
        decodeCase(readingFile, writingFile, k);
    }
    fclose(readingFile);
    fclose(writingFile);

    return EXIT_SUCCESS;
}


int checkCommandCheck(int argc, char **argv)
{

    if (checkArgsNum(argc, 4, CHECK_ERROR_MSG))
    {
        return EXIT_FAILURE;
    }

    FILE* readingFile = fopen(argv[2], READ_MODE);
    FILE* writingFile = fopen(argv[3], READ_MODE);
    if (readingFile == NULL || writingFile == NULL)
    {
        fprintf(stderr, "%s", INVALID_FILE_MSG);    // todo: close?
        return EXIT_FAILURE;
    }

    char origin[MAX_TEXT_SIZE];
    char encrypted[MAX_TEXT_SIZE];

    fread(origin, sizeof(char), MAX_TEXT_SIZE, readingFile);
    fread(encrypted, sizeof(char), MAX_TEXT_SIZE, writingFile);
//    fscanf(readingFile, "%s", origin);
//    fscanf(writingFile, "%s", encrypted);
    check(origin, encrypted);
    fclose(readingFile);
    fclose(writingFile);
    return EXIT_SUCCESS;
}



void encodeCase(FILE* readingFile, FILE* writingFile, int k)
{
    if (k >= 0)
    {
        encode(readingFile, writingFile, k);
    } else
    {
        decode(readingFile, writingFile, -k);
    }
}


void decodeCase(FILE* readingFile, FILE* writingFile, int k)
{
    if (k >= 0)
    {
        decode(readingFile, writingFile, k);
    } else
    {
        encode(readingFile, writingFile, -k);
    }
}



int checkArgsNum(int argNum, int rightArgsNum, const char* msg)
{
    if (argNum != rightArgsNum)
    {
        fprintf(stderr, "%s", msg);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}



void encode(FILE* readingFile, FILE* writingFile, int k)
{
    char c;
    while ((c = fgetc(readingFile)) != EOF)
    {
        c = encodeChar(c, k);
        fputc(c, writingFile);
    }
}


void decode(FILE* readingFile, FILE* writingFile, int k)
{
    char c;
    while ((c = fgetc(readingFile)) != EOF)
    {
        c = decodeChar(c, k);
        fputc(c, writingFile);
    }
}

char encodeChar(char c, int k)
{
    int firstInGap = 0;
    if ((LOWER_CASE_S_RANGE <= c && c <= LOWER_CASE_E_RANGE) || (UPPER_CASE_S_RANGE <= c && c <= UPPER_CASE_E_RANGE))
    {
        if (LOWER_CASE_S_RANGE <= c)
        {
            firstInGap = LOWER_CASE_S_RANGE;

        } else
        {
            firstInGap = UPPER_CASE_S_RANGE;
        }
        c = (c - firstInGap + k) % LETTERS_NUM + firstInGap;
    }
    return c;
}


char decodeChar(char c, int k)
{
    k = -k;
    MOD(k);
//    k >= 0 ? (k = k) : (k = LETTERS_NUM + k);
    return encodeChar(c, k);
}


void check(char *origin, char *encoded) {
    if (strlen(origin) != strlen(encoded))
    {
        fprintf(stdout, "%s", INVALID_ENCRYPTING_MSG);
        return;
    }

    int i = 0;
    int k = encoded[0] - origin[0] ;
    MOD(k);
//    k >= 0 ? (k = k) : (k = LETTERS_NUM + k);
    while (origin[i] != '\0')
    {
        if (encodeChar(origin[i], k) != encoded[i])
        {
            fprintf(stdout, "%s", INVALID_ENCRYPTING_MSG);
            return;
        }
        i++;
    }

    fprintf(stdout, VALID_ENCRYPTING_MSG, k);
    return;
}

int tests()
{
    int x;
    char* argv[5];

    char* read = "C:\\Users\\miris\\CLionProjects\\ex1\\read";
    char* write = "C:\\Users\\miris\\CLionProjects\\ex1\\write";


    fprintf(stderr, "~~~~~~~~~0~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  The given command is invalid\n");
    char* ar[1];

    x = checkCommand(1, ar);
    assert(x != 0); //    "The given command is invalid\n"

    fprintf(stderr, "~~~~~~~~~1~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  The given command is invalid\n");

    argv[1] = "m";
    argv[2] = "3";
    argv[3] = read;
    argv[4] = write;
    x = checkCommand(5, argv);
    assert(x != 0); //    "The given command is invalid\n"

    fprintf(stderr, "~~~~~~~2~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  The given command is invalid\n");

    char* argv2[4];

    argv2[1] = "m";
    argv2[2] = read;
    argv2[3] = write;
    x = checkCommand(4, argv2);
    assert(x != 0); //    "The given command is invalid\n"


    fprintf(stderr, "~~~~~~~~~~~~~~3~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Usage: cipher <encode|decode> <k> <source path file> <output path file>\n");

    argv2[1] = "encode";
    argv2[2] = read;
    argv2[3] = write;
    x = checkCommand(4, argv2);
    assert(x != 0); //   "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n"


    fprintf(stderr, "~~~~~~~~~4~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Usage: cipher <encode|decode> <k> <source path file> <output path file>\n");

    char* argv5[6];
    argv5[1] = "encode";
    argv5[2] = "2";
    argv5[3] = read;
    argv5[4] = write;
    argv5[5] = "5";
    x = checkCommand(6, argv5);
    assert(x != 0); //    "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n"


    fprintf(stderr, "~~~~~~~5~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Usage: cipher <encode|decode> <k> <source path file> <output path file>\n");


    argv2[1] = "decode";
    argv2[2] = read;
    argv2[3] = write;
    x = checkCommand(4, argv2);
    assert(x != 0); //   "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n"


    fprintf(stderr, "~~~~~~~~~6~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Usage: cipher <encode|decode> <k> <source path file> <output path file>\n");

    argv5[1] = "decode";
    argv5[2] = "2";
    argv5[3] = read;
    argv5[4] = write;
    argv5[5] = "5";
    x = checkCommand(6, argv5);
    assert(x != 0); //    "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n"

    fprintf(stderr, "~~~~~~~~~7~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  \"Usage: cipher <check> <source path file> <output path file>\n");

    char* argv7[3];
    argv7[1] = "check";
    argv7[2] = read;
    x = checkCommand(3, argv7);
    assert(x != 0); // "Usage: cipher <check> <source path file> <output path file>\n"

    fprintf(stderr, "~~~~~~~~~8~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Usage: cipher <check> <source path file> <output path file>\n");

    argv5[1] = "check";
    argv5[2] = "2";
    argv5[3] = read;
    argv5[4] = write;
    argv5[5] = "5";
    x = checkCommand(6, argv5);
    assert(x != 0); // "Usage: cipher <check> <source path file> <output path file>\n"

//    fprintf(stderr, "~~~~~~~~~9~~~~~~~~~~~~\n");
//    fprintf(stderr, "should:  The given shifts value is invalid\n");
//
//    argv[1] = "encode";
//    argv[2] = "0";
//    argv[3] = read;
//    argv[4] = write;
//    x = checkCommand(5, argv);
//    assert(x == 0); //    "The given shifts value is invalid\n"

//    fprintf(stderr, "~~~~~~~~~9~~~~~~~~~~~~\n");
//    fprintf(stderr, "should:  The given shifts value is invalid\n");
//
//    argv[1] = "decode";
//    argv[2] = "0";
//    argv[3] = read;
//    argv[4] = write;
//    x = checkCommand(5, argv);
//    assert(x != 0); //    "The given shifts value is invalid\n"

    fprintf(stderr, "~~~~~~~~~10~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  The given file is invalid\n");

    argv[1] = "encode";
    argv[2] = "2";
    argv[3] = "C:\\Users\\miris\\CLionProjects\\ex1\\readd";
    argv[4] = write;
    x = checkCommand(5, argv);
    assert(x != 0); //   "The given file is invalid\n"

    fprintf(stderr, "~~~~~~~~~11~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  The given file is invalid\n");

    argv[1] = "decode";
    argv[2] = "2";
    argv[3] = "C:\\Users\\miris\\CLionProjects\\ex1\\readd";
    argv[4] = write;
    x = checkCommand(5, argv);
    assert(x != 0); //   "The given file is invalid\n"


    fprintf(stderr, "~~~~~~~12~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  The given file is invalid\n");

    argv2[1] = "check";
    argv2[2] = "C:\\Users\\miris\\CLionProjects\\ex1\\readd";
    argv2[3] = write;
    x = checkCommand(4, argv2);
    assert(x != 0); //    "The given file is invalid\n"

    fprintf(stderr, "~~~~~~~13~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  The given file is invalid\n");

    argv2[1] = "check";
    argv2[2] = read;
    argv2[3] = "C:\\Users\\miris\\CLionProjects\\ex1\\writee";
    x = checkCommand(4, argv2);
    assert(x != 0); //    "The given file is invalid\n"


//    ############################################################################
//    valid
//    ############################################################################


    fprintf(stderr, "\n\n##########   valid   ###########\n\n");

    char* readA = "C:\\Users\\miris\\CLionProjects\\ex1\\readA";
    char* readZ = "C:\\Users\\miris\\CLionProjects\\ex1\\readZ";

    fprintf(stderr, "~~~~~~~~~1~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  \n");

    argv[1] = "encode";
    argv[2] = "1";
    argv[3] = readA;
    argv[4] = write;
    x = checkCommand(5, argv);
    char writeRes[10];
    FILE* writeFile = fopen(write, READ_MODE);
    fread(writeRes, sizeof(char), 10, writeFile);
    fclose(writeFile);
    assert(x == 0);
    assert(strcmp(writeRes, "b nLA!a") == 0);




    fprintf(stderr, "~~~~~~~~~2~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  \n");

    argv[1] = "encode";
    argv[2] = "-1";
    argv[3] = readA;
    argv[4] = write;
    x = checkCommand(5, argv);
    writeFile = fopen(write, READ_MODE);
    fread(writeRes, sizeof(char), 10, writeFile);
    fclose(writeFile);
    assert(x == 0);
    assert(strcmp(writeRes, "z lJY!y") == 0);


    fprintf(stderr, "~~~~~~~~~3~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  \n");

    argv[1] = "encode";
    argv[2] = "27";
    argv[3] = readA;
    argv[4] = write;
    x = checkCommand(5, argv);
    writeFile = fopen(write, READ_MODE);
    fread(writeRes, sizeof(char), 10, writeFile);
    fclose(writeFile);
    assert(x == 0);
    assert(strcmp(writeRes, "b nLA!a") == 0);


    fprintf(stderr, "~~~~~~~~~4~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  \n");

    argv[1] = "decode";
    argv[2] = "-1";
    argv[3] = readA;
    argv[4] = write;
    x = checkCommand(5, argv);
    writeFile = fopen(write, READ_MODE);
    fread(writeRes, sizeof(char), 10, writeFile);
    fclose(writeFile);
    assert(x == 0);
    assert(strcmp(writeRes, "b nLA!a") == 0);

    fprintf(stderr, "~~~~~~~~~5~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  \n");

    argv[1] = "decode";
    argv[2] = "1";
    argv[3] = readA;
    argv[4] = write;
    x = checkCommand(5, argv);
    writeFile = fopen(write, READ_MODE);
    fread(writeRes, sizeof(char), 10, writeFile);
    fclose(writeFile);
    assert(x == 0);
    assert(strcmp(writeRes, "z lJY!y") == 0);

    char* readB = "C:\\Users\\miris\\CLionProjects\\ex1\\readB";

    fprintf(stderr, "~~~~~~~6~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Valid encrypting with k = 1\n");
    argv2[1] = "check";
    argv2[2] = readA;
    argv2[3] = readB;
    x = checkCommand(4, argv2);
    assert(x == 0); //    Valid encrypting with k = 1


    fprintf(stderr, "~~~~~~~7~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Valid encrypting with k = 25\n");
    argv2[1] = "check";
    argv2[2] = readA;
    argv2[3] = readZ;
    x = checkCommand(4, argv2);
    assert(x == 0);


    fprintf(stderr, "~~~~~~~8~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Valid encrypting with k = 1\n");
    argv2[1] = "check";
    argv2[2] = readZ;
    argv2[3] = readA;
    x = checkCommand(4, argv2);
    assert(x == 0);

    char* invalidReadZ = "C:\\Users\\miris\\CLionProjects\\ex1\\invalidReadZ";
    char* shortInvalidReadZ = "C:\\Users\\miris\\CLionProjects\\ex1\\shortInvalidZ";

    fprintf(stderr, "~~~~~~~9~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Invalid encrypting\n");
    argv2[1] = "check";
    argv2[2] = readA;
    argv2[3] = invalidReadZ;
    x = checkCommand(4, argv2);
    assert(x == 0);

    fprintf(stderr, "~~~~~~~10~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Invalid encrypting\n");
    argv2[1] = "check";
    argv2[2] = readA;
    argv2[3] = shortInvalidReadZ;
    x = checkCommand(4, argv2);
    assert(x == 0);

    fprintf(stderr, "~~~~~~~~~11~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  \n");

    argv[1] = "encode";
    argv[2] = "0";
    argv[3] = readA;
    argv[4] = write;
    x = checkCommand(5, argv);
    writeFile = fopen(write, READ_MODE);
    fread(writeRes, sizeof(char), 10, writeFile);
    fclose(writeFile);
    assert(x == 0);
    assert(strcmp(writeRes, "a mKZ!z") == 0);

    fprintf(stderr, "~~~~~~~~~12~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  \n");

    argv[1] = "decode";
    argv[2] = "0";
    argv[3] = readA;
    argv[4] = write;
    x = checkCommand(5, argv);
    writeFile = fopen(write, READ_MODE);
    fread(writeRes, sizeof(char), 10, writeFile);
    fclose(writeFile);
    assert(x == 0);
    assert(strcmp(writeRes, "a mKZ!z") == 0);


//    Valid encrypting with k = 1
//    Valid encrypting with k = 25
//    Valid encrypting with k = 1
//    Invalid encrypting
//    Invalid encrypting



    return EXIT_SUCCESS;
}
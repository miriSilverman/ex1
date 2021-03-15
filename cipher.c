//
// Created by miris on 01/03/2021.
//


#include <stdio.h>
#include <stdlib.h>
#include "cipher.h"
#include <string.h>
#include <assert.h>
#include <math.h>

#define MOD(k) (k) >= 0 ? ((k) = (k)) : ((k) = LETTERS_NUM + (k))

const char* CHECK_ERROR_MSG = "Usage: cipher <check> <source path file> <output path file>\n";
const char* ENCODE_ERROR_MSG = "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n";
const char* INVALID_FILE_MSG = "The given file is invalid\n";
const char* INVALID_COMMAND_MSG = "The given command is invalid\n";
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
const int COMMAND = 1;
const int IS_ENCODE = 1;
const int IS_DECODE = 0;
const int ARGS_NUM_CHECK = 4;
const int ARGS_NUM_ENCODE = 5;
const int K = 2;
const int READING_FILE = 3;
const int WRITING_FILE = 4;
const int ORIGIN_FILE = 2;
const int ENCRYPTED_FILE = 3;




int main(int argc, char* argv[])
{
//    printf("%d\n", isLetter('Z'));
    return tests();
//    return checkCommand(argc, argv);
}



/**
 * Checks that the the command is valid.
 * @param argc number of arguments
 * @param argv the arguments
 * @return EXIT_FAILURE if the command is invalid or if there is a different failure in the program.
 * otherwise EXIT_SUCCESS.
 */
int checkCommand(int argc, char* argv[])
{
    char* command = argv[COMMAND];
    if (command == NULL)
    {
        fprintf(stderr, "%s", INVALID_COMMAND_MSG);
        return EXIT_FAILURE;
    }

    if (!strcmp(command, ENCODE_COMMAND))
    {
        return checkEncode(argc, argv, IS_ENCODE);
    }
    else if(!strcmp(command, DECODE_COMMAND))
    {
        return checkEncode(argc, argv, IS_DECODE);
    }
    else if (!strcmp(command, CHECK_COMMAND))
    {
        return checkCommandCheck(argc, argv);
    }

    fprintf(stderr, "%s", INVALID_COMMAND_MSG);
    return EXIT_FAILURE;
}

/**
 * Checks encode and decode commands
 * @param argc number of arguments
 * @param argv the arguments
 * @param isEncode 1 if the command is encode, 0 if decode
 * @return EXIT_FAILURE if the command line is invalid or if there is a problem with the files,
 * otherwise EXIT_SUCCESS
 */
int checkEncode(int argc, char* argv[], int isEncode)
{
    if (checkArgsNum(argc, ARGS_NUM_ENCODE, ENCODE_ERROR_MSG))
    {
        return EXIT_FAILURE;
    }

    int k = atoi(argv[K]);

    FILE* readingFile = fopen(argv[READING_FILE], READ_MODE);
    FILE* writingFile = fopen(argv[WRITING_FILE], WRITE_MODE);
    if (readingFile == NULL)
    {
        fprintf(stderr, "%s", INVALID_FILE_MSG);
        return EXIT_FAILURE;
    }

    isEncode ? (k = k) : (k = -k);
    MOD(k);
    encode(readingFile, writingFile, k);

    fclose(readingFile);
    fclose(writingFile);

    return EXIT_SUCCESS;
}

/**
 * Checks if the command check is valid
 * @param argc number of arguments
 * @param argv the arguments
 * @return EXIT_FAILURE if the command line is invalid or if there is a problem with the files,
 * otherwise EXIT_SUCCESS
 */
int checkCommandCheck(int argc, char **argv)
{

    if (checkArgsNum(argc, ARGS_NUM_CHECK, CHECK_ERROR_MSG))
    {
        return EXIT_FAILURE;
    }

    FILE* readingFile = fopen(argv[ORIGIN_FILE], READ_MODE);
    FILE* writingFile = fopen(argv[ENCRYPTED_FILE], READ_MODE);
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




/**
 * @param argNum number of arguments
 * @param rightArgsNum correct number of arguments
 * @param msg error message
 * @return EXIT_SUCCESS if argNum is equal to rightArgsNum, EXIT_FAILURE otherwise
 */
int checkArgsNum(int argNum, int rightArgsNum, const char* msg)
{
    if (argNum != rightArgsNum)
    {
        fprintf(stderr, "%s", msg);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/**
 * Encodes the content of the readingFile and writes it in writingFile
 * @param readingFile file to read from
 * @param writingFile file to write the encrypting to
 * @param k the offset parameter todo: not sure?
 */
void encode(const FILE* readingFile, FILE* writingFile, int k)
{
    char c;
    while ((c = fgetc(readingFile)) != EOF)
    {
        c = encodeChar(c, k);
        fputc(c, writingFile);
    }
}

/**
 * Encodes the char c according to caesar cipher with the offset parameter k
 * @param c char to encode
 * @param k offset parameter
 * @return the encrypted char for c
 */
char encodeChar(char c, int k)
{
    int firstInGap;
//    if ((LOWER_CASE_S_RANGE <= c && c <= LOWER_CASE_E_RANGE) || (UPPER_CASE_S_RANGE <= c && c <= UPPER_CASE_E_RANGE))
    if (isLetter(c))
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

/**
 * @param c  char
 * @return 1 if c is an english letter, 0 otherwise
 */
int isLetter(char c)
{
    return (LOWER_CASE_S_RANGE <= c && c <= LOWER_CASE_E_RANGE) || (UPPER_CASE_S_RANGE <= c && c <= UPPER_CASE_E_RANGE);
}

/**
 * Checks if the string encoded is a valid caesar cipher of the string origin, if it is
 * valid - prints the k, otherwise prints a message
 * @param origin string not encrypted
 * @param encoded encrypted string
 */
void check(char *origin, char *encoded) {
    if (strlen(origin) != strlen(encoded))
    {
        fprintf(stdout, "%s", INVALID_ENCRYPTING_MSG);
        return;
    }

    int i = 0;
    while (!isLetter(encoded[i]))
    {
        if (encoded[i] != origin[i])
        {
            fprintf(stdout, "%s", INVALID_ENCRYPTING_MSG);
            return;
        }
        i++;
    }

    int k = encoded[i] - origin[i] ;
    MOD(k);
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
}




int tests()
{
    int x;
    char* argv[5];

//    char* read = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/read";
//    char* write = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/write";
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
//    char* readA = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/readA";
//    char* readZ = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/readZ";

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
//    char* readB = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/readB";

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
//    char* invalidReadZ = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/invalidReadZ";
//    char* shortInvalidReadZ = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/shortInvalidZ";

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

    char* startNoLetter = "C:\\Users\\miris\\CLionProjects\\ex1\\startNL";
    char* startNoLetterOut = "C:\\Users\\miris\\CLionProjects\\ex1\\startNLout";
    char* startNoLetterNot = "C:\\Users\\miris\\CLionProjects\\ex1\\startNLnot";

    fprintf(stderr, "~~~~~~~13~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Valid encrypting with k = 1\n");
    argv2[1] = "check";
    argv2[2] = startNoLetter;
    argv2[3] = startNoLetterOut;
    x = checkCommand(4, argv2);
    assert(x == 0);

    fprintf(stderr, "~~~~~~~14~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Invalid encrypting\n");
    argv2[1] = "check";
    argv2[2] = startNoLetter;
    argv2[3] = startNoLetterNot;
    x = checkCommand(4, argv2);
    assert(x == 0);

    fprintf(stderr, "~~~~~~~15~~~~~~~~~~~~~~\n");
    fprintf(stderr, "should:  Valid encrypting with k = 0\n");
    argv2[1] = "check";
    argv2[2] = startNoLetter;
    argv2[3] = startNoLetter;
    x = checkCommand(4, argv2);
    assert(x == 0);

//    Valid encrypting with k = 1
//    Valid encrypting with k = 25
//    Valid encrypting with k = 1
//    Invalid encrypting
//    Invalid encrypting
//    Valid encrypting with k = 1
//    Invalid encrypting





    return EXIT_SUCCESS;
}
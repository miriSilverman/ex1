
#include <stdio.h>
#include <stdlib.h>
//#include "cipher.h"
#include <string.h>
#include <assert.h>
#include <math.h>

#define MOD(k) (k) >= 0 ? ((k) = (k)) : ((k) = LETTERS_NUM + (k))

#define CHECK_ERROR_MSG "Usage: cipher <check> <source path file> <output path file>\n"
#define ENCODE_ERROR_MSG "Usage: cipher <Encode|decode> <k> <source path file> <output path file>\n"
#define INVALID_FILE_MSG "The given file is invalid\n"
#define INVALID_COMMAND_MSG "The given command is invalid\n"
#define INVALID_ENCRYPTING_MSG  "Invalid encrypting\n"
#define VALID_ENCRYPTING_MSG "Valid encrypting with k = %d\n"
#define ENCODE_COMMAND "Encode"
#define DECODE_COMMAND "decode"
#define CHECK_COMMAND "check"
#define READ_MODE "r"
#define WRITE_MODE "w"
#define MAX_TEXT_SIZE 10000
#define LOWER_CASE_S_RANGE 97
#define LOWER_CASE_E_RANGE 122
#define UPPER_CASE_S_RANGE 65
#define UPPER_CASE_E_RANGE 90
#define LETTERS_NUM 26
#define COMMAND 1
#define IS_ENCODE 1
#define IS_DECODE 0
#define ARGS_NUM_CHECK 4
#define ARGS_NUM_ENCODE 5
#define K 2
#define READING_FILE 3
#define WRITING_FILE 4
#define ORIGIN_FILE 2
#define ENCRYPTED_FILE 3

int Tests ();    //todo: delete

int IsLetter (char c);
int CheckArgsNum (int argNum, int rightArgsNum, const char *msg);
int CheckEncode (int argc, char **argv, int isEncode);
int CheckCommand (int argc, char **argv);
int CheckCommandCheck (int argc, char **argv);

void Encode (FILE *readingFile, FILE *writingFile, int k);
void Check (char *origin, char *encoded);
char EncodeChar (char c, int k);

int main (int argc, char *argv[])
{
//    return tests();
    return CheckCommand (argc, argv);
}

/**
 * Checks that the the command is valid.
 * @param argc number of arguments
 * @param argv the arguments
 * @return EXIT_FAILURE if the command is invalid or if there is a different failure in the program.
 * otherwise EXIT_SUCCESS.
 */
int CheckCommand (int argc, char **argv)
{
    char *command = argv[COMMAND];
    if (command == NULL)
        {
            fprintf (stderr, "%s", INVALID_COMMAND_MSG);
            return EXIT_FAILURE;
        }

    if (!strcmp (command, ENCODE_COMMAND))
        {
            return CheckEncode (argc, argv, IS_ENCODE);
        }
    else if (!strcmp (command, DECODE_COMMAND))
        {
            return CheckEncode (argc, argv, IS_DECODE);
        }
    else if (!strcmp (command, CHECK_COMMAND))
        {
            return CheckCommandCheck (argc, argv);
        }

    fprintf (stderr, "%s", INVALID_COMMAND_MSG);
    return EXIT_FAILURE;
}

/**
 * Checks Encode and decode commands
 * @param argc number of arguments
 * @param argv the arguments
 * @param isEncode 1 if the command is Encode, 0 if decode
 * @return EXIT_FAILURE if the command line is invalid or if there is a problem with the files,
 * otherwise EXIT_SUCCESS
 */
int CheckEncode (int argc, char **argv, int isEncode)
{
    if (CheckArgsNum (argc, ARGS_NUM_ENCODE, ENCODE_ERROR_MSG))
        {
            return EXIT_FAILURE;
        }

    int k = atoi (argv[K]);

    FILE *readingFile = fopen (argv[READING_FILE], READ_MODE);
    FILE *writingFile = fopen (argv[WRITING_FILE], WRITE_MODE);
    if (readingFile == NULL)
        {
            fprintf (stderr, "%s", INVALID_FILE_MSG);
            return EXIT_FAILURE;
        }

    isEncode ? (k = k) : (k = -k);
    MOD(k);
    Encode (readingFile, writingFile, k);

    fclose (readingFile);
    fclose (writingFile);

    return EXIT_SUCCESS;
}

/**
 * Checks if the command check is valid
 * @param argc number of arguments
 * @param argv the arguments
 * @return EXIT_FAILURE if the command line is invalid or if there is a problem with the files,
 * otherwise EXIT_SUCCESS
 */
int CheckCommandCheck (int argc, char **argv)
{

    if (CheckArgsNum (argc, ARGS_NUM_CHECK, CHECK_ERROR_MSG))
        {
            return EXIT_FAILURE;
        }

    FILE *readingFile = fopen (argv[ORIGIN_FILE], READ_MODE);
    FILE *writingFile = fopen (argv[ENCRYPTED_FILE], READ_MODE);
    if (readingFile == NULL || writingFile == NULL)
        {
            fprintf (stderr, "%s", INVALID_FILE_MSG);    // todo: close?
            return EXIT_FAILURE;
        }

    char origin[MAX_TEXT_SIZE];
    char encrypted[MAX_TEXT_SIZE];

    fread (origin, sizeof (char), MAX_TEXT_SIZE, readingFile);
    fread (encrypted, sizeof (char), MAX_TEXT_SIZE, writingFile);
//    fscanf(readingFile, "%s", origin);
//    fscanf(writingFile, "%s", encrypted);
    Check (origin, encrypted);

    fclose (readingFile);
    fclose (writingFile);
    return EXIT_SUCCESS;
}

/**
 * @param argNum number of arguments
 * @param rightArgsNum correct number of arguments
 * @param msg error message
 * @return EXIT_SUCCESS if argNum is equal to rightArgsNum, EXIT_FAILURE otherwise
 */
int CheckArgsNum (int argNum, int rightArgsNum, const char *msg)
{
    if (argNum != rightArgsNum)
        {
            fprintf (stderr, "%s", msg);
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
void Encode (FILE *readingFile, FILE *writingFile, int k)
{
    char c;
    while ((c = fgetc (readingFile)) != EOF)
        {
            c = EncodeChar (c, k);
            fputc (c, writingFile);
        }
}

/**
 * Encodes the char c according to caesar cipher with the offset parameter k
 * @param c char to Encode
 * @param k offset parameter
 * @return the encrypted char for c
 */
char EncodeChar (char c, int k)
{
    int firstInGap;
    if (IsLetter (c))
        {
            if (LOWER_CASE_S_RANGE <= c)
                {
                    firstInGap = LOWER_CASE_S_RANGE;

                }
            else
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
int IsLetter (char c)
{
    return (LOWER_CASE_S_RANGE <= c && c <= LOWER_CASE_E_RANGE) || (UPPER_CASE_S_RANGE <= c && c <= UPPER_CASE_E_RANGE);
}

/**
 * Checks if the string encoded is a valid caesar cipher of the string origin, if it is
 * valid - prints the k, otherwise prints a message
 * @param origin string not encrypted
 * @param encoded encrypted string
 */
void Check (char *origin, char *encoded)
{
    if (strlen (origin) != strlen (encoded))
        {
            fprintf (stdout, "%s", INVALID_ENCRYPTING_MSG);
            return;
        }

    int i = 0;
    while (!IsLetter (encoded[i]))
        {
            if (encoded[i] != origin[i])
                {
                    fprintf (stdout, "%s", INVALID_ENCRYPTING_MSG);
                    return;
                }
            i++;
        }

    int k = encoded[i] - origin[i];
    MOD(k);
    while (origin[i] != '\0')
        {
            if (EncodeChar (origin[i], k) != encoded[i])
                {
                    fprintf (stdout, "%s", INVALID_ENCRYPTING_MSG);
                    return;
                }
            i++;
        }

    fprintf (stdout, VALID_ENCRYPTING_MSG, k);
}

int Tests ()
{
    int x;
    char *argv[5];

//    char* read = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/read";
//    char* write = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/write";
    char *read = "C:\\Users\\miris\\CLionProjects\\ex1\\read";
    char *write = "C:\\Users\\miris\\CLionProjects\\ex1\\write";

    fprintf (stderr, "~~~~~~~~~0~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  The given command is invalid\n");
    char *ar[1];

    x = CheckCommand (1, ar);
    assert (x != 0); //    "The given command is invalid\n"

    fprintf (stderr, "~~~~~~~~~1~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  The given command is invalid\n");

    argv[1] = "m";
    argv[2] = "3";
    argv[3] = read;
    argv[4] = write;
    x = CheckCommand (5, argv);
    assert (x != 0); //    "The given command is invalid\n"

    fprintf (stderr, "~~~~~~~2~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  The given command is invalid\n");

    char *argv2[4];

    argv2[1] = "m";
    argv2[2] = read;
    argv2[3] = write;
    x = CheckCommand (4, argv2);
    assert (x != 0); //    "The given command is invalid\n"


    fprintf (stderr, "~~~~~~~~~~~~~~3~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Usage: cipher <Encode|decode> <k> <source path file> <output path file>\n");

    argv2[1] = "Encode";
    argv2[2] = read;
    argv2[3] = write;
    x = CheckCommand (4, argv2);
    assert (x != 0); //   "Usage: cipher <Encode|decode> <k> <source path file> <output path file>\n"


    fprintf (stderr, "~~~~~~~~~4~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Usage: cipher <Encode|decode> <k> <source path file> <output path file>\n");

    char *argv5[6];
    argv5[1] = "Encode";
    argv5[2] = "2";
    argv5[3] = read;
    argv5[4] = write;
    argv5[5] = "5";
    x = CheckCommand (6, argv5);
    assert (x != 0); //    "Usage: cipher <Encode|decode> <k> <source path file> <output path file>\n"


    fprintf (stderr, "~~~~~~~5~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Usage: cipher <Encode|decode> <k> <source path file> <output path file>\n");

    argv2[1] = "decode";
    argv2[2] = read;
    argv2[3] = write;
    x = CheckCommand (4, argv2);
    assert (x != 0); //   "Usage: cipher <Encode|decode> <k> <source path file> <output path file>\n"


    fprintf (stderr, "~~~~~~~~~6~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Usage: cipher <Encode|decode> <k> <source path file> <output path file>\n");

    argv5[1] = "decode";
    argv5[2] = "2";
    argv5[3] = read;
    argv5[4] = write;
    argv5[5] = "5";
    x = CheckCommand (6, argv5);
    assert (x != 0); //    "Usage: cipher <Encode|decode> <k> <source path file> <output path file>\n"

    fprintf (stderr, "~~~~~~~~~7~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  \"Usage: cipher <check> <source path file> <output path file>\n");

    char *argv7[3];
    argv7[1] = "check";
    argv7[2] = read;
    x = CheckCommand (3, argv7);
    assert (x != 0); // "Usage: cipher <check> <source path file> <output path file>\n"

    fprintf (stderr, "~~~~~~~~~8~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Usage: cipher <check> <source path file> <output path file>\n");

    argv5[1] = "check";
    argv5[2] = "2";
    argv5[3] = read;
    argv5[4] = write;
    argv5[5] = "5";
    x = CheckCommand (6, argv5);
    assert (x != 0); // "Usage: cipher <check> <source path file> <output path file>\n"



    fprintf (stderr, "~~~~~~~~~9~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  The given file is invalid\n");

    argv[1] = "Encode";
    argv[2] = "2";
    argv[3] = "C:\\Users\\miris\\CLionProjects\\ex1\\readd";
    argv[4] = write;
    x = CheckCommand (5, argv);
    assert (x != 0); //   "The given file is invalid\n"

    fprintf (stderr, "~~~~~~~~~10~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  The given file is invalid\n");

    argv[1] = "decode";
    argv[2] = "2";
    argv[3] = "C:\\Users\\miris\\CLionProjects\\ex1\\readd";
    argv[4] = write;
    x = CheckCommand (5, argv);
    assert (x != 0); //   "The given file is invalid\n"


    fprintf (stderr, "~~~~~~~11~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  The given file is invalid\n");

    argv2[1] = "check";
    argv2[2] = "C:\\Users\\miris\\CLionProjects\\ex1\\readd";
    argv2[3] = write;
    x = CheckCommand (4, argv2);
    assert (x != 0); //    "The given file is invalid\n"

    fprintf (stderr, "~~~~~~~12~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  The given file is invalid\n");

    argv2[1] = "check";
    argv2[2] = read;
    argv2[3] = "C:\\Users\\miris\\CLionProjects\\ex1\\writee";
    x = CheckCommand (4, argv2);
    assert (x != 0); //    "The given file is invalid\n"


//    ############################################################################
//    valid
//    ############################################################################


    fprintf (stderr, "\n\n##########   valid   ###########\n\n");

    char *readA = "C:\\Users\\miris\\CLionProjects\\ex1\\readA";
    char *readZ = "C:\\Users\\miris\\CLionProjects\\ex1\\readZ";
//    char* readA = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/readA";
//    char* readZ = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/readZ";

    fprintf (stderr, "~~~~~~~~~13~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  \n");

    argv[1] = "Encode";
    argv[2] = "1";
    argv[3] = readA;
    argv[4] = write;
    x = CheckCommand (5, argv);
    char writeRes[10];
    FILE *writeFile = fopen (write, READ_MODE);
    fread (writeRes, sizeof (char), 10, writeFile);
    fclose (writeFile);
    assert (x == 0);
    assert (strcmp (writeRes, "b nLA!a") == 0);

    fprintf (stderr, "~~~~~~~~~14~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  \n");

    argv[1] = "Encode";
    argv[2] = "-1";
    argv[3] = readA;
    argv[4] = write;
    x = CheckCommand (5, argv);
    writeFile = fopen (write, READ_MODE);
    fread (writeRes, sizeof (char), 10, writeFile);
    fclose (writeFile);
    assert (x == 0);
    assert (strcmp (writeRes, "z lJY!y") == 0);

    fprintf (stderr, "~~~~~~~~~15~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  \n");

    argv[1] = "Encode";
    argv[2] = "27";
    argv[3] = readA;
    argv[4] = write;
    x = CheckCommand (5, argv);
    writeFile = fopen (write, READ_MODE);
    fread (writeRes, sizeof (char), 10, writeFile);
    fclose (writeFile);
    assert (x == 0);
    assert (strcmp (writeRes, "b nLA!a") == 0);

    fprintf (stderr, "~~~~~~~~~16~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  \n");

    argv[1] = "decode";
    argv[2] = "-1";
    argv[3] = readA;
    argv[4] = write;
    x = CheckCommand (5, argv);
    writeFile = fopen (write, READ_MODE);
    fread (writeRes, sizeof (char), 10, writeFile);
    fclose (writeFile);
    assert (x == 0);
    assert (strcmp (writeRes, "b nLA!a") == 0);

    fprintf (stderr, "~~~~~~~~~17~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  \n");

    argv[1] = "decode";
    argv[2] = "1";
    argv[3] = readA;
    argv[4] = write;
    x = CheckCommand (5, argv);
    writeFile = fopen (write, READ_MODE);
    fread (writeRes, sizeof (char), 10, writeFile);
    fclose (writeFile);
    assert (x == 0);
    assert (strcmp (writeRes, "z lJY!y") == 0);

    char *readB = "C:\\Users\\miris\\CLionProjects\\ex1\\readB";
//    char* readB = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/readB";

    fprintf (stderr, "~~~~~~~18~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Valid encrypting with k = 1\n");
    argv2[1] = "check";
    argv2[2] = readA;
    argv2[3] = readB;
    x = CheckCommand (4, argv2);
    assert (x == 0); //    Valid encrypting with k = 1


    fprintf (stderr, "~~~~~~~19~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Valid encrypting with k = 25\n");
    argv2[1] = "check";
    argv2[2] = readA;
    argv2[3] = readZ;
    x = CheckCommand (4, argv2);
    assert (x == 0);

    fprintf (stderr, "~~~~~~~20~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Valid encrypting with k = 1\n");
    argv2[1] = "check";
    argv2[2] = readZ;
    argv2[3] = readA;
    x = CheckCommand (4, argv2);
    assert (x == 0);

    char *invalidReadZ = "C:\\Users\\miris\\CLionProjects\\ex1\\invalidReadZ";
    char *shortInvalidReadZ = "C:\\Users\\miris\\CLionProjects\\ex1\\shortInvalidZ";
//    char* invalidReadZ = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/invalidReadZ";
//    char* shortInvalidReadZ = "/cs/usr/miri_silverman/CLionProjects/ex1CTA/shortInvalidZ";

    fprintf (stderr, "~~~~~~~21~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Invalid encrypting\n");
    argv2[1] = "check";
    argv2[2] = readA;
    argv2[3] = invalidReadZ;
    x = CheckCommand (4, argv2);
    assert (x == 0);

    fprintf (stderr, "~~~~~~~22~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Invalid encrypting\n");
    argv2[1] = "check";
    argv2[2] = readA;
    argv2[3] = shortInvalidReadZ;
    x = CheckCommand (4, argv2);
    assert (x == 0);

    fprintf (stderr, "~~~~~~~~~23~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  \n");

    argv[1] = "Encode";
    argv[2] = "0";
    argv[3] = readA;
    argv[4] = write;
    x = CheckCommand (5, argv);
    writeFile = fopen (write, READ_MODE);
    fread (writeRes, sizeof (char), 10, writeFile);
    fclose (writeFile);
    assert (x == 0);
    assert (strcmp (writeRes, "a mKZ!z") == 0);

    fprintf (stderr, "~~~~~~~~~24~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  \n");

    argv[1] = "decode";
    argv[2] = "0";
    argv[3] = readA;
    argv[4] = write;
    x = CheckCommand (5, argv);
    writeFile = fopen (write, READ_MODE);
    fread (writeRes, sizeof (char), 10, writeFile);
    fclose (writeFile);
    assert (x == 0);
    assert (strcmp (writeRes, "a mKZ!z") == 0);

    char *startNoLetter = "C:\\Users\\miris\\CLionProjects\\ex1\\startNL";
    char *startNoLetterOut = "C:\\Users\\miris\\CLionProjects\\ex1\\startNLout";
    char *startNoLetterNot = "C:\\Users\\miris\\CLionProjects\\ex1\\startNLnot";

    fprintf (stderr, "~~~~~~~25~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Valid encrypting with k = 1\n");
    argv2[1] = "check";
    argv2[2] = startNoLetter;
    argv2[3] = startNoLetterOut;
    x = CheckCommand (4, argv2);
    assert (x == 0);

    fprintf (stderr, "~~~~~~~26~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Invalid encrypting\n");
    argv2[1] = "check";
    argv2[2] = startNoLetter;
    argv2[3] = startNoLetterNot;
    x = CheckCommand (4, argv2);
    assert (x == 0);

    fprintf (stderr, "~~~~~~~27~~~~~~~~~~~~~~\n");
    fprintf (stderr, "should:  Valid encrypting with k = 0\n");
    argv2[1] = "check";
    argv2[2] = startNoLetter;
    argv2[3] = startNoLetter;
    x = CheckCommand (4, argv2);
    assert (x == 0);

//    Valid encrypting with k = 1
//    Valid encrypting with k = 25
//    Valid encrypting with k = 1
//    Invalid encrypting
//    Invalid encrypting
//    Valid encrypting with k = 1
//    Invalid encrypting
//    Valid encrypting with k = 0





    return EXIT_SUCCESS;
}


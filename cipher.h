//
// Created by miris on 01/03/2021.
//

#ifndef EX1_CIPHER_H
#define EX1_CIPHER_H

int tests();    //todo: delete


int checkArgsNum(int argNum,  int rightArgsNum, const char* msg);
int checkEncode(int argc, char* argv[], int isEncode);
int checkCommand(int argc, char* argv[]);
int checkCommandCheck(int argc, char **argv);

void encode(const FILE* readingFile, FILE* writingFile, int k);
void check(char *origin, char *encoded);
char encodeChar(char c, int k);


#endif //EX1_CIPHER_H

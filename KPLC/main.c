/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "parser.h"
#include "codegen.h"

int dumpCode = 0;

void printUsage(void)
{
  printf("Usage: kplc input output [-dump]\n");
  printf("   input: input kpl program\n");
  printf("   output: executable\n");
  printf("   -dump: code dump\n");
}

int analyseParam(char *param)
{
  if (strcmp(param, "-dump") == 0)
  {
    dumpCode = 1;
    return 1;
  }
  return 0;
}

/******************************************************************/

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    printf("kplc: no input file.\n");
    printf("Usage: KplC input.kpl\n");
    return -1;
  }

  initCodeBuffer();

  if (compile(argv[1]) == IO_ERROR)
  {
    printf("\n\nCan\'t read input file!\n");
    return -1;
  }

  printf("\n\n--- Assembly Code ---\n");
  printCodeBuffer();

  serialize("a.out");

  cleanCodeBuffer();

  return 0;
}

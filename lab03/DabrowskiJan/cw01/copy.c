#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int copy()
{
  int inputFile = open("./texts/text_copy.txt", O_RDONLY);
  if (inputFile == -1) {
    printf("Error with input file!");
    return 1;
  }
  int outputFile = open("./results/reversed_copy.txt", O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR );
  off_t fileSize = lseek(inputFile, 0, SEEK_END);
  if (fileSize == -1){
    close(inputFile);
    printf("Error - file empty");
    return 1;
  }
  off_t offset  = -1;
  char c;
  while(lseek(inputFile,offset,SEEK_END) != -1 && read(inputFile, &c, 1) == 1){
    write(outputFile, &c, 1);
    offset--;
  }
  close(inputFile);
  close(outputFile);
  return 0;

}


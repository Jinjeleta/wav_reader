#include <stdio.h>

int main()
{
    // Application simply decodes wav files
    printf("This is C application written by Nerijus\n");
    
    FILE *fp;
    
    fp = open("sample.wav", "r");
    
    fclose(fp);
    printf("Program is finished.\n");
    
    return 0;
}
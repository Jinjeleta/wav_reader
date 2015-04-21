#include <stdio.h>
#include <stdlib.h>

unsigned short char2short(unsigned char *buff);
unsigned int char2int(unsigned char *buff);

int main()
{
    // Application simply decodes wav files
    printf("This is C application written by Nerijus\n");
    
    //Local variables
    char fileName[] = "sample.wav";
    long fileSize = 0;
    size_t result; // will store how many bytes have been red or written
    FILE *fp;
    unsigned char *buffer; // Buffer pointer, will be allocated dynamically
    //wave file format header
    char ckId[4];
    char ckSize[4];
    char waveId[4];
    unsigned int test;
    
    
    //Open file and check for an error
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open file: '%s'!\n", fileName);
    }
    
    //Obtain file size
    fseek(fp, 0, SEEK_END);     //Put file reader pointer to the very end
    fileSize = ftell(fp);       //Check current reader pointer position
    rewind(fp);                 //Return reader pointer to the beginning of the file
    printf("File size: %d bytes.\n", fileSize);
    
    //Dynamic buffer allocation
    buffer = (char*) malloc(sizeof(char) * fileSize);
    if(buffer == NULL) fprintf(stderr, "Failed to allocate memory of %d bytes!", fileSize);
    
    //----------------------------------------------------------------------
    //Read first chunk of data file
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("Chunk ID: %s \n", buffer);
    
    //Read first chunk of data file
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    //test = (unsigned int)buffer[0] | (((unsigned int)buffer[1]) << 8) | (((unsigned int)buffer[2]) << 16) | (((unsigned int)buffer[3]) << 24);
    printf("Chunk size: %d\n", char2int(buffer));
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("Wave ID: %s \n", buffer);
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("Chunk ID: %s \n", buffer);
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("Chunk size: %d\n", char2int(buffer));
    
    result = fread(buffer, 1, 2, fp);
    if(result != 2) fprintf(stderr, "Failed reading file!\n");
    printf("wFormatTag: %d\n", char2short(buffer));
    
    result = fread(buffer, 1, 2, fp);
    if(result != 2) fprintf(stderr, "Failed reading file!\n");
    printf("Number of channels: %d \n", char2short(buffer));
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("Samples per sec: %d \n", char2int(buffer));
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("AvgBytes per sec: %d \n", char2int(buffer));
    
    result = fread(buffer, 1, 2, fp);
    if(result != 2) fprintf(stderr, "Failed reading file!\n");
    printf("nBlockAlign: %d \n", char2short(buffer));
    
    result = fread(buffer, 1, 2, fp);
    if(result != 2) fprintf(stderr, "Failed reading file!\n");
    printf("wBitsPerSample: %d \n", buffer[0]);
    
    result = fread(buffer, 1, 2, fp);
    if(result != 2) fprintf(stderr, "Failed reading file!\n");
    printf("cbSize: %d \n", char2short(buffer));
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("ckID: %s \n", buffer);
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("cksize: %d \n", char2int(buffer));
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("dwSampleLength: %d \n", char2int(buffer));
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("ckID: %s \n", buffer);
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) fprintf(stderr, "Failed reading file!\n");
    printf("cksize: %d \n", char2int(buffer));
    
    
    
    //-----------------------------------------------------------------------
    
    //Close the file and free dynamically allocated buffer
    fclose(fp);
    free(buffer);
    
    //Last message
    printf("Program is finished.\n");
    printf("Size of int: %d\n", sizeof(int));
    printf("Size of short int: %d\n", sizeof(short int));
    
    return 0;
}

unsigned short char2short(unsigned char *buff)
{
    unsigned short int result = ((unsigned short)buff[0]) | (((unsigned short)buff[1]) << 8);
    return result;
}
unsigned int char2int(unsigned char *buff)
{
    unsigned int result = ((unsigned short)buff[0]) | (((unsigned int)buff[1]) << 8) | (((unsigned int)buff[2]) << 16) | (((unsigned int)buff[3]) << 24);
    return result;   
}
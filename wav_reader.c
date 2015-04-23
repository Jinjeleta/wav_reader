//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Header files or external libraries
#include "wav_file.h"

int readWaveData(unsigned char *fileName, struct wav_file *waveData);
void printWaveData(struct wav_file *waveData);
float uint2float(unsigned int integer);
unsigned short char2short(unsigned char *buff);
unsigned int char2int(unsigned char *buff);

int main( int argc, char *argv[] )
{
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s filename\n", argv[0] );
    }
    else
    {
        // Application simply decodes wav files
        printf("This is C application written by Nerijus\n");
        
        //Local variables
        
        char fileName[] = "sample.wav";
        strcpy ( fileName, argv[1] );
        struct wav_file *waveData = malloc(sizeof *waveData);
        
        
        readWaveData(fileName, waveData);
        printWaveData(waveData);
        
        //printf("Struct cbSize: %d \n", waveData->cbSize);
        
        free(waveData->data);
        free(waveData);
        //Last message
        printf("Program is finished.\n");
    }
        
    
    return 0;
}

int readWaveData(unsigned char *fileName, struct wav_file *waveData)
{
    //some local variables
    FILE *fp;
    long fileSize;
    size_t result;
    unsigned char buffer[16];
    unsigned short int waveFormat; // 0x0001 - WAVE_FORMAT_PCM; 0x0003 - WAVE_FORMAT_IEEE_FLOAT; 0xFFFE - WAVE_FORMAT_EXTENSIBLE; otherwise unsupported
    
    //Open file and check for an error
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open file: '%s'!\n", fileName);
        fclose(fp);
        return -1;
    }
    
    //Obtain file size
    fseek(fp, 0, SEEK_END);     //Put file reader pointer to the very end
    fileSize = ftell(fp);       //Check current reader pointer position
    rewind(fp);                 //Return reader pointer to the beginning of the file
    
    printf("File size: %d bytes.\n", fileSize); //------------------DEBUG------------------
    
    // 1 - Check if it "RIFF" file
    // 2 - Check if it has "WAVE" format
    // 3 - Check if it has "fmt " chunk ID
    // 4 - Make decision based on wave format (PCM, nonPCM, extensible)
    
    // 1 - Check if it "RIFF" file
    result = fread(buffer, 1, 4, fp);
    if(result != 4) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -1;
    }    
    printf("Chunk ID: %s \n", buffer); //------------------DEBUG------------------
    if (memcmp("RIFF", buffer, 4) != 0)
    {
        fprintf(stderr, "This wave file doesn't have RIFF chunk!\n");
        fclose(fp);
        return -2;
    }
    memcpy(waveData->chunkId, buffer, 4);
    
    // read chunk RIFF size
    result = fread(buffer, 1, 4, fp);
    if(result != 4) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("Chunk size: %d\n", char2int(buffer));//------------------DEBUG------------------
    waveData->chunkSize = char2int(buffer);
    
    // 2 - Check if it has "WAVE" format
    result = fread(buffer, 1, 4, fp);
    if(result != 4) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -1;
    }    
    printf("Wave ID: %s \n", buffer); //------------------DEBUG------------------
    if (memcmp("WAVE", buffer, 4) != 0)
    {
        fprintf(stderr, "This wave file doesn't have WAVE format!\n");
        fclose(fp);
        return -2;
    }
    memcpy(waveData->format, buffer, 4);
    
     // 3 - Check if it has "fmt " chunk ID
    result = fread(buffer, 1, 4, fp);
    if(result != 4) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -1;
    }    
    printf("Sub Chunk 1 ID: %s \n", buffer); //------------------DEBUG------------------
    if (memcmp("fmt ", buffer, 4) != 0)
    {
        fprintf(stderr, "This wave file doesn't have 'fmt ' chunk ID!\n");
        fclose(fp);
        return -2;
    }
    memcpy(waveData->subChunk1Id, buffer, 4);
    
    // read sub chunk 1 size
    result = fread(buffer, 1, 4, fp);
    if(result != 4) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("Chunk size: %d\n", char2int(buffer)); //------------------DEBUG------------------
    waveData->subChunk1Size = char2int(buffer);
    
    // read wave audio format
    result = fread(buffer, 1, 2, fp);
    if(result != 2) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("wFormatTag: %d\n", char2short(buffer)); //------------------DEBUG------------------
    waveFormat = char2short(buffer);
    waveData->audioFormat = char2short(buffer);
    
    // read number of channels
    result = fread(buffer, 1, 2, fp);
    if(result != 2) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("Number of channels: %d \n", char2short(buffer)); //------------------DEBUG------------------
    waveData->numChannels = char2short(buffer);
    
    // read sample rate
    result = fread(buffer, 1, 4, fp);
    if(result != 4) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("Samples per sec: %d \n", char2int(buffer)); //------------------DEBUG------------------
    waveData->sampleRate = char2int(buffer);
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("AvgBytes per sec: %d \n", char2int(buffer)); //------------------DEBUG------------------
    waveData->byteRate = char2int(buffer);
    
    result = fread(buffer, 1, 2, fp);
    if(result != 2) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("nBlockAlign: %d \n", char2short(buffer)); //------------------DEBUG------------------
    waveData->blockAlign = char2short(buffer);
    
    result = fread(buffer, 1, 2, fp);
    if(result != 2) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("wBitsPerSample: %d \n", char2short(buffer)); //------------------DEBUG------------------
    waveData->bitsPerSample = char2short(buffer);
    
    // ============= Make Decision based on format ===================================
    switch(waveFormat)
    {
        case 0x0001:    // WAVE_FORMAT_PCM
            break;
        case 0x0003:    // WAVE_FORMAT_IEEE_FLOAT
            result = fread(buffer, 1, 2, fp);
            if(result != 2) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("cbSize: %d \n", char2short(buffer)); //------------------DEBUG------------------
            waveData->cbSize = char2short(buffer);
            
            result = fread(buffer, 1, 4, fp);
            if(result != 4) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("ckID: %s \n", buffer); //------------------DEBUG------------------
            memcpy(waveData->subChunk2Id, buffer, 4);
            
            result = fread(buffer, 1, 4, fp);
            if(result != 4) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("cksize: %d \n", char2int(buffer)); //------------------DEBUG------------------
            waveData->subChunk2Size = char2int(buffer);
            
            result = fread(buffer, 1, 4, fp);
            if(result != 4) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("dwSampleLength: %d \n", char2int(buffer)); //------------------DEBUG------------------
            waveData->sampleLength = char2int(buffer);
            
            break;
        case 0xFFFE:    // WAVE_FORMAT_EXTENSIBLE
            result = fread(buffer, 1, 2, fp);
            if(result != 2) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("cbSize: %d \n", char2short(buffer)); //------------------DEBUG------------------
            waveData->cbSize = char2short(buffer);
            // 3 missing
            
            result = fread(buffer, 1, 2, fp);
            if(result != 2) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("wValidBitsPerSample: %d \n", char2short(buffer)); //------------------DEBUG------------------
            waveData->wValidBitsPerSample = char2short(buffer);
            
            result = fread(buffer, 1, 4, fp);
            if(result != 4) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("dwChannelMask: %d \n", char2int(buffer)); //------------------DEBUG------------------
            waveData->dwChannelMask = char2int(buffer);   
            
            result = fread(buffer, 1, 16, fp);
            if(result != 16) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("SubFormat: %s \n", buffer); //------------------DEBUG------------------
            memcpy(waveData->SubFormat, buffer, 16);
            
            //---
            result = fread(buffer, 1, 4, fp);
            if(result != 4) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("ckID: %s \n", buffer); //------------------DEBUG------------------
            memcpy(waveData->subChunk2Id, buffer, 4);
            
            result = fread(buffer, 1, 4, fp);
            if(result != 4) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("cksize: %d \n", char2int(buffer)); //------------------DEBUG------------------
            waveData->subChunk2Size = char2int(buffer);
            
            result = fread(buffer, 1, 4, fp);
            if(result != 4) 
            {
                fprintf(stderr, "Failed reading file!\n");
                fclose(fp);
                return -2;
            }
            printf("dwSampleLength: %d \n", char2int(buffer)); //------------------DEBUG------------------
            waveData->sampleLength = char2int(buffer);
            
            break;
        default:        // WAVE_FORMAT_ALAW or WAVE_FORMAT_MULAW
            fprintf(stderr, "Unsupported wave format\n");
            fclose(fp);
            return -2;
            break;
    }
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("ckID: %s \n", buffer); //------------------DEBUG------------------
    memcpy(waveData->subChunk3Id, buffer, 4);
    
    
    result = fread(buffer, 1, 4, fp);
    if(result != 4) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    printf("cksize: %d \n", char2int(buffer)); //------------------DEBUG------------------
    waveData->subChunk3Size = char2int(buffer);
    
    // read actual data set
    waveData->data = malloc(waveData->subChunk3Size * sizeof *waveData->data);
    
    result = fread(waveData->data, 1, waveData->subChunk3Size, fp);
    if(result != waveData->subChunk3Size) 
    {
        fprintf(stderr, "Failed reading file!\n");
        fclose(fp);
        return -2;
    }
    
    //Close the file
    fclose(fp);
}

void printWaveData(struct wav_file *waveData)
{
    printf("Struct chunkId: %-4.4s \n", waveData->chunkId);
    printf("Struct chunkSize: %d \n", waveData->chunkSize);
    printf("Struct format: %-4.4s \n", waveData->format);
    printf("Struct subChunk1Id: %-4.4s \n", waveData->subChunk1Id);
    printf("Struct subChunk1Size: %d \n", waveData->subChunk1Size);
    printf("Struct audioFormat: %X \n", waveData->audioFormat);
    printf("Struct numChannels: %d \n", waveData->numChannels);
    printf("Struct sampleRate: %d \n", waveData->sampleRate);
    printf("Struct byteRate: %d \n", waveData->byteRate);
    printf("Struct blockAlign: %d \n", waveData->blockAlign);
    printf("Struct bitsPerSample: %d \n", waveData->bitsPerSample);
    printf("Struct cbSize: %d \n", waveData->cbSize);
    printf("Struct wValidBitsPerSample: %d \n", waveData->wValidBitsPerSample);
    printf("Struct dwChannelMask: %d \n", waveData->dwChannelMask);
    printf("Struct cbSize: %d \n", waveData->cbSize);
    printf("Struct subChunk2Id: %-4.4s \n", waveData->subChunk2Id);
    printf("Struct subChunk2Size: %d \n", waveData->subChunk2Size);
    printf("Struct sampleLength: %d \n", waveData->sampleLength);
    printf("Struct subChunk3Id: %-4.4s \n", waveData->subChunk3Id);
    printf("Struct subChunk3Size: %d \n", waveData->subChunk3Size);
    /*
    FILE *fp2;
    
    fp2 = fopen("output.txt", "w");
    int k = 0;
    unsigned int tempLeft = 0.0;
    unsigned int tempRight = 0.0;
    for(k = 0; k < waveData->subChunk3Size; k+=8)
    {
        tempLeft = waveData->data[k+0] | waveData->data[k+1]<<8 | waveData->data[k+2]<<16 | waveData->data[k+3]<<24;
        tempRight = waveData->data[k+4] | waveData->data[k+5]<<8 | waveData->data[k+6]<<16 | waveData->data[k+7]<<24;
        
        printf("[ %4.8f ]  [ %4.8f ]\n", uint2float(tempLeft), uint2float(tempRight));
        fprintf(fp2, "%1.8f\n", uint2float(tempRight));
    }
    printf("Counted: %d\n", k);
    fclose(fp2);
    */
}

float uint2float(unsigned int integer)
{
    union
    {
        unsigned int uint;
        float f;
    }converter;
    
    converter.uint = integer;
    
    return converter.f;
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

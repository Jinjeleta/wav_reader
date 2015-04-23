//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Header files or external libraries
#include "wav_file.h"

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
        
        
        free(waveData->data);
        free(waveData);
        //Last message
        printf("Program is finished.\n");
    }
    return 0;
}

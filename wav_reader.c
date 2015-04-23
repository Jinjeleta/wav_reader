//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//Header files or external libraries
#include "wav_file.h"
#include "portaudio.h"


#define NUM_SECONDS   (5)

/*
#ifndef M_PI
#define M_PI  (3.14159265)
#endif
*/
#define FRAMES_PER_BUFFER  (64)
#define TABLE_SIZE   (200)
typedef struct
{
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
}
paTestData;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags, 
                            *userData)
{
    //paTestData *data = (paTestData*)userData;
    struct wav_file *waveData = (struct wav_file*)userData;
    float *out = (float*)outputBuffer;
    unsigned long k;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;
    
    unsigned int tempLeft = 0;
    unsigned int tempRight = 0;
    unsigned long int j = 0;

    for( k=0; k<framesPerBuffer; k++ )
    {
        j = waveData->playProgress;
        if((j + k + 7) < waveData->subChunk3Size)
        {
            tempLeft = waveData->data[j+k+0] | waveData->data[j+k+1]<<8 | waveData->data[j+k+2]<<16 | waveData->data[j+k+3]<<24;
            tempRight = waveData->data[j+k+4] | waveData->data[j+k+5]<<8 | waveData->data[j+k+6]<<16 | waveData->data[j+k+7]<<24;
            *out++ = uint2float(tempLeft);
            *out++ = uint2float(tempRight);
            waveData->playProgress += 1;
        }
        else
        {
            *out++ = 0.0;
            *out++ = 0.0;
        }
        
    }
    
    return paContinue;
}

/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished( *userData )
{
   //paTestData *data = (paTestData *) userData;
   printf( "Stream Completed: ... \n");
}

int main( int argc, char *argv[] )
{
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s filename\n", argv[0] );
        return 0;
    }
    else
    {
        // Application simply decodes wav files
        printf("This is C application written by Nerijus\n");
        
        //Local variables
        char fileName[] = "sample.wav";
        strcpy ( fileName, argv[1] );
        struct wav_file *waveData = malloc(sizeof *waveData);
        //-----
        PaStreamParameters outputParameters;
        PaStream *stream;
        PaError err;
        //paTestData data;
        int i;
        
        readWaveData(fileName, waveData);
        printWaveData(waveData);
           
        printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);
        
        /* initialise sinusoidal wavetable */
        /*
        for( i=0; i<TABLE_SIZE; i++ )
        {
            data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
        }
        data.left_phase = data.right_phase = 0;
        */
        
        err = Pa_Initialize();
        if( err != paNoError ) goto error;

        outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
        if (outputParameters.device == paNoDevice) {
          fprintf(stderr,"Error: No default output device.\n");
          goto error;
        }
        //outputParameters.channelCount = 2;       /* stereo output */
        outputParameters.channelCount = waveData->numChannels;       /* stereo output */
        outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
        outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;
        
        /*
        err = Pa_OpenStream(
                  &stream,
                  NULL, // no input
                  &outputParameters,
                  SAMPLE_RATE,
                  FRAMES_PER_BUFFER,
                  paClipOff,      // we won't output out of range samples so don't bother clipping them
                  patestCallback,
                  &data );
        */
        err = Pa_OpenStream(
                  &stream,
                  NULL, // no input
                  &outputParameters,
                  waveData->sampleRate,
                  FRAMES_PER_BUFFER,
                  paClipOff,      // we won't output out of range samples so don't bother clipping them
                  patestCallback,
                  waveData );
        if( err != paNoError ) goto error;

        sprintf( data.message, "No Message" );
        err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
        if( err != paNoError ) goto error;

        err = Pa_StartStream( stream );
        if( err != paNoError ) goto error;

        printf("Play for %d seconds.\n", NUM_SECONDS );
        Pa_Sleep( NUM_SECONDS * 1000 );

        err = Pa_StopStream( stream );
        if( err != paNoError ) goto error;

        err = Pa_CloseStream( stream );
        if( err != paNoError ) goto error;

        Pa_Terminate();
        printf("Test finished.\n");
        
        free(waveData->data);
        free(waveData);
        return err;
    error:
        Pa_Terminate();
        free(waveData->data);
        free(waveData);
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        return err;
    }
}


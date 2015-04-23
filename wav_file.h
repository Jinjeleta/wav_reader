#ifndef WAV_FILE_H
#define WAV_FILE_H
 
struct wav_file {
    unsigned char chunkId[4];                   // all
    unsigned int chunkSize;                     // all
    unsigned char format[4];                    // all
    unsigned char subChunk1Id[4];               // all
    unsigned int subChunk1Size;                 // all
    unsigned short int audioFormat;             // all
    unsigned short int numChannels;             // all    
    unsigned int sampleRate;                    // all
    unsigned int byteRate;                      // all
    unsigned short int blockAlign;              // all
    unsigned short int bitsPerSample;           // all
    unsigned short int cbSize;                  // nonPCM and extensible
    unsigned short int wValidBitsPerSample;     // only extensible
    unsigned int dwChannelMask;                 // only extensible
    unsigned char SubFormat[16];                // only extensible
    unsigned char subChunk2Id[4];               // nonPCM and extensible
    unsigned int subChunk2Size;                 // nonPCM and extensible
    unsigned int sampleLength;                  // nonPCM and extensible
    unsigned char subChunk3Id[4];               // all    
    unsigned int subChunk3Size;                 // all
    unsigned char *data;                        // all
};

int readWaveData(unsigned char *fileName, struct wav_file *waveData);
void printWaveData(struct wav_file *waveData);
float uint2float(unsigned int integer);
unsigned short char2short(unsigned char *buff);
unsigned int char2int(unsigned char *buff);
 
#endif // WAV_FILE_H
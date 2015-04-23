PROGRAM = wav_reader
SOURCE = wav_reader.c wav_file.c
OBJECT = $(SOURCE:.c=.o)

$(PROGRAM): $(OBJECT)
	$(CC) -o $@ $(OBJECT) $(CFLAGS)

wav_reader.o: wav_reader.c wav_file.h wav_file.c

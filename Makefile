PROGRAM = wav_reader
SOURCE = wav_reader.c
OBJECT = $(SOURCE:.c=.o)

$(PROGRAM): $(OBJECT)
	$(CC) $(CFLAGS) -o $@ $(OBJECT)

wav_reader.o: wav_reader.c

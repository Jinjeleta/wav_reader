PROGRAM = wav_reader
SOURCE = wav_reader.c wav_file.c
OBJECT = $(SOURCE:.c=.o)
LIBDIR = /usr/local/lib
LIBRARY = $(LIBDIR)/libportaudio.so

$(PROGRAM): $(OBJECT)
	$(CC) -o $@ $(OBJECT) $(CFLAGS) $(LDFLAGS) $(LIBRARY)

wav_reader.o: wav_reader.c wav_file.h wav_file.c portaudio.h

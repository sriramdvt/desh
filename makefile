CC = gcc
CFLAGS = -Werror -Wall -Wextra

SDIR = src
IDIR = include
BDIR = build

_OBJECT_FILES = desh.o utils.o handler.o echo.o dpwd.o cd.o ls.o pinfo.o general_execute.o history.o pipeline.o redirection.o jobs.o sig.o fg.o bg.o
OBJECT_FILES = $(patsubst %,$(BDIR)/%,$(_OBJECT_FILES))

build: $(OBJECT_FILES)
	$(CC) $(CFLAGS) -I$(IDIR) -o desh $(OBJECT_FILES)

$(BDIR)/desh.o: $(SDIR)/desh.c $(SDIR)/utils.c $(IDIR)/utils.h $(IDIR)/structs.h $(IDIR)/constants.h $(IDIR)/handler.h $(SDIR)/handler.c $(SDIR)/general_execute.c $(IDIR)/general_execute.h $(IDIR)/pipeline.h $(SDIR)/pipeline.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/desh.c -o $(BDIR)/desh.o  

$(BDIR)/utils.o: $(SDIR)/utils.c $(IDIR)/utils.h $(IDIR)/constants.h
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/utils.c -o $(BDIR)/utils.o  

$(BDIR)/handler.o: $(IDIR)/handler.h $(SDIR)/handler.c $(IDIR)/structs.h $(IDIR)/constants.h $(IDIR)/libraries.h $(IDIR)/echo.h $(SDIR)/echo.c $(IDIR)/dpwd.h $(SDIR)/dpwd.c $(IDIR)/dpwd.h $(SDIR)/dpwd.c $(IDIR)/cd.h $(SDIR)/cd.c $(IDIR)/ls.h $(SDIR)/ls.c $(SDIR)/general_execute.c $(IDIR)/general_execute.h $(IDIR)/jobs.h $(SDIR)/jobs.c $(IDIR)/sig.h $(SDIR)/sig.c $(IDIR)/fg.h $(SDIR)/fg.c $(IDIR)/bg.h $(SDIR)/bg.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/handler.c -o $(BDIR)/handler.o

$(BDIR)/echo.o: $(IDIR)/echo.h $(SDIR)/echo.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/echo.c -o $(BDIR)/echo.o

$(BDIR)/dpwd.o: $(IDIR)/dpwd.h $(SDIR)/dpwd.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/dpwd.c -o $(BDIR)/dpwd.o

$(BDIR)/cd.o: $(IDIR)/cd.h $(SDIR)/cd.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/cd.c -o $(BDIR)/cd.o

$(BDIR)/ls.o: $(IDIR)/ls.h $(SDIR)/ls.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/ls.c -o $(BDIR)/ls.o

$(BDIR)/pinfo.o: $(IDIR)/pinfo.h $(SDIR)/pinfo.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/pinfo.c -o $(BDIR)/pinfo.o

$(BDIR)/general_execute.o: $(IDIR)/general_execute.h $(SDIR)/general_execute.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c $(IDIR)/jobs.h $(SDIR)/jobs.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/general_execute.c -o $(BDIR)/general_execute.o

$(BDIR)/history.o: $(IDIR)/history.h $(SDIR)/history.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/history.c -o $(BDIR)/history.o

$(BDIR)/pipeline.o: $(IDIR)/pipeline.h $(SDIR)/pipeline.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c $(IDIR)/handler.h $(SDIR)/handler.c $(SDIR)/general_execute.c $(IDIR)/general_execute.h $(IDIR)/redirection.h $(SDIR)/redirection.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/pipeline.c -o $(BDIR)/pipeline.o

$(BDIR)/redirection.o: $(IDIR)/redirection.h $(SDIR)/redirection.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/redirection.c -o $(BDIR)/redirection.o

$(BDIR)/jobs.o: $(IDIR)/jobs.h $(SDIR)/jobs.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/jobs.c -o $(BDIR)/jobs.o

$(BDIR)/sig.o: $(IDIR)/sig.h $(SDIR)/sig.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c $(IDIR)/jobs.h $(SDIR)/jobs.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/sig.c -o $(BDIR)/sig.o

$(BDIR)/fg.o: $(IDIR)/fg.h $(SDIR)/fg.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c $(IDIR)/jobs.h $(SDIR)/jobs.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/fg.c -o $(BDIR)/fg.o

$(BDIR)/bg.o: $(IDIR)/bg.h $(SDIR)/bg.c $(IDIR)/structs.h $(IDIR)/libraries.h $(IDIR)/constants.h $(IDIR)/utils.h $(SDIR)/utils.c $(IDIR)/jobs.h $(SDIR)/jobs.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $(SDIR)/bg.c -o $(BDIR)/bg.o

.PHONY: clean

clean:
	rm -f $(BDIR)/*.o .desh_pd .desh_history


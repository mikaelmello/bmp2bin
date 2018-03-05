CC       = gcc
OBJ 	 = bmp2bin.c
LIBS     = 
INCS     = 
BIN      = bmp2bin
CFLAGS   = $(INCS) -std=c11
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

all-after: 
	mkdir -p ~/bin
	mv $(BIN) ~/bin/

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIBS)

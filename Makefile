CC = gcc

BINDIR = ./bin/
OBJDIR = ./obj/

CFLAGS  = -O3 -s -DNDEBUG -Wall -mwindows
LIBS    = -lwinmm

# the build target executable:
TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).c
	mkdir -p $(OBJDIR) $(BINDIR)
	$(CC) $(CFLAGS) -c -o $(OBJDIR)main.o main.c
	windres resource.rc $(OBJDIR)res.o
	$(CC) $(CFLAGS) -o $(BINDIR)$(TARGET).exe $(OBJDIR)main.o ./ufmod/ufmod.obj $(OBJDIR)res.o $(LIBS)
	#upx -9 $(TARGET).exe

clean:
	$(RM) $(TARGET)

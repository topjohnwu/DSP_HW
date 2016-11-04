.PHONY: all clean

CFLAGS+= -O3
CPPFLAGS += $(CFLAGS)
LDFLAGS+=-lm     # link to math library

TARGET= train test

all: $(TARGET)
# type make/make all to compile test_hmm

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file

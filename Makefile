.PHONY: all clean
	
CC        	= gcc
TARGET		= serminal
ARCH		= $(shell arch)
UNAME 		= $(shell uname -s)

all: $(TARGET)

$(TARGET): *.mm
		@echo "[INFO]: compiling $(TARGET).."
		$(CC) -o $@ $^
		@echo "OK: compiled $(TARGET) for $(UNAME) $(ARCH)"

clean:
	rm -f $(TARGET) && rm -f *.o
project_name=CH559USB
xram_size=0x0800
xram_loc=0x0600
code_size=0xEFFF
dfreq_sys=48000000

PRJ = $(project_name)

VERBOSE = -V

CC = sdcc

CPPFLAGS = -I/
TFLAGS = -mmcs51 --model-large --xram-size $(xram_size) --xram-loc $(xram_loc) --code-size $(code_size) -DFREQ_SYS=$(dfreq_sys)

CFLAGS = -c $(VERBOSE) $(CPPFLAGS) $(TFLAGS)
LFLAGS = $(VERBOSE) $(TFLAGS)

# core system source files
SRC = main.c util.c USBHost.c uart.c
OBJ = $(SRC:.c=.rel)

all : $(PRJ) 
	@echo
	@echo "Done."
	@echo

$(PRJ): $(PRJ).bin

$(PRJ).bin : $(PRJ).hex
	makebin -p $< $@

$(PRJ).hex : $(PRJ).ihx
	packihx $< > $@

$(PRJ).ihx : $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@

%.d : %.c
	@$(CC) -MM $(CFLAGS) $< > $@

%.rel : %.c
	@$(CC) $(CFLAGS) $<

-include $(SRC:.c=.d)

build: _rm_obj all

install: 
	@echo
	@echo Waiting to be written...
	@echo

clean : _rm_obj _rm_deps

_rm_obj:
	rm -f *.rel
	rm -f *.lst
	rm -f *.asm
	rm -f *.sym
	rm -f *.rst

_rm_deps:
	rm -f *.d

_rm_dist:
	rm -f *.lk
	rm -f *.map
	rm -f *.mem
	rm -f *.ihx

distclean: clean _rm_dist

.PHONY : all build install clean distclean

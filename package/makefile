CC = clang
LD = clang

CFLAGS += -Wall -pedantic -I include/
LDFLAGS += -L ./
LDLIBS +=

OUT = crumbshell
SRC = $(wildcard *.cpp)
OBJ = $(addprefix obj/,$(SRC:.cpp=.o))

all: crumbshell pbs pfe boot

clean:
	rm -f $(OBJ) $(OUT)

run: all
	./crumbshell

CRUMBSHELL_SRC = crumbshell.c
CRUMBSHELL_OBJ = $(addprefix obj/,$(CRUMBSHELL_SRC:.c=.o))
crumbshell: $(CRUMBSHELL_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

PBS_SRC = pbs.c fatSupport.c fat.c
PBS_OBJ = $(addprefix obj/,$(PBS_SRC:.c=.o))
pbs: $(PBS_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

PFE_SRC = pfe.c fatSupport.c
PFE_OBJ = $(addprefix obj/,$(PFE_SRC:.c=.o))
$(info $(PFE_OBJ))
pfe: $(PFE_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

BOOT_SRC = boot.c fatSupport.c
BOOT_OBJ = $(addprefix obj/,$(BOOT_SRC:.c=.o))
boot: $(BOOT_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

obj/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

$(OUT): $(OBJ)
	$(CC) $(LDFLAGS) -o $(OUT) $^ $(LDLIBS)

#
#ALL: fatSupport.c fat.c boot.c
#	gcc fatSupport.c fat.c boot.c -o boot
#
#BOOT: fatSupport.c fat.c boot.c
#	gcc fatSupport.c fat.c boot.c -o boot
#

CC=g++
OBJ_ARGS=-lsfml-graphics -lsfml-window -lsfml-system
CC_SRCS=main.cpp
NOSSE_SRC=nosse.cpp
AVX_SRC=avx.cpp
TEST_SRC=tester.cpp
OBJDIR=obj/
DEPDIR=dep/
CC_ARGS=-I /usr/include/SFML

all: $(OBJDIR) $(DEPDIR) nosse avx test

$(OBJDIR)%.o: %.cpp
	$(CC) -M $(CC_ARGS) $< -o $(DEPDIR)$(<:.cpp=.d)
	echo "$(OBJDIR)" > line.tmp
	mv $(DEPDIR)$(<:.cpp=.d) input.tmp
	head -c -1 -q line.tmp input.tmp > $(DEPDIR)$(<:.cpp=.d)
	rm input.tmp line.tmp
	$(CC) -c $(CC_ARGS) $< -o $@ -march=native

include $(wildcard $(DEPDIR)/*.d)

nosse: $(addprefix $(OBJDIR), $(NOSSE_SRC:.cpp=.o) $(CC_SRCS:.cpp=.o))
	$(CC) $^ -o $@ $(OBJ_ARGS)

avx: $(addprefix $(OBJDIR), $(AVX_SRC:.cpp=.o) $(CC_SRCS:.cpp=.o))
	$(CC) $^ -o $@ $(OBJ_ARGS) -march=native

test: $(addprefix $(OBJ_DIR), $(TEST_SRC:.cpp=.o))
	$(CC) $^ -o $@ $(OBJ_DIR)

$(OBJDIR):
	mkdir $@

$(DEPDIR):
	mkdir $@

clean:
	rm -rf *.o *.d nosse avx test times.txt
	rm -rf $(DEPDIR) $(OBJDIR)
	clear

.PHONY: clean all
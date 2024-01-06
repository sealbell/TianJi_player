
EXEC = test
GCC = gcc
OBJ = test.o  start_mplayer.o  my_lrc_play.o console.o my_add.o
$(EXEC):$(OBJ)
	$(GCC) $^ -o $@

%.o:%.c
	$(GCC) -c $< -o $@

clean:
	rm $(EXEC) $(OBJ)

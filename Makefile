INC=./includes
SRC=./src
OUT=./bin

$(OUT)/lab: $(OUT)/main.o $(OUT)/std.o
	gcc -o $(OUT)/lab $(OUT)/main.o $(SRC)/main.c

$(OUT)/main.o: $(SRC)/main.c $(INC)/general.h
	gcc -I$(INC) -o $(OUT)/main.o
clean:
	rm $(OUT)/*.o $(OUT)/lab

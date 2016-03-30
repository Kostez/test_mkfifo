INC=./includes
SRC=./src
OUT=./bin

$(OUT)/lab: $(OUT)/main.o 
	gcc -o $(OUT)/lab $(OUT)/main.o 
$(OUT)/zxc: $(OUT)/zxc.o 
	gcc -o $(OUT)/zxc $(OUT)/zxc.o 

$(OUT)/main.o: $(SRC)/main.c $(INC)/general.h
	gcc -I$(INC) -o $(OUT)/main.o -c $(SRC)/main.c
$(OUT)/zxc.o: $(SRC)/zxc.c
	gcc -o $(OUT)/zxc.o-$(SRC)/zxc.c

clean:
	rm $(OUT)/*.o $(OUT)/lab

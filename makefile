CC=gcc
CFLAGS=-Wall -ansi -pedantic -O2
LDFLAGS=-lMLV -lm

SRC_DIR=src
OBJ_DIR=bin
INCLUDE_DIR=include

OBJ=$(OBJ_DIR)/main.o $(OBJ_DIR)/game.o $(OBJ_DIR)/ground.o $(OBJ_DIR)/display.o $(OBJ_DIR)/player.o $(OBJ_DIR)/keys.o $(OBJ_DIR)/position.o $(OBJ_DIR)/guardian.o $(OBJ_DIR)/relic.o $(OBJ_DIR)/leaderboard.o
EXEC=stealth

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/main.o: main.c $(INCLUDE_DIR)/game.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/game.o: $(SRC_DIR)/game.c $(INCLUDE_DIR)/game.h $(INCLUDE_DIR)/ground.h $(INCLUDE_DIR)/display.h $(INCLUDE_DIR)/keys.h $(INCLUDE_DIR)/player.h $(INCLUDE_DIR)/position.h $(INCLUDE_DIR)/guardian.h $(INCLUDE_DIR)/relic.h $(INCLUDE_DIR)/leaderboard.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/ground.o: $(SRC_DIR)/ground.c $(INCLUDE_DIR)/ground.h $(INCLUDE_DIR)/relic.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/display.o: $(SRC_DIR)/display.c $(INCLUDE_DIR)/display.h $(INCLUDE_DIR)/ground.h $(INCLUDE_DIR)/player.h $(INCLUDE_DIR)/position.h $(INCLUDE_DIR)/guardian.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/player.o: $(SRC_DIR)/player.c $(INCLUDE_DIR)/player.h $(INCLUDE_DIR)/ground.h $(INCLUDE_DIR)/position.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/keys.o: $(SRC_DIR)/keys.c $(INCLUDE_DIR)/keys.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/position.o: $(SRC_DIR)/position.c $(INCLUDE_DIR)/position.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/guardian.o: $(SRC_DIR)/guardian.c $(INCLUDE_DIR)/guardian.h $(INCLUDE_DIR)/ground.h $(INCLUDE_DIR)/position.h $(INCLUDE_DIR)/player.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/relic.o: $(SRC_DIR)/relic.c $(INCLUDE_DIR)/relic.h $(INCLUDE_DIR)/ground.h $(INCLUDE_DIR)/position.h $(INCLUDE_DIR)/player.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/leaderboard.o: $(SRC_DIR)/leaderboard.c $(INCLUDE_DIR)/leaderboard.h
	$(CC) -o $@ -c $< $(CFLAGS)

circle:
	$(CC) $(CFLAGS) -DBASIC_CIRCLE -c $(SRC_DIR)/display.c -o $(OBJ_DIR)/display.o
	$(CC) -o $(EXEC) $(OBJ) $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR)/*.o

mrproper: clean
	rm -rf $(EXEC)
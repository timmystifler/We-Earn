CC=g++
CXX=g++
CFLAGS += -g -pipe 
CFLAGS+= -Wall

MOD_NAME=we_earn

APP_EXE=./$(MOD_NAME)

APP_LIB	= ./lib/trader_api_636/thostmduserapi.so ./lib/trader_api_636/thosttraderapi.so

SUB_MODULES= . init md_spi trade_spi

CINC += $(foreach dir,$(SUB_MODULES),-I $(dir))
CINC += -I ./lib/trader_api_636

ALL_CFILES = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.c))

ALL_CPPFILES = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.cpp))

ALL_OBJ = $(ALL_CFILES:.c=.o) $(ALL_CPPFILES:.cpp=.o)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CINC) $< -o $@

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(CINC) $< -o $@

$(APP_EXE): $(ALL_OBJ) $(APP_LIB) 
	$(CC) $(CINC) -o $@ $(CFLAGS) $(ALL_OBJ) $(APP_LIB)

DEL_II = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.i))
DEL_OBJ = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.o))

all: $(APP_EXE)

clean:
	-rm -rf *.o $(APP_EXE) $(DEL_OBJ)

.PHONY: all clean

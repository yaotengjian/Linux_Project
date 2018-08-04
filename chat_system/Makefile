ROOT_PATH=$(shell pwd)
SERVER_PATH=$(ROOT_PATH)/server
CLIENT_PATH=$(ROOT_PATH)/client
DATA_POOL_PATH=$(ROOT_PATH)/data_pool
COMM_PATH=$(ROOT_PATH)/comm
LIB_PATH=$(ROOT_PATH)/lib
WINDOW_PATH=$(ROOT_PATH)/window

cc=g++
FLAGS=-I$(DATA_POOL_PATH) -I$(COMM_PATH) -I$(LIB_PATH)/include -I$(WINDOW_PATH)
LDFLAGS=-L$(LIB_PATH)/lib -lpthread -ljsoncpp

ser=chat_server
ser_obj=$(shell ls $(SERVER_PATH)/*.cpp $(DATA_POOL_PATH)/*.cpp $(COMM_PATH)/*.cpp| sed 's/.cpp/.o/'|sed 's/.*\///g')

cli=chat_client
cli_obj=$(shell ls $(CLIENT_PATH)/*.cpp $(COMM_PATH)/*.cpp $(WINDOW_PATH)/*.cpp| sed 's/.cpp/.o/'|sed 's/.*\///g')

.PHONY:all
all:$(ser) $(cli)

$(ser):$(ser_obj)
	@$(cc) -o $@ $^ $(LDFLAGS)
	@echo "Linking [$^] to [$@] ... done"
$(cli):$(cli_obj)
	@$(cc) -o $@ $^ $(LDFLAGS) -lncurses -static
	@echo "Linking [$^] to [$@] ... done"
%.o:$(SERVER_PATH)/%.cpp
	@$(cc) -c $< $(FLAGS)
	@echo "Comping [$<] to [$@] ... done"
%.o:$(DATA_POOL_PATH)/%.cpp
	@$(cc) -c $< $(FLAGS)
	@echo "Comping [$<] to [$@] ... done"
%.o:$(CLIENT_PATH)/%.cpp
	@$(cc) -c $< $(FLAGS)
	@echo "Comping [$<] to [$@] ... done"
%.o:$(COMM_PATH)/%.cpp
	@$(cc) -c $< $(FLAGS)
	@echo "Comping [$<] to [$@] ... done"
%.o:$(WINDOW_PATH)/%.cpp
	@$(cc) -c $< $(FLAGS)
	@echo "Comping [$<] to [$@] ... done"

.PHONY:clean
clean:
	rm -f *.o $(ser) $(cli)

.PHONY:debug
debug:
	@echo $(ser_obj)
	@echo $(cli_obj)







all : mkdir diff
SRC_DIFF_DIR = ./src/differentiator/
SRC_TREE_DIR = ./src/tree/
SRC_CALC_DIR = ./src/calcul/
OBJ_DIR = ./obj/
DUMP_DIR = ./graph_dumps/
CC = g++

# RELEASE_FLAGS := -DNDEBUG
# release:
# 	g++ $(RELEASE_FLAGS)

CFLAGS= -save-temps -Wall -Wextra -fsanitize=address -g -O0#-D _DEBUG -ggdb3 -std=c++20  -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default \
-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -flto-odr-type-merging \
-fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr \
-pie -Wlarger-than=8192 -Wstack-usage=8192

##include paths
INCLUDE_PATH = -I./include/ 

#sources
DIFF_SRC := $(wildcard $(SRC_DIFF_DIR)*.cpp)
TREE_SRC := $(wildcard $(SRC_TREE_DIR)*.cpp)
CALC_SRC := $(wildcard $(SRC_CALC_DIR)*.cpp)


#obj
DIFF_OBJ  := $(patsubst $(SRC_DIFF_DIR)%.cpp, $(OBJ_DIR)%.o, $(DIFF_SRC)) 
TREE_OBJ  := $(patsubst $(SRC_TREE_DIR)%.cpp, $(OBJ_DIR)%.o, $(TREE_SRC)) 
CALC_OBJ  := $(patsubst $(SRC_CALC_DIR)%.cpp, $(OBJ_DIR)%.o, $(CALC_SRC)) 



#exe
DIFF_EXE := diff

$(DIFF_EXE) : $(DIFF_OBJ) $(TREE_OBJ) $(CALC_OBJ)
	@$(CC) $(CFLAGS) $(INCLUDE_PATH) $(DIFF_OBJ) $(TREE_OBJ) $(CALC_OBJ) -o $(DIFF_EXE)

$(OBJ_DIR)%.o : $(SRC_DIFF_DIR)%.cpp 
	@$(CC) $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(SRC_TREE_DIR)%.cpp 
	@$(CC) $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(SRC_CALC_DIR)%.cpp 
	@$(CC) $(INCLUDE_PATH) -c $< -o $@

mkdir :
	@mkdir $(OBJ_DIR) -p
	@mkdir $(DUMP_DIR) -p

clean:
	rm $(OBJ_DIR)*.o $(OBJ_DIR)*.ii $(OBJ_DIR)*.s $(DUMP_DIR)*.jpeg *.save diff
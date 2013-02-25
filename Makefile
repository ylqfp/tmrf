CC=g++
CFLAGS=-c -pipe -O3
INCLUDE=-I /home/xh/Nutstore/my/code/cpp/mylib
#SRC=$(wildcard *.cpp)
MODEL_OBJ=model.o train.o infer.o main.o
MODEL_EXE=tmrf

all:$(MODEL_EXE) 

$(MODEL_EXE):$(MODEL_OBJ)
	$(CC) $(MODEL_OBJ) -o $@

main.o:train.h infer.h
model.o:model.h
train.o:train.h model.h
infer.o:infer.h model.h

%.o:%.cpp
	$(CC) $(INCLUDE) $(CFLAGS) $< -o $@

clean:
	rm -rf $(MODEL_OBJ) $(MODEL_EXE)

CXX=g++
CFLAGS=-Wall
LDFLAGS=-lallegro
INCLUDE=-I. -I/usr/include
OUT=game
SOURCE=*.cpp

all: debug

clean:
	rm -rf *.o $(OUT)

debug: $(OBJS)
	$(CXX) $(SOURCE) -g -o $(OUT) $(INCLUDE) $(CFLAGS) $(LDFLAGS)

release: $(OBJS)
	$(CXX) $(SOURCE) -o2 -o $(OUT) $(INCLUDE) $(CFLAGS) $(LDFLAGS)

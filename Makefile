# === Configuration ===
CXX := g++
CXXFLAGS := -g -Iinclude
LDFLAGS := -Llib -lmingw32 -lSDL3 -lSDL3_image -lSDL3_ttf
SRCS := src/main.c res/resource.res
OUT := kanoodle.exe
OUT-DEBUG := kanoodle-debug.exe

# === Targets ===

compile: test run

test: $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(OUT) $(LDFLAGS)

build: $(SRCS)
	windres res/resource.rc -O coff -o res/resource.res
	$(CXX) $(CXXFLAGS) -mwindows $(SRCS) -o $(OUT) $(LDFLAGS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(OUT-DEBUG) $(LDFLAGS)

run:
	./$(OUT)

clean:
	rm -f $(OUT)
	rm -f $(OUT-DEBUG)

.PHONY: build run clean compile
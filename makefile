COMPILER = clang++
CFLAGS = -std=c++17 -Wall
SDLFLAGS=${shell sdl2-config --cflags --libs}

# Sources
CSOURCE = ${wildcard ./*.cpp}
DLLSOURCE = ${wildcard include/*.h}
CPPSOURCE = ${wildcard src/*.cpp}

# Objects
OBJECTS = ${CSOURCE:.cpp=.o}

# Executables
EXECUTABLE = SABWR.exe
DLLOBJECTS = ${DLLSOURCE:include/%.h=lib/lib%.so}
DLLS = ${DLLOBJECTS:lib/lib%.so=-l%}

run: ${EXECUTABLE}
	./${EXECUTABLE}

${EXECUTABLE}: ${OBJECTS} ${DLLOBJECTS}
	${COMPILER} ${SDLFLAGS} ${CFLAGS} ${OBJECTS} -o $@ ${DLLS} -Wl,-rpath,./lib -L./lib -lSDL2_ttf

./lib/lib%.so: src/%.cpp include/%.h
	${COMPILER} -shared -fPIC $< -o $@

%.o: %.cpp
	${COMPILER} ${CFLAGS} -c $<

clean:
	rm -f ${EXECUTABLE} ./*.o ./lib/*.so
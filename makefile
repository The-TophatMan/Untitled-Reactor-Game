COMPILER = clang++
CFLAGS = -std=c++17 -Wall -g
SDLFLAGS=${shell sdl2-config --cflags --libs}

# Sources
CSOURCE = ${wildcard ./*.cpp}
DLLSOURCE = ${wildcard include/*.h}
CPPSOURCE = ${wildcard source/*.cpp}

# Objects
OBJECTS = ${CSOURCE:.cpp=.o}

# Executables
EXECUTABLE = MHR.exe
DLLOBJECTS = ${DLLSOURCE:include/%.h=lib/lib%.so}
DLLS = ${DLLOBJECTS:lib/lib%.so=-l%}

run: ${EXECUTABLE}
	./${EXECUTABLE}

${EXECUTABLE}: ${OBJECTS} ${DLLOBJECTS}
	${COMPILER} ${SDLFLAGS} ${CFLAGS} ${OBJECTS} -o $@ ${DLLS} -Wl,-rpath,./lib -L./lib -lSDL2_ttf

./lib/lib%.so: source/%.cpp include/%.h
	${COMPILER} -g -shared -fPIC $< -o $@

%.o: %.cpp
	${COMPILER} ${CFLAGS} -c $<

clean:
	rm -f ${EXECUTABLE} ./*.o ./lib/*.so ./*.log

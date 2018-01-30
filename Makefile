INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I ./include/vecmath

LINKFLAGS = -lglut -lGL -lGLU
LINKFLAGS += -L ./ -lvecmath

CFLAGS    = -O2 -Wall -ansi -DSOLN
CC        = g++
SRCS      = main.cpp
OBJS      = $(SRCS:.cpp=.o)
PROG      = a0

all: $(SRCS) $(PROG)

vecmath.a:
	g++ Matrix2f.cpp  Matrix3f.cpp  Matrix4f.cpp  Quat4f.cpp  Vector2f.cpp  Vector3f.cpp  Vector4f.cpp


$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

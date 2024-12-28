CFLAGS=-g -Wall -O0 -Wextra -Wno-unused-parameter -std=c++20 #-std=c++17 #-static # use -O2 when done
#REQOBJS=${OBJS} lib/mywebsocketsWrapper/mywebsockets/websockets.o lib/simdjson/simdjson.cpp
REQOBJS=${OBJS}
BUILD_DIR=out
EXECBIN=${BUILD_DIR}/RPI_LED${EXT}

CNV_INC=${CNV_INC}
CNVLIB=${CNVLIB}
#CXX="C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-gcc-10.exe"

# bruh: src/animation/gifdump.o src/test.cc
# 	g++ ${CFLAGS} src/animation/gifdump.o src/test.cc 

# src/animation/gifdump.o : src/animation/gifdump.cc
# 	g++ ${CFLAGS} -c -o src/animation/gifdump.o src/animation/gifdump.cc

# chb :
# 	valgrind --dsymutil=yes --track-origins=yes --tool=memcheck -v --leak-check=full --show-leak-kinds=all ./a.out


all :
	@echo "Run build.sh! Don't manually make!"

${EXECBIN} : ${REQOBJS} src/main.o
	$(CXX) ${CFLAGS} ${CNV_INC} -Ilib/ ${REQOBJS} ${PREPROC} src/main.o -o ${EXECBIN} ${CNV_LIB}

src/main.o : src/main.cc
	@echo hhiihiii ${CNV_INC}
	$(CXX) ${CFLAGS} ${CNV_INC} -Ilib/ ${BST_INC} ${PREPROC} -c -o $@ $< ${CNV_LIB}

src/canvas/canvas.o : ${CNV_SRC}
	$(CXX) ${CFLAGS} ${CNV_INC} ${PREPROC} -c -o $@ $< ${CNV_LIB}

%.o : %.cc
	$(CXX) $(CFLAGS) ${PREPROC} -c -o $@ $<

src/util/websockets.o : src/util/websockets.cc
	$(CXX) $(CFLAGS) ${BST_INC} ${PREPROC} -c -o $@ $<

clean : 
	find . -name '*.o' -not -path "*/lib/*" -print -delete

dryclean : 
	find . -name '*.o' -not -path "*/lib/*" -print

check :
	valgrind --dsymutil=yes --track-origins=yes --tool=memcheck -v --leak-check=full --show-leak-kinds=all ./${EXECBIN}
#valgrind --dsymutil=yes --track-origins=yes ./${EXECBIN}
#valgrind --dsymutil=yes --track-origins=yes --tool=memcheck -v --leak-check=full --show-leak-kinds=all ./${EXECBIN}
#valgrind --dsymutil=yes --track-origins=yes --tool=memcheck --log-file=valgrind.out ./${EXECBIN}

#all : ${EXECBIN}

#${EXECBIN} : lib/gradient.o



# all : $(REQOBJS)
# 	@echo $(FOO)
# 	@echo $(SWAG)

# **.o : README.md
# 	@echo "SWAG" $@

# Read the value of the environment variable and set it to a global variable,
# use "default_value" if the environment variable is not set.
#ENV_VAR_VALUE ?= $(shell echo $$ENV_VARIABLE_NAME)
# ENV_VAR_VALUE ?= $(shell echo $$ENV_VARIABLE_NAME)
# ENV_VAR_VALUE := $(or $(ENV_VAR_VALUE), default_value)

# all:
# 	@echo "Environment variable value: $(ENV_VAR_VALUE)"


# TODO read env var or passed variable (make VAR=val) to determine OS + which canvas to use
# canvas should be a precompiler directive (SFML or dummy canvas) and also used for includes

# ifeq ($(FOO),)         # check if FOO is not defined
# CFLAGS= -DFOO=999      # if not defined, set a default value
# else
# CFLAGS= -DFOO=$(FOO)   # or use the value from argument
# endif

# ENV_VAR_VALUE ?= $(shell echo $$ENV_VARIABLE_NAME)

# ifeq ($(ENV_VAR_VALUE), some_value)
#     # Actions to be executed when ENV_VAR_VALUE is "some_value"
#     all:
#         @echo "The environment variable has the value 'some_value'"
# else
#     # Actions to be executed when ENV_VAR_VALUE is not "some_value"
#     all:
#         @echo "The environment variable does not have the value 'some_value'"
# endif
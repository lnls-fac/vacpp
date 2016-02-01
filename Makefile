CXX = g++
MACHINE = -m64

OPTFLAG = -O3 -std=c++11
DBGFLAG = -O0 -g3 -std=c++11

INCDIR = include
SRCDIR = src
OBJDIR = build

ifeq ($(MAKECMDGOALS), debug)
MODEFLAG = $(DBGFLAG)
TGTDIR = debug
else
MODEFLAG = $(OPTFLAG)
TGTDIR = release
endif

LDFLAGS = $(MACHINE)
CFLAGS = $(MACHINE) $(MODEFLAG) $(DFLAGS) -pthread

INC = -I./$(INCDIR)
LIBS =

SRCS = main.cpp

OBJS = $(addprefix $(OBJDIR)/$(TGTDIR)/, $(SRCS:.cpp=.o))

.PHONY: all debug clean

all: $(OBJDIR)/$(TGTDIR)/vacpp

debug: all

# Find dependencies
$(shell $(CXX) -MM $(CFLAGS) $(INC) $(addprefix $(SRCDIR)/, $(SRCS)) | \
	sed 's/.*\.o/$(OBJDIR)\/$(TGTDIR)\/&/' > .depend)
-include .depend

$(OBJDIR)/$(TGTDIR)/vacpp: $(OBJS) | $(OBJDIR)/$(TGTDIR)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

$(OBJDIR)/$(TGTDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)/$(TGTDIR)
	$(CXX) -c $(CFLAGS) $(INC) $< -o $@

$(OBJDIR)/$(TGTDIR):
	mkdir -p $(OBJDIR)/$(TGTDIR)

clean:
	-rm -rf build .depend

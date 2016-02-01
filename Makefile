CXX = g++
MACHINE = -m64

OPTFLAG = -O3 -std=c++11
DBGFLAG = -O0 -g3 -std=c++11

INCDIR = include
SRCDIR = src
OBJDIR = build

ifdef RELEASE
MODEFLAG = OPTFLAG
TGTDIR = release
else
MODEFLAG = DBGFLAG
TGTDIR = debug
endif

LDFLAGS = $(MACHINE)
CFLAGS = $(MACHINE) $(DBGFLAG) $(DFLAGS) -pthread

INC = -I./$(INCDIR)
LIBS =

SRCS = main.cpp

OBJS = $(addprefix $(OBJDIR)/$(TGTDIR)/, $(SRCS:.cpp=.o))

.PHONY: all vacpp clean

all: vacpp

# Find dependencies
$(shell $(CXX) -MM $(CFLAGS) $(INC) $(addprefix $(SRCDIR)/, $(SRCS)) | \
	sed 's/.*\.o/$(OBJDIR)\/$(TGTDIR)\/&/' > .depend)
-include .depend

vacpp: $(OBJDIR)/$(TGTDIR)/vacpp

$(OBJDIR)/$(TGTDIR)/vacpp: $(OBJS) | $(OBJDIR)/$(TGTDIR)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

$(OBJDIR)/$(TGTDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)/$(TGTDIR)
	$(CXX) -c $(CFLAGS) $(INC) $< -o $@

$(OBJDIR)/$(TGTDIR):
	mkdir -p $(OBJDIR)/$(TGTDIR)

clean:
	-rm -rf build .depend

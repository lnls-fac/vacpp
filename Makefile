CXX = g++
MACHINE = -m64

OPTFLAG = -O3 -std=c++11
DBGFLAG = -O0 -g3 -std=c++11

INCDIR = include
SRCDIR = src
OBJDIR = build
PYDIR = python
PKGDIR = package

ifeq ($(MAKECMDGOALS), debug)
MODEFLAG = $(DBGFLAG)
TGTDIR = debug
else
MODEFLAG = $(OPTFLAG)
TGTDIR = release
endif

LDFLAGS = $(MACHINE)
CFLAGS = $(MACHINE) $(MODEFLAG) $(DFLAGS) -pthread -fPIC

INC = -I./$(INCDIR) -I/usr/include/python3.4
LIBS = -ltrackcpp

SRCS = \
	driver.cpp
BINSRC = main.cpp

OBJS = $(addprefix $(OBJDIR)/$(TGTDIR)/, $(SRCS:.cpp=.o))
BINOBJ = $(addprefix $(OBJDIR)/$(TGTDIR)/, $(BINSRC:.cpp=.o))

.PHONY: vacpp lib debug python package clean

vacpp: $(OBJDIR)/$(TGTDIR)/vacpp

package: lib | $(OBJDIR)/$(TGTDIR)/$(PKGDIR)
	cp $(PYDIR)/vacpp.py $(OBJDIR)/$(TGTDIR)/$(PKGDIR)
	cp $(OBJDIR)/$(TGTDIR)/libvacpp.so $(OBJDIR)/$(TGTDIR)/$(PKGDIR)/_vacpp.so

$(OBJDIR)/$(TGTDIR)/$(PKGDIR):
	mkdir -p $(OBJDIR)/$(TGTDIR)/$(PKGDIR)

lib: $(OBJDIR)/$(TGTDIR)/libvacpp.so

debug: vacpp

# Find dependencies
$(shell $(CXX) -MM $(CFLAGS) $(INC) $(addprefix $(SRCDIR)/, $(SRCS)) | \
	sed 's/.*\.o/$(OBJDIR)\/$(TGTDIR)\/&/' > .depend)
-include .depend

$(OBJDIR)/$(TGTDIR)/vacpp: $(OBJS) $(BINOBJ) | $(OBJDIR)/$(TGTDIR)
	$(CXX) $(LDFLAGS) $(OBJS) $(BINOBJ) $(LIBS) -o $@

$(OBJDIR)/$(TGTDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)/$(TGTDIR)
	$(CXX) -c $(CFLAGS) $(INC) $< -o $@

$(OBJDIR)/$(TGTDIR):
	mkdir -p $(OBJDIR)/$(TGTDIR)

$(OBJDIR)/$(TGTDIR)/libvacpp.so: python $(OBJS) | $(OBJDIR)/$(TGTDIR)
	$(CXX) -shared $(LDFLAGS) $(OBJS)  $(OBJDIR)/$(TGTDIR)/vacpp_wrap.o \
		$(LIBS) -o $@

python: $(PYDIR)/vacpp.i $(OBJDIR)/$(TGTDIR)/vacpp_wrap.o

$(OBJDIR)/$(TGTDIR)/vacpp_wrap.o: $(PYDIR)/vacpp_wrap.cxx | $(OBJDIR)/$(TGTDIR)
	$(CXX) -c $(CFLAGS) $(INC) $< -o $@

$(PYDIR)/vacpp_wrap.cxx:
	swig -c++ -python $(INC) $(PYDIR)/vacpp.i

clean:
	-rm -rf build .depend $(PYDIR)/vacpp.py $(PYDIR)/vacpp_wrap.cxx

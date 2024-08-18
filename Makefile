OUTDIR = ./out
CPP = clang++
CPPFLAGS = -g -Wall -std=c++14 -I$(INCDIR)
LDFLAGS = 
BINDIR = $(OUTDIR)/bin
OBJDIR = $(OUTDIR)/obj
SRCDIR = src
INCDIR = include
FILES = main.cpp lexer.cpp

all:
	make $(BINDIR)/spl

$(BINDIR)/spl: $(patsubst %.cpp,$(OBJDIR)/%.o,$(FILES))
	@mkdir -p $(BINDIR)
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@if [ ! -d $(OBJDIR) ]; then \
		mkdir -p $(OBJDIR); \
	fi
	
	$(CPP) -c $(CPPFLAGS) -o $@ $<

run: $(BINDIR)/spl $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(INCDIR)/*.h)
	./$<
	

clean:
	@if [ -d $(OUTDIR) ]; then \
		rm -rf $(OUTDIR); \
	fi
	@echo RM $(OUTDIR)

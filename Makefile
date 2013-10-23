LIBDIR=lib
OBJDIR=obj
INCLUDEDIR=include
SRCDIR=src
DOCDIR=doc
CFLAGS=-std=c++0x -Wall -Os -c -I$(INCLUDEDIR) -I/usr/include/
CP=cp
RM=rm -f
RMDIR=rm -rf
MKDIR=mkdir -p
CC=g++
CHMOD=chmod -R 777
NAME=ring

.PHONY: install clean uninstall

$(LIBDIR)/lib$(NAME).a: $(OBJDIR)/$(NAME).o
	$(MKDIR) $(LIBDIR)
	ar r $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ $<

install:
	$(CP) $(LIBDIR)/lib$(NAME).a /usr/lib/lib$(NAME).a
	$(CHMOD) /usr/lib/lib$(NAME).a
	$(MKDIR) /usr/include/$(NAME)/
	$(CHMOD) /usr/include/$(NAME)/
	$(CP) $(INCLUDEDIR)/* /usr/include/$(NAME)/

uninstall:
	$(RMDIR) /usr/include/$(NAME)
	$(RM) /usr/lib/lib$(NAME).a

clean:
	$(RMDIR) $(LIBDIR)
	$(RMDIR) $(OBJDIR)
	$(RMDIR) $(DOCDIR)

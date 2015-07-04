EDITOR = gedit
NOMBRE = ioport
COMPILADOR = gcc
FUENTES = ioport.c strnum.c strnum.h
OBJETOS = strnum.o
DESTINO = /usr/bin

all: $(NOMBRE)
$(NOMBRE): $(NOMBRE).c $(OBJETOS)
	$(COMPILADOR) -o $(NOMBRE) $(NOMBRE).c $(OBJETOS)
strnum.o: strnum.c strnum.h
	$(COMPILADOR) -o strnum.o -c strnum.c
clean:
	-rm -f $(OBJETOS) core $(NOMBRE)
cleanall:
	-rm -f $(OBJETOS) core $(NOMBRE) *~
edit:
	$(EDITOR) $(FUENTES) Makefile &
install:
	sudo chmod +x ioport
	sudo cp ioport $(DESTINO)
	sudo ln -s $(DESTINO)/ioport $(DESTINO)/inb
	sudo ln -s $(DESTINO)/ioport $(DESTINO)/vinb
	sudo ln -s $(DESTINO)/ioport $(DESTINO)/outb
	sudo ln -s $(DESTINO)/ioport $(DESTINO)/xoutb
uninstall:
	sudo rm -f $(DESTINO)/ioport
	sudo rm -f $(DESTINO)/inb
	sudo rm -f $(DESTINO)/vinb
	sudo rm -f $(DESTINO)/outb
	sudo rm -f $(DESTINO)/xoutb

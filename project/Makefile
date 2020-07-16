vn CFLAGS=-std=c11 -Wall -Wextra -pedantic -g -O2
FICHEIROS=cgi.h damas.c damas.h Makefile
EXECUTAVEL=Damas

install: $(EXECUTAVEL)
	sudo cp $(EXECUTAVEL) /usr/lib/cgi-bin
	touch install

OBJS = damas.o
$(EXECUTAVEL): $(OBJS)
	cc -o $(EXECUTAVEL) $(OBJS)

diretorias:
	sudo mkdir -p /usr/local/games/Damas
	sudo chmod -R a+rwx /usr/local/games/Damas/
	sudo cp estado.txt /usr/local/games/Damas
	sudo cp estadoInit.txt /usr/local/games/Damas

imagens:
	sudo mkdir -p /var/www/html/ImagensDamas
	sudo cp -R Imagens/*.png /var/www/html/ImagensDamas

Damas.zip: $(FICHEIROS)
	zip -9 Damas.zip $(FICHEIROS)

doc:
	doxygen

clean:
	rm -rf *.o $(EXECUTAVEL) Doxyfile latex html install
	sudo rm -rf /usr/local/games/Damas
	sudo rm -rf /var/www/html/ImagensDamas

damas.o: damas.c damas.h cgi.h
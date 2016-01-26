# Fichier Makefile racine

FOLDERS=webserver

.PHONY: all $(FOLDERS)

# s'execute si make est lance sans arguments
all: $(FOLDERS)

# Permet de lancer le make dans le repertoire webserver
webserver:
	make -C webserver

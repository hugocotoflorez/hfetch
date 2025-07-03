hfetch: fetch.c fetch.h
	gcc -Wall ./fetch.c -o hfetch

install: hfetch
	cp hfetch ~/.local/bin/

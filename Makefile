%.o: src/%.c src/net_lib.h src/net_data.h src/config_lib.h
	gcc -c -o $@ $< 

netmonitor: netmon.o net_lib.o net_data.o config_lib.o
	gcc -o $@ $^ 

.PHONY: clean install uninstall

clean:
	rm -f *.o  

install:
	install -m 0755 netmonitor /usr/local/bin
	test -d /usr/local/share/man/man1 || install -m 0775 -d /usr/local/share/man/man1
	install -m 0644 src/netmonitor.1 /usr/local/share/man/man1
	mandb

uninstall:
	rm /usr/local/bin/netmonitor
	rm /usr/local/share/man/man1/netmonitor.1
	mandb

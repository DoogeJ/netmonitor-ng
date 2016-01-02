# netmonitor-ng
https://github.com/DoogeJ/netmonitor-ng

This is a fork of Dwayne - hellcat82 - Hoy's Netmonitor on SourceForce, since this project is inactive since 2005.

Original project homepage: https://sourceforge.net/projects/netmonitor/

##ABOUT
netmonitor is a console application that monitors network interfaces.

It displays the transfer rate (upload and download) for each device and the data consumed by each device.

The main features of netmonitor are low resource usage, a simple yet effective design, and an easy to understand tabular output.

It also logs this data so that it can retain these values after a crash or restart.

It displays values whose units change dynamically to scale with the value.

##COMPILING
To compile and install netmonitor type the following commands

````sh
make 

make clean

make install
````

make install must be run as superuser.

##UNINSTALLING
To uninstall netmonitor type the collowing command as superuser

````sh
make uninstall
````

Data will be retained but is saved in ~/.netmonitor

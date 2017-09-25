# SnickerOS

A from-baremetal-to-OS project targetting the Krtkl Snickerdoodle. Currently in very-early development. 

## Toolchain 

Uses the Platform Cable USB II to program the target. If you have this setup and want to replicate, run: 
  > toolchain/debugsrv.sh
  > xmd
  > connect arm hw
  > dow ../src/main.elf
This exposes a fully-functional GDB interface which you can connect to with another terminal using:
  > toolchain/gdb_setup.sh
  
## State of project:

There isn't any real OS here yet. At the moment, it's mostly baremetal dev tool construction. First priority is accurate debugging and memory management, followed by filesystem integration. Malloc is currently fully functioning, with free being developed. 

## License

This code is open-source and free to use for whatever you want. 

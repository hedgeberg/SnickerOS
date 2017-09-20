. settings64.sh

HW_SERVER_ALLOW_PL_ACCESS=1 hw_server -Llogfile.txt -l0xFC00  &

xsdb -interactive setup.tcl

killall hw_server 

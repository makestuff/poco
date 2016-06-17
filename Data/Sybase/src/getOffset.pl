#!/usr/bin/perl
#
# Find offset from global label "sybnet_exit" to local label "$sybnet__exithandler":
#
# objdump -T -t $SYBASE_OCS/lib/libsybtcl64.so | ./getOffset.pl
#
# This offset can be used in an ugly hack to ensure sybnet__exithandler() is actually called. For
# example, in Connector::~Connector():
#
# extern "C" void sybnet_exit(void);
# #pragma GCC diagnostic push
# #pragma GCC diagnostic ignored "-Wpedantic"
# char *hackChar = ((char*)sybnet_exit) + 0x40;  // <-- offset here
# typedef void (*FuncPtr)(void);
# FuncPtr hackFunc = (FuncPtr)hackChar;
# #pragma GCC diagnostic pop
# hackFunc();
#
my ($sybnet_exit, $sybnet__exithandler);

while ( <STDIN> ) {
	 if ( m/^0*([0-9a-f]+) g\s+F .text\s+0*[0-9a-f]+\s+sybnet_exit$/ ) {
		  $sybnet_exit = $1;
	 }
	 if ( m/^0*([0-9a-f]+) l\s+F .text\s+0*[0-9a-f]+\s+sybnet__exithandler$/ ) {
		  $sybnet__exithandler = $1;
	 }
}
printf("0x%04X\n", hex($sybnet__exithandler) - hex($sybnet_exit));

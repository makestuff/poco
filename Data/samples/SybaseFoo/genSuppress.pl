#!/usr/bin/perl
#
# (valgrind --gen-suppressions=all --show-leak-kinds=all --leak-check=full bin/Linux/x86_64/SybaseFood 2>&1) > raw.txt
# e raw.txt
# cat raw.txt | ./genSuppress.pl
# md5sum file*.txt | sort
# rm ...
# cat test*.txt > my-suppressions.txt
# rm test*.txt
#
my $i = 0, $fileName, $fileHandle;
while ( <STDIN> ) {
	chomp;
	if ( !m/^==.*?$/ ) {
		if ( $_ eq "{" ) {
			$fileName = sprintf("file%02d.txt", $i);
			open($fileHandle, '>', $fileName) or die "Could not open file '$fileName' $!";
			print $fileHandle "{\n";
		} elsif ( $_ eq "}" ) {
			print $fileHandle "}\n";
			close $fileHandle;
			$i++;
		} else {
			print $fileHandle "$_\n";
		}
	}
}
# TODO: put each suppression into a hash-set

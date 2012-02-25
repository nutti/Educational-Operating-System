#!/usr/bin/perl

use strict;

my $fin_name = @ARGV[ 0 ];
my $fout_name = @ARGV[ 1 ];

unless( -e $fin_name ){
	die "$fin_name isn't exist\n";
}

if( -e $fout_name ){
	die "$fout_name is already exist\n";
}

open FIN, "<$fin_name" or die "Failed to open $fin_name";
open FOUT, ">$fout_name" or die "Failed to open $fout_name";

print FOUT ".code32\n";
print FOUT ".global std_font\n";
print FOUT "std_font:\n";

while( <FIN> ){
	s/[.]/.byte 0\n/g;
	s/[*]/.byte 1\n/g;
	print FOUT $_;
}

close FIN;
close FOUT;

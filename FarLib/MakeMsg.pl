#!/usr/bin/perl -w
# MakeMsg Input.msg Header.h First.lng[=cp] [Second.lng...]
use Data::Dumper;
use File::BOM qw(:all);
die "Insifficient parameters\n" if @ARGV < 3;

my $Input = shift @ARGV;
my $Header = shift @ARGV;
my @Language = map { /^(.*)=(.*)$/ ? {'file'=>$1, 'cp'=>$2} : /rus\./i ? {'file'=>$_, 'cp'=>'cp866'} : {'file'=>$_, 'cp'=>'cp850'} } @ARGV;
my @Lang = ();
my $Line;

open(HDR, "<$Header") or die "Cannot open '$Header': $!\n";
open(HDROUT, ">${Header}_out") or die "Cannot create '${Header}_out': $!\n";
while (defined($Line=<HDR>)) {
	chomp $Line;
	print HDROUT "$Line\n";
	last if $Line eq 'enum eStringTable {';
}
while (defined($Line=<HDR>)) {
	chomp $Line;
	last if $Line eq '};';
}

#open(MSG, '<:via(File::BOM)', $Input) or die "Cannot open '$Input': $!\n";
open_bom(MSG, $Input, ':encoding(cp866)');

$Line=<MSG>;
chomp $Line;
my @Fields = split("\t+", $Line);
die "Invalid first line: '$Line'\n" if shift @Fields ne ".Language";

for (my $Lng = 0; $Lng < @Fields; $Lng++) {
	my $File = $Language[$Lng]->{'file'};
	my $Enc  = '>:encoding('.$Language[$Lng]->{'cp'}.')';

	open($Lang[$Lng], $Enc, $File) or die "Cannot create '$File': $!\n";
	print {$Lang[$Lng]} ".Language=$Fields[$Lng]\n\n";
}

while (defined($Line=<MSG>)) {
#	chomp $Line;
	$Line =~ s/\s+$//;
	next if $Line =~ /^(#|\/\/|;)/;

	if ($Line =~ /^\s*$/) {
		print HDROUT "\n";
		for (my $Lng = 0; $Lng < @Lang; $Lng++) {print {$Lang[$Lng]} "\n";}
	} else {
		@Fields = split("\t+", $Line);

		print HDROUT "\t$Fields[0],\n";
		for (my $Lng = 0; $Lng < @Lang; $Lng++) {
			my $Field = $Fields[$Lng+1];
			$Field = $Fields[1] unless defined $Field;
			$Field = $Fields[0] unless defined $Field;
			print {$Lang[$Lng]} $Field, "\n";
		}
	}
}

print HDROUT "};\n";
while (defined($Line=<HDR>)) {
	print HDROUT $Line;
}
close(HDROUT);
close(HDR);
rename("${Header}_out", $Header);

close(MSG);

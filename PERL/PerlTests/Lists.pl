#!/usr/bin/perl

@mai = (A .. Z);
@min = (a .. z);

push(@mai, @min);

$mai1 = join( ' | ', @mai );
$file = "Out.txt"; # /tmp/file.txt
open($fh, '>', $file) or die "Nao foi possivel abrir o arquivo '$file' $!";

print $fh "$mai1\n";

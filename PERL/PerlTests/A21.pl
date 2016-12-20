#!/usr/bin/perl

#S21
$s = "frase com ausencia de criatividade";
print "s = $s\n";

$s =~ s/a/A/; #Primeiro a por A
print "s = $s\n";

$s = "frase com ausencia de criatividade";
$s =~ s/a/A/g; #a's por A's
print "s = $s\n";

$s = "frase com ausencia de criatividade";
$s =~ s/(.*)d/$1.D/e; #Último d por D
print "s = $s\n";

$s = "frase com ausencia de criatividade";
$s =~ s/[ea]/1/g; #a's e e's por 1
print "s = $s\n";

$s = "frase com ausencia de criatividade";
$s =~ s/e$/E/; #e no final da string por E
print "s = $s\n";

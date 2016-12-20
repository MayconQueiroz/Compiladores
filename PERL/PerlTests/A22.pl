#!/usr/bin/perl

#S22
$s = "frase com ausencia de criatividade";
print "s = $s\n";

$s =~ tr/[a-z]/[A-Z]/; #uppercase
print "s = $s\n";

$s = "frase com ausencia de criatividade";
$s =~ tr/ae/1/; #a's e e's por 1
print "s = $s\n";

$s = "frase com ausencia de criatividade";
$s =~ tr/ae/12/; #a's por 1 e e's por 2
print "s = $s\n";

$s = "frase com ausencia de criatividade";
$s =~ tr/ae/1/d; #a's por 1 e apaga e's
print "s = $s\n";

$s = "frase com ausencia de criatividade";
$s =~ tr/ae/1/c; #todo o resto vira 1
print "s = $s\n";

$s = "frase com ausencia de criatividade";
$s =~ tr/a/12/; #a's viram 1 (ignora 2)
print "s = $s\n";

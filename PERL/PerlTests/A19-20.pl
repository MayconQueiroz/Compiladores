#!/usr/bin/perl

#S19-S20
$s = "frase com ausencia de criatividade";

if ($s =~ m/sencia/){
  print "sencia na frase\n";
}
if ($s =~ /CRIAT/i){
  print "CRIAT na frase\n";
}
if ($s =~ m/e+/){
  print "e pelo o menos uma vez\n";
}
if ($s =~ m/x+/){
  print "x pelo o menos uma vez\n";
}
if ($s =~ m/\s/){
  print "espaco na frase\n";
}
if ($s =~ m/\d/){
  print "digitos na frase\n";
}

#!/usr/bin/perl

#S19-S20
$T = "0001122222";

for (my $p = 0; $p < 40; $p++){
  for (my $s = 1; $s < 39; $s++){
    $t = $p + $s;
    if ($T =~ m/0{$p}1{$s}2{$t}/){
      print "p = $p - s = $s - t = $t\n";
    }
  }
}

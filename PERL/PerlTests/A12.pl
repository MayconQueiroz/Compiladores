#!/usr/bin/perl

$var = 10;
$a = 5;
$b = 1;

for (my $i = 0; $i < $a; $i++) {
  if ($i == 4){
    $a = 10;
  }
  print "i = $i - a = $a\n";
}

print "---------------------\n";

for (my $i = 0; $i < $a; $i=$i+$b) {
  if ($i == 4){
    $b = 2;
  }
  print "i = $i - b = $b\n";
}

print "---------------------\n";

$c = 0;
for (my $i = 0; $i < $a; $i++) {
  if ($i == 4 && $c == 0){
    print "i = $i - c = $b\n";
    $c = 1;
    redo;
  }
  print "i = $i - b = $b\n";
}

#!/usr/bin/perl

$a = 3;
$b = 2;
$t = $a / $b;
for (; $t < 50; $t++){
  print "t = $t\n";
}

print "STDIN iminente > ";
$t = <STDIN>;
if ($t < 50.0){
  print "t<\n";
}

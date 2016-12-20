#!/usr/bin/perl

($first, $second, $third) = (20, 40, 60);

print "First = $first\n";
print "Second = $second\n";
print "Third = $third\n";

($first, $second) = ($second, $first);

print "2First = $first\n";
print "2Second = $second\n";

@list = (5,4,3,2,1)[1..3];

print "List = @list\n";

$a = 45;
$b = "alfa";

$c = $b / $a;

print "C = $c\n";

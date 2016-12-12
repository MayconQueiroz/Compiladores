#!/usr/bin/perl

$var1 = "Nome";
$var2 = "Kome0";
$var3 = "NOME";

if ($var1 == $var2){
  print "var1 == var2\n";
}

if ($var1 == $var3){
  print "var1 == var3\n";
}
$a = ($var1 <=> $var2);

if ($a == 0){
  print "var1 <=> var2\n";
}

if ($var1 eq $var2){
  print "var1 eq var2\n";
}

if ($var1 eq $var3){
  print "var1 eq var3\n";
}

if (9 gt 8){
  print "Nome2 eq Nome2\n";
}

@a = "primeiro " . "segundo";
print "---a = @a[1]\n";

@b = ("abaco " x 3);
print "---b = @b[1]\n";

@c = (2..10);
print "---c = @c\n";

@d = ('a'..'f');
print "---c = @d\n";

$dir = <STDIN>;

print "===$dir\n";

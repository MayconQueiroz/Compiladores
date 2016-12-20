#!/usr/bin/perl

#S08-S09
$exp = 2 ** 16;

print "Exp = $exp\n";

$var1 = "Nome";
$var2 = "Lome0";
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

if ($var1 gt $var2){
  print "var1 gt var2\n";
}

if ($var1 gt $var3){
  print "var1 gt var3\n";
}

if (9 gt 8){
  print "9 gt 8\n";
}

$a = "primeiro " . "segundo";
print "---a = $a\n";

$b = ("terceiro " x 3);
print "---b = $b\n";

@c = (2..10);
print "---c = @c\n";

@d = ('a'..'f');
print "---d = @d\n";

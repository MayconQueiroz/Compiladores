#!/usr/bin/perl

#=begin comment
#S13-S14
$string = "Hello, World!";

sub PrintHello{
   my $string;
   $string = "Hello, Perl!";
   PrintMe();
   print "Inside the function PrintHello $string\n";
}
sub PrintMe{
   print "Inside the function PrintMe $string\n";
}
PrintHello();
print "Outside the function $string\n";

$var = 5;
print "Var     = $var\n";
for (my $var = 0; $var < 3; $var++) {
  print "Varfor  = $var\n";
  my $var = 4;
  print "Varfor2 = $var\n";
}
print "Var     = $var\n";
#=cut

=begin comment
#S15
sub PrintHash{
   my (%hash) = @_;

   foreach $item (%hash){
      print "Item : $item\n";
   }
}
%hash = ('name' => 'Tom', 'age' => 19);
PrintHash(%hash);

sub Soma{
  $soma = 0;
  foreach $item (@_){
    $soma += $item;
  }
  return $soma;
}

@list = (10, 20, 30);
$cref = \&Soma;
$result = &$cref(@list);
print "Soma = $soma\n";
=cut

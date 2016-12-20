#!/usr/bin/perl

#Teste de Legibilidade em Perl
#S24
sub ImprimeLista2{
  foreach (@_){
     if (/e/){
       s/(\w)(\w+)/uc($1).$2/e;
       print;
     }
  }
}

sub ImprimeLista1{
  $tam = @_;
  my $a = 0;
  for (my $i = 0; $i < $tam ; $i++) {
    if ($_[$i] =~ m/.*e.*/){
      print "\u$_[$i]";
    }
  }
}

@Lista = ("pollo", "sandia", "madre", "helado", "nublarse", "gafas", "empezar", "coser", "basura", "vereda");

ImprimeLista1(@Lista);
print "\n";
ImprimeLista2(@Lista);
print "\n";

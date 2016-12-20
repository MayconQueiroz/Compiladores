#!/usr/bin/perl

#S16 aux
=begin comment
sub Soma{
   #Quando uma funcao recebe variaveis, essas vao para o array subtraco (ou traco inferior); na linha abaixo, tam recebe o tamanho do array subtraco
   $tam = @_;
   $soma = 0;
   @E = @_;
   print "@E\n";
   print "$E[0] ";
   print "$E[1] ";
   print "$E[2] ";
   print "$E[3] \n";
   # get total number of arguments passed.
   for (my $i = 0; $i < $tam ; $i++) {
     print "somando $E[$i] para i = $i\n";
     $soma = $soma + $_[$i];
   }


   print "A soma dos numeros e : $soma\n";
}

Soma(10, 20, 30, 40);
=cut

=begin comment
sub Reverse{
  @A = undef;
  $o = 0;
  for(my $i = (@_ - 1); $i >= 0; $i--){
    @A[$o] = $_[$i];
    $o++;
  }
  return @A;
}

@R = Reverse(10,20,30,40,50);
print "Reverse = @R\n";
=cut

=begin comment
sub HashTest{
  my %aa = @_;
  return %aa;
}

%K = HashTest("Ab", 5, "Bc", 6, "Cd", 7, "De", 8);
=cut

=begin comment
sub ReferenceTest{
  my $L = $_[0];
  $p = \$L;
  return $p;
}

print "\$K{'Ab'} = $K{'Ab'}\n";
print "\$K{'Bc'} = $K{'Bc'}\n";
print "\$K{'Cd'} = $K{'Cd'}\n";
print "\$K{'De'} = $K{'De'}\n";

$re = ReferenceTest(50);

print "Valor em re = $$re\n";

$Ka = $re;

print "Valor em Ka = $Ka";
=cut

=begin comment
sub Soma{
  $soma = 0;
  foreach $item (@_){
    $soma += $item;
  }
  return $soma;
}
sub Funcao{
  return Soma(@_);
}

@list = (10, 20, 30);
$v = Funcao(@list);
print "v = $v\n";
=cut

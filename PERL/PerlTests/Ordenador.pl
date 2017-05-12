#!/usr/bin/perl

#Ordenador de elementos em vetor usando estatistica
#Codigo abandonado por ineficiencia
#Maycon Q

#Verifica se estao em ordem
sub Doublecheck{
  $ORD = 0;
  for (my $i = 0; $i < $TAM-1; $i++) {
    $ORD = 1 if $_[$i] > $_[$i+1];
  }
}

#Inicia TAM, MED, MIN e MAX
sub Iniciais{
  $TAM = @_;
  $MAX = $_[0];
  $MIN = $_[$TAM-1];
  foreach (@_){
    $S += $_;
    $MIN = $_ if $_ < $MIN;
    $MAX = $_ if $_ > $MAX;
  }
  $MED = $S / $TAM;
}

#Coloca os elementos de L1 em L2
#De acordo com a distribuicao de media
#Onde os elementos menores que a media vao parar no comeco do vetor
#Tentativa de adiantar o caminhar, mas parece que nao esta dando certo
sub Carrega12{
  my $B = 0;
  my $E = $TAM-1;
  foreach (@_){
    if ($_ < $MED){
      @L2[$B] = $_;
      $B++;
    }
    else {
      @L2[$E] = $_;
      $E--;
    }
  }
}

#Funcao do R quadrado
#Nao funciona, nao consegui terminar
sub Rquad{
  foreach (@_){
    $SOMQ += ($_ - $MED) ** 2;
  }
}

open(IN, "<Entr.txt") or die "Nao deu pra abrir o arquivo $!";
open(OT, ">Said.txt") or die "Nao deu pra abrir o arquivo $!";
chomp(@L1 = <IN>);
close(IN);
$TAM = @L1;
Iniciais(@L1);
@L3 = (1..$TAM);
#Nada acima daqui!
Carrega12(@L1);
#Rquad(@L2);
foreach (@L2){
  print OT "$_\n";
}

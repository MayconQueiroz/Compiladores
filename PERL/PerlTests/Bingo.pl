#!/usr/bin/perl
use Win32::Console::ANSI;
use Term::ANSIColor qw(:constants);

#Programa que verifica cartelas de bingo
#Maycon Q

#Obs.: As cartelas entram pelo arquivo Entr.txt
#      Cada linha e uma cartela e As linhas devem ter a mesma quantia de numeros
#      Zero e usado para sair do programa. Windows Master Race.

sub countCPB{
  for (my $i = 0; $i < $TC; $i++){
    if ($O[$i][$T+1] == $T-1){
      $CPB++;
    }
  }
}

sub doublecheck{
  for (my $i = 0; $i < $TC; $i++){
    for (my $j = 0; $j < $T; $j++) {
      if ($C[$i][$j] == $A){
        $O[$i][$j] = 1;
        $O[$i][$T+1] = $O[$i][$T+1] + 1;
        if ($O[$i][$T+1] == $T){
          $A = 0;
          $G = $i;
        }
      }
    }
  }
}

sub printCartelas{
  for (my $i = 0; $i < $TC; $i++){
    if ($i < 10){
      print " ";
    }
    print "$i - ";
    print RESET;
    for (my $j = 0; $j < $T; $j++) {
      if ($C[$i][$j] < 10){
        print " ";
      }
      if ($O[$i][$j] == 1){
        print YELLOW, ON_BLACK, "$C[$i][$j] ";
      }
      else {
        print "$C[$i][$j] ";
      }
      print RESET;
    }
    if ($O[$i][$T+1] == $T-1){
      print YELLOW, ON_BLACK, "- $O[$i][$T+1]\n";
    }
    elsif ($O[$i][$T+1] == $T){
      print GREEN, ON_BLACK, "- $O[$i][$T+1] FECHOU\n";
    }
    else{
      print "- $O[$i][$T+1]\n";
    }
    print RESET;
  }
}

open(IN, "<Entr.txt") or die "Nao deu pra abrir o arquivo $!";
chomp(@K = <IN>);
close(IN);
$TC = @K;

@J = undef;
@C = undef;
for (my $v1 = 0; $v1 < $TC; $v1++) {
  @J = split(' ', $K[$v1]);
  $T = @J;
  for (my $j = 0; $j < $T; $j++) {
    $C[$v1][$j] = $J[$j];
  }
}
print "T = $T\n";
@O = undef;
for (my $v1 = 0; $v1 < $TC; $v1++) {
  for (my $v2 = 0; $v2 < $T+1; $v2++) {
    $O[$v1][$v2] = 0;
  }
}
$A = 1;
$G = undef;
while ($A != 0){
  $CPB = 0;
  countCPB();
  printCartelas();
  if ($CPB > 0){
    print "$CPB Cartelas com um numero\n";
  }
  print "-----------------------------------------------------------\n";
  print "Numero: ";
  $A = <STDIN>;
  doublecheck();
  print "-----------------------------------------------------------\n";
}
print "-----------------------------------------------------------\n";
if ($G != undef){
  printCartelas();
  print "A cartela $G ganhou\n";
}
print "-----------------------------------------------------------\n";

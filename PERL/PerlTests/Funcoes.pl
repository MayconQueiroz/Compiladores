#!/usr/bin/perl

# Function definition
sub Hello{
   print "Hello, World!\n";
}

# Function call
Hello();

# Function definition
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

# Function call
Soma(10, 20, 30, 40);

#!/usr/bin/perl

#S16

#Definicao de formato
format EMPLOYEE =
@<<<<<<<<<<<<<<<<<<<<<< @<<  @#####.##
$name, $age, $salary
--------------------------------------
.
format EMPLOYEE_TOP =
======================================
Nome                   Idade   Salario
======================================
.

select(STDOUT);
$~ = EMPLOYEE;
$^ = EMPLOYEE_TOP;

@n = ("Empregado1", "Empregado2", "Empregado3", "Empregado4");
@a = (20, 30, 4005, 3.57);
@s = (2000.00, 2500.00, 4000000.000, 458.587);

$i = 0;
foreach (@n){
   $name = $_;
   $age = $a[$i];
   $salary = $s[$i++];
   write;
}

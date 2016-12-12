#!/usr/bin/perl

$age = 25;             # An integer assignment
$name = "John Paul";   # A string
$salary = 1445.50;     # A floating point
$age++;

print "Age = $age\n";
print "Name = $name\n";
print "Salary = $salary\n";

$age = "My new variable";
$name = " fim!";
$salary = $age . $name;

print "New Age = $age\n";
print "New Name = $name\n";
print "New Salary = $salary\n";

@ages = (25, 30, 40);
@names = ("John Paul", "Lisa", "Kumar");

print "ages = @ages\n";
print "ages[0] = $ages[0]\n";
print "ages[1] = $ages[1]\n";
print "ages[2] = $ages[2]\n";
print "names[0] = $names[0]\n";
print "names[1] = $names[1]\n";
print "names[2] = $names[2]\n";

@days = qw/Mon Tue Wed Thu Fri Sat Sun/;

print "$days[0]\n";
print "$days[1]\n";
print "$days[2]\n";
print "$days[6]\n";
print "$days[-1]\n";
print "$days[-7]\n";

#################################Hash
$data{'Adicao'} = 80;
print "......$data{'Adicao'}\n";


%data = ('John Paul' => 45, 'Lisa' => 30, 'Kumar' => 40);

$data{'Adicao'} = 80;

print "$data{'John Paul'}\n";
print "$data{'Lisa'}\n";
print "$data{'Kumar'}\n";
print "$data{'Adicao'}\n";

print "\n------------------------------------------\n$data{'Adicao'}\n";


####################################PONTEIROS
$var = 10;

# Now $r has reference to $var scalar.
$r = \$var;

# Print value available at the location stored in $r.
print "Value of $var is : ", $$r, "\n";

@var = (1, 2, 3);
# Now $r has reference to @var array.
$r = \@var;
# Print values available at the location stored in $r.
print "Value of @var is : ",  @$r, "\n";

%var = ('key1' => 10, 'key2' => 20);
# Now $r has reference to %var hash.
$r = \%var;
# Print values available at the location stored in $r.
print "Value of %var is : ", %$r, "\n";

#######################################
$var = 10;
$r = \$var;
print "Reference type in r : ", ref($r), "\n";

@var = (1, 2, 3);
$r = \@var;
print "Reference type in r : ", ref($r), "\n";

%var = ('key1' => 10, 'key2' => 20);
$r = \%var;
print "Reference type in r : ", ref($r), "\n";

################
#Devem imprimir
#Reference type in r : SCALAR
#Reference type in r : ARRAY
#Reference type in r : HASH

#Definicao de formato
format EMPLOYEE =
@<<<<<<<<<<<<<<<<<<<<<< @<<  @#####.##
$name $age $salary
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

=begin comment
===================================
Ali                     20
  2000.00
===================================
===================================
Raza                    30
  2500.00
===================================
===================================
Jaffer                  40
  4000.00
===================================
=cut

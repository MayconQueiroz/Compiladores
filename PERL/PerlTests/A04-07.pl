#!/usr/bin/perl

#S04~S07
$age = 25;
$name = "John Paul";
$salary = 1445.50;

print "Age = $age\n";
print "Name = $name\n";
print "Salary = $salary\n";

####################################LISTA
@ages = (40, 60, 30);
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

####################################HASH
$data{'Adicao'} = 80;
print "...$data{'Adicao'}\n";

%data = ('John Paul' => 45, 'Lisa' => 30, 'Kumar' => 40);

$data{'Adicao2'} = 80;

print "---$data{'Adicao'}\n";
print "$data{'John Paul'}\n";
print "$data{'Lisa'}\n";
print "$data{'Kumar'}\n";
print "$data{'Adicao2'}\n";

####################################PONTEIROS
$var = 10;
$r = \$var;
print "Value of $var is : ", $$r, "\n";
print "Reference type in r : ", ref($r), "\n";

@var = (1, 2, 3);
$r = \@var;
print "Value of @var is : ",  @$r, "\n";
print "Reference type in r : ", ref($r), "\n";

%var = ('key1' => 10, 'key2' => 20);
$r = \%var;
print "Value of %var is : ", %$r, "\n";
print "Reference type in r : ", ref($r), "\n";

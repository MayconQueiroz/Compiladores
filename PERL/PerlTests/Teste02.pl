#!/usr/bin/perl

=begin comment
@var_10 = (1..10);
#@var_20 = (10..20);
#@var_abc = (a..z);

print "@var_10\n";   # Prints number from 1 to 10
#print "@var_20\n";   # Prints number from 10 to 20
#print "@var_abc\n";  # Prints number from a to z

@var_10 = @var_10 * 10;
print "--------------------------------\n.@var_10.\n";

# define Strings
$var_string = "Rain-Drops-On-Roses-And-Whiskers-On-Kittens";
$var_names = "Larry,David,Roger,Ken,Michael,Tom";

# transform above strings into arrays.
@string = split('-', $var_string);
@names  = split(',', $var_names);

# transform above arrays into strings.
$string1 = join( '-', @string );
$string2 = join( ',', @names );

print "$string1\n";
print "$string2\n";
=cut

@array = qw/This is an array you fool/;

print "@array\n";
$exam = @array;

print "$exam";

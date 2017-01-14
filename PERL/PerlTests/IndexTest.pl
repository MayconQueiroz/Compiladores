#!/usr/bin/perl
my $dir = '/';

opendir(DIR, $dir) or die $!;

while (my $file = readdir(DIR)) {
  # Use a regular expression to ignore files beginning with a period
  #next if ($file =~ m/^\./);
  print "$file\n";
}
closedir(DIR);
exit 0;

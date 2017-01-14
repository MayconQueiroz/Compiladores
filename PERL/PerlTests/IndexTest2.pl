#!/usr/bin/perl

use autodie; # die if problem reading or writing a file

sub search_dir {
  my ($dir) = @_;
  my $dh;
  if ( !opendir ($dh, $dir)) {
    warn "Unable to open $dir: $!\n";
    return;
  }
  # Two dummy reads for . & ..
  readdir ($dh);
  readdir ($dh);
  while (my $file = readdir ($dh) ) {
    my $path = "$dir/$file";    # / should work on UNIX & Win32
    if ( -d $path ) {
      print "Pasta $path\n";
      search_dir ($path);
    }
    else {
      print $fh "$path\n";
    }
  }
  closedir ($dh);
}
##########################################################
#print "Enter The Directory Path";
$file = "Latimore2.txt"; # /tmp/file.txt
open($fh, '>', $file) or die "Não foi possível abrir o arquivo '$file' $!";
$dir = '/Index';
#my $dir = <STDIN>;
chomp($dir);
search_dir($dir);

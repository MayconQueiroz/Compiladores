#!/usr/bin/perl
use strict;
use warnings;

use Path::Class;

my $dir = dir(); # Diretorio corrente

# Vai ate o ultimo cabra da pasta
while (my $file = $dir->next) {
    
    # Se for um diretorio, pula
    next if $file->is_dir();
    
    # Imprime
    print $file->stringify . "\n";
}
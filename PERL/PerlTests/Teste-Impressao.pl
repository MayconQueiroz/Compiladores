#!/usr/bin/perl

#Impressao
print "This is a single statement.\n";
print "Look, ", "a ", "list!\n";
@lista = ("primeiro", "segundo", "terceiro", "quarto");
print @lista;

#Impressao em varias linhas
print "\nHello
          world\n";

#Diferenca entre aspas e apostrofo
print "Hello, world\n";
print 'Hello, world\n';

$a = 10;
print "Value of a = $a\n";
print 'Value of a = $a\n';

############################ Nao documentado, testar para obter resultados

#Variavel usando EOF, o EOF pode ser substituido contanto que seja substituido la embaixo
$var = <<"EOF";
This is the syntax for here document and it will continue
until it encounters a EOF in the first line.
This is case of double quote so variable value will be
interpolated. For example value of a = $a
EOF
print "\n------------------------------\n$var\n";

#Apostrofo
$var = <<'EOF';
This is case of single quote so variable value will not be
interpolated. For example value of a = $a
EOF
print "--------------------------------\n$var\n";

#!/usr/bin/perl

# https://blogs.perl.org/users/aristotle/2018/11/modern-perl-cgi.html
use strict;
use warnings;
use CGI;
use Encode::Simple;

my $cgi = CGI->new;
my $word;
eval {
    $word = decode 'UTF-8', scalar $cgi->param('word');
};
$word = defined $word ? $word : 'No word provided';
print $cgi->header('text/html; charset=UTF-8');
print <<"END_HTML";
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>CGI Test Perl 1 - WORD</title>
</head>
<body>
    <h1>CGI Test Perl 1 - WORD</h1>
    <h2>Word: $word</h2>
</body>
</html>
END_HTML

print "<h1>CGI Test Perl 2 - ENV</h1>\r\n";
foreach my $key (keys %ENV) {
    print "$key <==> $ENV{$key}<br>";
}
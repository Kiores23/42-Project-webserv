#!/usr/bin/perl

use strict;
use warnings;

print "Content-type: text/html\n\n";
print "<html>\n";
print "<head>\n";
print "<title>CGI Test Perl 1 - ENV</title>\n";
print "</head>\n";
print "<body>\n";
print "<h1>CGI Test Perl 1 - ENV</h1>\n";

foreach my $key (sort keys %ENV) {
    print "<p>" . sprintf("%20s = %s", $key, $ENV{$key}) . "</p>\n";
}

print "</body>\n";
print "</html>\n";

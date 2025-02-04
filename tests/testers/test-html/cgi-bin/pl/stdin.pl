#!/usr/bin/perl

use strict;
use warnings;
use IO::Select;

my $timeout = 3;
my $selector = IO::Select->new();
$selector->add(\*STDIN);

print "Content-type: text/html\n\n";
print "<html>\n";
print "<head>\n";
print "<title>CGI Test Perl - stdin.pl</title>\n";
print "</head>\n";
print "<body>\n";
print "<h1>CGI Test Perl - stdin.pl</h1><p>\n";

if (my @ready = $selector->can_read($timeout)) {
    my $input_data = <STDIN>;
    if (!defined $input_data || $input_data eq "") {
        print "Server has no data to send\n";
    } else {
        print "Server sent data: $input_data\n";
    }
} else {
    print "Server has sent no data received within $timeout seconds.\n";
}

print "<p></body>\n";
print "</html>\n";

#! /usr/bin/perl -wT
use strict;
use CGI;

my $q = new CGI;

print $q->header,
      $q->start_html("hello world"),
      $q->h1("Hello world"),
      $q->end_html;

=pod
print "$ENV{SERVER_PROTOCOL} 200 OK\n";
print "Server: $ENV{SERVER_SOFTWARE}\n";
print "Content-Type: text/html\n";
print "Content-Length: 200\n";
print "\n";

my $body = "
<html>
  <head>
    <title>About this server</title>
  </head>
  <body>
    <h1>About this server</h1>
    <hr>
    <pre>
      server name: $ENV{SERVER_NAME}
      server port: $ENV{SERVER_PORT}
      server software: $ENV{SERVER_SOFTWARE}
      server protocol: $ENV{SERVER_PROTOCOL}
      CGI version: $ENV{GATEWAY_INTERFACE}
    </pre>
    <hr>
  </body>
</html>";

print $body;
=cut

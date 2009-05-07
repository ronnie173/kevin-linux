#! /usr/bin/perl -wT
use strict;
use CGI;

my $q = new CGI;

my $username = $q->param("username");
my $password = $q->param("password");
my $address = $q->param("address");
my $city = $q->param("city");
my $state = $q->param("state");
my $zipcode = $q->param("zipcode");
my $email = $q->param("email");
my $phone = $q->param("phone");

print $q->header,
      $q->start_html("hello world"),
      $q->p("username len is " . length($username)),
      $q->p("password len is " . length($password)),
      $q->p("address len is " . length($address)),
      $q->p("city len is " . length($city)),
      $q->p("state len is " . length($state)),
      $q->p("zipcode len is " . length($zipcode)),
      $q->p("email len is " . length($email)),
      $q->p("phone len is " . length($phone));

print "<a href=\"/test_form.html\">Back</a>";

print $q->end_html;


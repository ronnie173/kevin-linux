#!/usr/bin/python
import cgi

def main():
    print "Content-type: text/html\n"
    form = cgi.FieldStorage()
    print "<p>username len is ", form["username"].value, "</p>"
    print "<p>password len is ", form["password"].value, "</p>"
    print "<p>address len is ", form["address"].value, "</p>"
    print "<p>city len is ", form["city"].value, "</p>"
    print "<p>state len is ", form["state"].value, "</p>"
    print "<p>zipcode len is ", form["zipcode"].value, "</p>"
    print "<p>email len is ", form["email"].value, "</p>"
    print "<p>phone len is ", form["phone"].value, "</p>"
main()

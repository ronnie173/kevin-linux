#!/usr/bin/python
import cgi

def main():
    print "Content-type: text/html\n"
    
    strlen = 0;
    form = cgi.FieldStorage()
    
    if form.has_key("username") :
        strlen = len(form["username"].value)
    else :
        strlen = 0;
        
    print "<p>username len is ", strlen, "</p>"
        
    if form.has_key("password") :
        strlen = len(form["password"].value)
    else :
        strlen = 0;
    print "<p>password len is ", strlen, "</p>"
    
    if form.has_key("address") :
        strlen = len(form["address"].value)
    else :
        strlen = 0;

    print "<p>address len is ", strlen, "</p>"

    if form.has_key("city") :
        strlen = len(form["city"].value)
    else :
        strlen = 0;
    print "<p>city len is ", strlen, "</p>"
    
    if form.has_key("state") :
        strlen = len(form["state"].value)
    else :
        strlen = 0;
    print "<p>state len is ", strlen, "</p>"

    if form.has_key("zipcode") :
        strlen = len(form["zipcode"].value)
    else :
        strlen = 0;
    print "<p>zipcode len is ", strlen, "</p>"
    
    if form.has_key("email") :
        strlen = len(form["email"].value)
    else :
        strlen = 0;
    print "<p>email len is ", strlen, "</p>"
    
    if form.has_key("phone") :
        strlen = len(form["phone"].value)
    else :
        strlen = 0;
    print "<p>phone len is ", strlen, "</p>"
main()

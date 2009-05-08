#!/usr/bin/python
def bar(x):
    try:
        return 1.0 / x
    except ZeroDivisionError, message:
        print "Can't divide by zero:", message
        
def test(a, b):
    "this is a test function"
    while a != 0:
        a, b = b % a, a
    return b

def main():
    if 0 :
        print "in if statement\n"
        print "out if statement\n"

    b = test(12, 20)
    print "b is ", b, "\n"
    
    print bar(3), "\n"
    print bar(0), "\n"
    
    print "end of program\n"

main()

import hello
planet = hello.World()
planet.set('foo')

def multiply(a,b):
    print planet.greet()
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    return c
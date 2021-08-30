# Credits: https://andrea.corbellini.name/2015/05/17/elliptic-curve-cryptography-a-gentle-introduction/

import random
from collections import namedtuple

def getPrimes(start, stop):
    """ Returns a list of prime number in range start->stop."""
    if start >= stop:
        return []

    primes = [2]

    for n in range(3, stop+1, 2):
        for p in primes:
            if n % p == 0:
                break
            else:
                primes.append(n)

    while primes and primes[0] < start:
        del primes[0]

    return primes

def areRelativelyPrime(a, b):
    """ When two numbers have no common factors other than 1."""
    for n in range(2, min(a,b) + 1):
        if (a % n) == (b % n) == 0:
            return False
    return True

def makeKeyPair(length):
    """ Create a public-private key pair.
    The key pair is generated from two random prime numbers.
    Length specifies the bit length of the number 'n' shared between the two keys.
    The higher n is, the better.
    """

    if length < 4:
        raise ValueError('Can not generate a key of length less than 4, got {length}')
    
    # First, we need to find a number 'n' that is the product of two prime numbers, p & q.
    # 'n' must have the number of bits specified by 'length', so it must be in the
    # range(nMin, nMax+1).
    nMin = 1 << (length - 1)
    nMax = (1 << length) - 1
    

    # The key is stronger if primes p and q have similar bit length.
    # Choose two prime number in range(start, stop) s.t. the difference of
    # bit lengths is at most 2.
    start = 1 << (length // 2 - 1)
    stop = 1 << (length // 2 + 1)
    primes = getPrimes(start, stop)

    # Select two primes s.t. their product is in range(nMin, nMax+1)
    while primes:
        p = random.choice(primes)
        primes.remove(p)
        qCandidates = [q for q in primes if nMin <= p * q <= nMax]

        if qCandidates:
            q = random.choice(qCandidates)
            break
    else:
        raise AssertionError('Can not find p and q  for a key of length {length}')

    # Secondly, we choose a number, 'e', s.t. it is lower than (p-1) * (q-1)
    # and s.t. it shares no factors with (p-1) * (q-1)
    stop = (p - 1) * (q - 1)
    for e in range(3, stop, 2):
        if areRelativelyPrime(e, stop):
            break
    else:
        raise AssertionError('Can not find e with p = {p} and q = {q}')
    
    # Thridly, find d such that (d*e-1) is divisable by (p-1) * (q-1)
    for d in range(3, stop, 2):
        if d * e % stop == 1:
            break
    else:
        raise AssertionError('Can not find d with p = {p}, q = {q}, and e = {e}')

    return PublicKey(p*q, e), PrivateKey(p*q, d)

class PublicKey(namedtuple('PublicKey', 'n e')):
    """ Used to encrypt data."""
    __slots__ = ()

    def encrypt(self, x):
        """ Encrypts the number x."""
        return pow(x, self.e, self.n)

class PrivateKey(namedtuple('PrivateKey', 'n, d')):
    """ Used to decrypt the data."""
    __slots__ = ()

    def decrypt(self, x):
        """ Decrypts the number x."""
        return pow(x, self.d, self.n)


if __name__ == '__main__':
    public = PublicKey(n=2534665157, e=7)
    private = PrivateKey(n=2534665157, d=1810402843)

    assert public.encrypt(123) == 2463995467
    assert public.encrypt(456) == 2022084991
    assert public.encrypt(123456) == 1299565302

    assert private.decrypt(2463995467) == 123
    assert private.decrypt(2022084991) == 456
    assert private.decrypt(1299565302) == 123456

    for length in range(4, 17):
        public, private = makeKeyPair(length)

        assert public.n == private.n
        assert len(bin(public.n)) - 2 == length

        x = random.randrange(public.n - 2)
        y = public.encrypt(x)
        assert private.decrypt(y) == x

        assert public.encrypt(public.n - 1) == public.n - 1
        assert public.encrypt(public.n) == 0

        assert private.decrypt(public.n - 1) == public.n - 1
        assert private.decrypt(public.n) == 0

    import doctest
    doctest.testfile(__file__, globs=globals())

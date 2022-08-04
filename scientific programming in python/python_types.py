import numbers
def f(): pass

def f1(): yield

class Cl:
    c = 111
    def func(self):
        pass

def get_objects():
    tipe  = f.__class__
    liss = [AttributeError(), EOFError(), Exception(), None, NotImplemented, TypeError(), ValueError(), bool(), len, bytearray(), bytes(), 
    complex(), dict(), iter(dict()), enumerate([]), float(), frozenset(), f, f1(), int(), list(), 
    iter(list()), memoryview(b'ab'),  Cl().func, Cl.__call__, Cl.__format__, numbers, object(), range(1), set(), 
    slice([]),  staticmethod(f1), str(), tuple(), tipe, Cl.__init__]
    return liss
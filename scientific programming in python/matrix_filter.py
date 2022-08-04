import numpy as np
class score_filter():
    
    def __init__(self, min_score, max_score):
        self.res = list()
        self.min_score_ = min_score
        self.max_score_ = max_score

    def __len__(self):
        return len(self.result)

    def __iter__(self):
        return self
    
    def fit(self, x, y, iterable):
        if (iterable == 0):
            raise RuntimeError('Iterable is empty')
        np.x = np.cumsum(x, dtype=np.float32)
        if (x == 0):
            raise RuntimeError('x is empty')
        np.y = np.cumsum(y, dtype=np.float32)
        if (y == 0):
            raise RuntimeError('y is empty')
        divid = np.multiply(np.x[-1], np.y[-1])
        
        for i, j in iterable:
            condition = np.divide(np.multiply(np.x[i], np.y[j]), divid)
            if self.min_score_ < condition and condition < self.max_score_:
                self.res.append((i, j))
        self.current = 0

    def __next__(self):
        if self.current + 1 <= len(self.res):
            current = self.current
            self.current += 1
            return self.res[current] 
        else:
            raise StopIteration
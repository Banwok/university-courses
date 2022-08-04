class list_divider(list):
    def __truediv__(self, n):
        if type(n) is not int:
            raise TypeError()
        if n<=0:
            raise ValueError()
        tup = divmod(len(self), n)
        k = tup[0]
        m = tup[1]
        return [self[i * k + min(i, m):(i + 1) * k + min(i + 1, m)] for i in range(n)]
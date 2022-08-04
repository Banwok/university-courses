class graph_partition:
    def __init__(self, process_count, process_number):
        self.__process_count = process_count
        self.__process_number = process_number
        self.leng = 0
        self.curr=0
        self.i = 0
        self.j = 0
    
    def __len__(self): 
        return self.leng 
        
    def fit(self, num):
        self.num = num
        if num <= 0:
            return
        pairs_number = int(num * (num - 1) / 2)
        mod = pairs_number % self.__process_number
        ind = self.__process_count * (pairs_number // self.__process_number)
        
        self.leng = pairs_number // self.__process_number
        if self.__process_count >= mod:
            ind += mod
        else:
            self.leng += 1
            ind += self.__process_count

        for i in range(num):
            if ind >= 0:
                ind -= num - i - 1
            elif ind < 0:
                break
            else:
                break
        self.i = i - 1
        self.j = ind + num
    
    def __iter__(self):
        return self

    def __next__(self):
        if self.curr >= self.leng:
            raise StopIteration
        self.curr += 1
        if self.j >= self.num:
            self.i += 1
            self.j = self.i + 2
            return(self.i, self.j - 1)
        else:
            self.j += 1
            return(self.i, self.j - 1)
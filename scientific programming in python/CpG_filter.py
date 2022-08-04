import numpy as np
class cpgs_annotation():
    def __init__(self, table, names):
        self.__names = names
        self.__defines = {'cpgs': 'ID_REF', 'chr': 'CHR', 'gene': 'UCSC_REFGENE_NAME', 'geotype': 'RELATION_TO_UCSC_CPG_ISLAND', 'crossr': 'CROSS_R', 'class': 'Class', 'genepart': 'UCSC_REFGENE_GROUP'}
        self.__table = table

    def __coincidence(self, cell, value):
        if type(value)==float: 
            return -1
        else:
            if type(cell)==str: 
                cell = cell.split(';')
            else:
                cell = [cell] 

                
            if type(value)==list:
                for v in value:
                    if v in cell:
                        return 1
            else:
                return int(value in cell)
        return  0

    def get_cpgs(self, filters):
        filter_in = dict()
        filter_out = dict()
        for name, value in filters.items():
            np.filter_name = np.asarray(name.split('_'))
            if len(np.filter_name)!=2:
                continue
            if np.filter_name[1] == 'in': 
                filter_in[np.filter_name[0]] = value
            elif np.filter_name[1] == 'out': 
                filter_out[np.filter_name[0]] = value

        np.output_cpgs = list()
        np.output_index = list()
        for i, row in enumerate(self.__table): 
            fl = True 
            for name, value in filter_out.items(): 
                if self.__coincidence(row[self.__names.index(self.__defines.get(name))], value)==1:
                    fl = False
                    break
            for name, value in filter_in.items(): 
                if self.__coincidence(row[self.__names.index(self.__defines.get(name))], value)==0:
                    fl = False
                    break
            if fl: 
                np.output_cpgs.append(row[self.__names.index(self.__defines.get('cpgs'))]) 
                np.output_index.append(i) 

        return np.output_cpgs, np.output_index 
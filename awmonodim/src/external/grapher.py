

from music21 import *



def grapher(fp='_awData.txt'):
    # load data from a file passed as a an arg, 
    raw = []
    f = open(fp)
    for l in f: # assume each line is a data point
        if l.strip() == '': continue
        raw.append(float(l))

    data = []
    for x, y in enumerate(raw):
        data.append((x, y))
    g = graph.GraphScatter(figureSize=(10,5), title="aw::", alpha=1)
    g.setData(data)
    g.process() # will show




if __name__ == '__main__':
    grapher()
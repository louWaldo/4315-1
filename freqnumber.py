from curses.ascii import isdigit
from fileinput import close
from typing import List
from functools import reduce
import re
from posixpath import split
import sys

def cleanlist(seq):
    #base case
    clnseq = []
    if seq[0] in clnseq:
        cseq=seq[1:].copy()
        print(cseq)
        cleanlist(seq[1:])
    else:
        clnseq.append(seq[0])
      
    return clnseq

def freqNum(k, inFile, outfile):
        
    integerlst: List[int] # integers only
    tempintLst: List[int]
    fltlst: List[float] # all real numbers
    tempfltLst: List[float]

    with open(inFile) as f:
        bloated = [s.strip().split(" ") for s in f.readlines()]
        flatList = [element for innerList in bloated for element in innerList]
        copyflt = flatList.copy()
    f.close()

    floatr = list(map(lambda i: re.findall('-?\d+\.{1}\d+',i), copyflt))
    #print(floatr, sep=',')
    cleanlst = flatList.copy()
    inti = list(map(lambda i: re.findall('-?\d*',i), flatList))
    #print(inti, sep=',')
    
    flatten=lambda l: sum(map(flatten,l),[]) if isinstance(l,list) else [l]
    F=flatten(floatr)
    print(inti)
    print("Fu me")
    print(F)
    print("")
    print("")
    print(cleanlist(inti))


    #This is for the end

    with open(outfile, "a") as f:
        for i in range(k):
            f.write(flatList[i])
        # for i in range(k):
        #     f.write(flatList[i])
    f.close()
    return 0


if __name__ == '__main__':
    argl = sys.argv[1].split(";")
    i = 0
    for pattrn in argl:
        if i == 0:
            k = "".join([val for val in pattrn[(pattrn.find("=")+1):]])
        elif i == 1:
            infile = ("".join([val for val in pattrn[(pattrn.find("=")+1):]]))
        else:
            out = "".join([val for val in pattrn[(pattrn.find("=")+1):]])
        i+=1
    freqNum(int(k),infile,out)

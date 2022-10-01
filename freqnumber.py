from curses.ascii import isdigit
from itertools import groupby
import re
import itertools
import sys

#Developed by Kevin Flores and Luis Waldo group #13, The closest thing we had to mutation was when it came to converting the 
#tuple inside the list into a string for writing to the outputfile, and maybe the sorting algorithm. 


#This function takes a tuple object and converts it to a sting, and it also formats
#the string so it fits the requirements
def convTup(tup):
    st = ''.join(map(str,tup))
    lastchr = len(st)-2
    ot = st[1:lastchr]
    fot = ot.replace(',','')
    return (fot + '\n')

#This function takes a tuple object and the outputfile, passed the tuple object to convTup, which returns a string
#that string is then written/appended to the outputfile
def writeOut(strO,out):
    with open(out,"a") as f:
        f.write(convTup((str(strO) + '\n' )))    
        f.close()
    return 0

#this function writes out the titles of what is being printed. integers or reals.
# titles are passed in as strT, and so is the outputfile
def writeTout(strT,out):
    with open(out,"a") as f:
        f.write(strT)
    f.close()
    return 0

#Main driver for the code, it gets the Input from infile, sets k , and passes in the output file name to functions within.
#First we clean the input using a map lambda function and regex to get integers and reals as their own lists
#Then we create a new list from copies of the dirty first list, and flatten that list
#Then we convert from string to actual type: i.e int or float
#we make a list of all the unique numbers, and then we get the frequency of those numbers into their own list
# *note these two lists are in order, and this their indexes are 1:1
#We then zip both lists together using out custom function ziptie
#Then we sort the list by their frequency, such that the highest frequencys are at the begining
#Finally we pass these sorted zipped lists into rtraverse, which recursively goes through the list k times and writes to output file
# *note to handle ties, we had to access the kth tuples frequency value, and make a copy of the subsequence from originalseq[k:]
# *this way we look at the remaining subsequence looking for the other tuples with the same frequency we found at the kth item
def freqNum(k,infile,out):
    with open(infile) as file:
         lines = file.readlines()

    orgFloat = list(map(lambda i: re.findall('-?\d+\.{1}\d+', i), lines))
    orgInt = list(map(lambda i: re.findall(r'-?\b(?<!\.)\d+(?!\.)\b', i), lines))
    flatten=lambda i: sum(map(flatten,i),[]) if isinstance(i,list) else [i]
    flatten=lambda i: sum(map(flatten,i),[]) if isinstance(i,list) else [i]
    strToInt = lambda i: list(map(int, i))
    strToFloat = lambda i: list(map(float, i))
    unique = lambda i: bag_to_set(i)
    freq = lambda i: frequencies(i)
    ziptie = lambda i, j: zip(i, j)
    sortByFreq = lambda i: sorted(i, key = lambda j: j[1], reverse = True)

    intFlat = flatten(orgInt)
    floatFlat = flatten(orgFloat)

    intNums = strToInt(intFlat)
    floatNums = strToFloat(floatFlat)

    floatSorted = sorted(floatNums)
    intSorted = sorted(intNums)

    floatUnique = unique(floatSorted)
    intUnique = unique(intSorted)

    intFreq = freq(intSorted)
    fpFreq = freq(floatSorted)

    intZip = ziptie(intUnique, intFreq)
    fpZip = ziptie(floatUnique, fpFreq)
    
    fpList = list(fpZip)
    intList = list(intZip)

    intLast = sortByFreq(intList)
    fpLast = sortByFreq(fpList)

    file.close()
    i = 0
    rtraverse(intLast, k, out, i)
    rtraverse(fpLast, k, out, i)

#this function is the function that recursively goes through the subsequence and finds the target frequency of the kth tuple
def rtraverse2(seq, j, out, t):
    if j < len(seq):
        if seq[j][1] == t:
            writeOut(seq[j],out)
        rtraverse2(seq, j+1, out, t)

#This function recursively iterates through the list and writes the kth elements in the sequence
#If the first elements is an integer, then write integer to outfile, same with floats with writeTout()
#otherwise writeout the i'th element in the sequence, which is sent to writeOut()
#however if we get to the kth element, check the remaining subsequence for any tuples with the frequency of the kth element in the seq
def rtraverse(seq, k, out, i):
    if len(seq) > k:
        if i < k:
            print(i)
            if type(seq[i][0]) == int and i == 0:
                writeTout("integer:"+'\n',out)
            elif type(seq[i][0]) == float and i == 0:
                writeTout("real:"+'\n',out)
            writeOut(seq[i],out)
            rtraverse(seq, k, out, i+1)
        elif i == k:
            j = 0
            temp = seq[k:]
            t = seq[i][1]
            rtraverse2(temp, j, out, t)
    elif len(seq) <= k:
        diff = len(seq)
        if i < diff:
            if type(seq[i][0]) == int and i == 0:
                writeTout("integer:"+'\n',out)
            elif type(seq[i][0]) == float and i == 0:
                writeTout("real:"+'\n',out)
            writeOut(seq[i],out)
            rtraverse(seq, k, out, i+1)

#creates the sets of the numbers/reals
def bag_to_set(old_list):
    new_list = []
    if old_list == []:
        new_list = []
    else:
        if (old_list[0] not in old_list[1:]):
            new_list = [old_list[0]] + bag_to_set(old_list[1:])
        else:
            new_list = bag_to_set(old_list[1:])
    return new_list 

#calculates the frequency of an item
def frequencies(lst):
    if len(lst) == 0:
        return []
    else:
        return [lst.count(lst[0])] + frequencies(lst[lst.count(lst[0]):])

#main function that drives the program, this reads the cli arguments and starts the function that does all the work
if __name__ == "__main__":
    argl = sys.argv[1].split(';') 
    i = 0

    for pattern in argl:
        if i == 0:
            k = "".join([val for val in pattern[(pattern.find("=")+1):]])
        elif i == 1:
            infile = "".join([val for val in pattern[(pattern.find("=")+1):]])
        else:
            out = "".join([val for val in pattern[(pattern.find("=")+1):]])
        i+=1
    m  = int(k)-1
    freqNum(m,infile,out)

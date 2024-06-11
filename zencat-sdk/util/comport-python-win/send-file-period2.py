import re

def find_all_occurrences(A, B):
    start = 0
    indexes = []
    while True:
        index = A.find(B, start)
        if index == -1:
            break
        indexes.append(index)
        start = index + 1
    return indexes

A = b'This is a test. This test is a test.'
B = b'test'

indexes = find_all_occurrences(A, B)
print("Indexes:", indexes)

tok_1 = b'+HTTPCLIENT:'
vtmp = 0
blen = 0
dlen = 0

with open('mock/teraterm-1.log', 'rb') as f:
    data = f.read()

print(type(data), len(data))
# print(data, '\n')

indexes = find_all_occurrences(data, tok_1)
print("Indexes:", indexes, "Len:", len(indexes))

for i in range(len(indexes) - 1):
    pattern = re.compile(r'\d+')
    cn = indexes[i]
    nn = indexes[i + 1]
    tktmp = ''.join(data[cn:nn].decode('utf-8', errors='ignore'))
    match1 = pattern.search(tktmp)
    if match1:
        vtmp = match1.group()
    if i < (len(indexes) - 1):
        vlen = len(vtmp)
        blen = (indexes[i + 1] - indexes[i]) - (len(tok_1) + vlen + 1) - 2
        dlen += blen
    if i == (len(indexes) - 2):
        tktmp = ''.join(data[nn:nn + 16].decode('utf-8', errors='ignore'))
        match1 = pattern.search(tktmp)
        if match1:
            vtmp = match1.group()
        dlen += int(vtmp)
    bk = 0
print("dlen:", dlen) # 386125 - 359715 = 26410

# for i in range(len(data[392:372797])):
#     print(i, hex(data[i + 392])) # i begin from 0
import serial

ser = serial.Serial('COM1', 115200, 8, 'N', 1)
flag = ser.is_open

if flag:
    print('open success\n')
else:
    print('open error\n')

with open('mock/teraterm-1.log', 'rb') as f:
    data = f.read()

print(type(data), len(data))
print(data, '\n')

# mylist = [0x2b, 0x3a]
# ser.write("From python\n".encode("utf-8"))
# ser.write(mylist)
# ser.write(data[372800:372803])

tok = []
tok_idx = 0
tok_1 = '+HTTPCLIENT:'
tok_1e = ','
sta = 0

# png data len of payload with header: 372405
for i in range(len(data[392:372797])):
    print(i, hex(data[i + 392])) # i begin from 0
    # ser.write(data[i + 392])
    # if sta == 0:
    #     tok.insert(tok_idx, data[i + 392])
    #     tok.insert(tok_idx + 1, '\0')
    #     tok_idx = tok_idx + 1



ser.close()
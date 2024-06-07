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
tok_value = []
tok_value_idx = 0
tok_1 = '+HTTPCLIENT:'
tok_1e = ','
tok_2 = '\r\n'
tok_3 = '\r\nbusy p...'
record = []
record_idx = 0
record_len = 0
record_end_idx = 0
total_record_len = 0
sta = 0 # 0: scan meta, 1: scan meta value, 2: scan data, 3: end of scan data

# png data len of payload with header: 372405
for i in range(len(data[392:372797])):
    # print(i, hex(data[i + 392])) # i begin from 0
    # ser.write(data[i + 392])
    if sta == 0:
        tok.insert(tok_idx, chr(data[i + 392]))
        tok.insert(tok_idx + 1, '\0')
        tok_idx = tok_idx + 1

        # skip 'busy p...'
        if tok_idx >= 1:
            if ''.join(tok).rstrip('\0').startswith(tok_2):
                if ''.join(tok).rstrip('\0') == tok_3:
                    tok = []
                    tok_idx = 0
                    sta = 3
                    continue
                continue

        if tok_idx >= len(tok_1):
            if ''.join(tok).rstrip('\0') == tok_1:               
                sta = 1
                continue
            else:
                # teraterm-1
                # Cursor Offset: 92880 RLen:512 TLen: 505
                print("Token Error")

    if sta == 1:
        tok_value.insert(tok_value_idx, chr(data[i + 392]))
        tok_value.insert(tok_value_idx + 1, '\0')
        tok_value_idx = tok_value_idx + 1

        if chr(data[i + 392]) == tok_1e and \
        1 <= tok_value_idx and tok_value_idx <= 4:
            tok_value[tok_value_idx - 1] = '\0'
            record_len = int(''.join(tok_value).rstrip('\0'))
            sta = 2
            continue

    if sta == 2:
        record.insert(record_idx, data[i + 392])        
        record_idx = record_idx + 1
        record_len = record_len - 1
        if(record_len == 0):
            hex_chars = [hex(num) for num in record]
            print("len =", len(record), hex_chars)
            total_record_len += len(record)
            print("total_record_len =", total_record_len)
            tok = []
            tok_idx = 0
            tok_value = []
            tok_value_idx = 0
            sta = 3
            continue

    if sta == 3:
        record_end_idx += 1
        if(record_end_idx == 2):
            record = []
            record_idx = 0
            record_len = 0
            record_end_idx = 0
            sta = 0



ser.close()
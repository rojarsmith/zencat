import serial

ser = serial.Serial('COM1', 115200, 8, 'N', 1)
flag = ser.is_open

if flag:
    print('open success\n')
else:
    print('open error\n')

mylist = [0x2b, 0x3a]
ser.write("From python\n".encode("utf-8"))
ser.write(mylist)
ser.close()
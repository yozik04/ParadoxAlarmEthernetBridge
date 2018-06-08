#!/usr/bin/env python3
import socket
import binascii
import struct

progname = "paradox"

UDP_IP = "192.168.1.2"
UDP_PORT = 5000

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

def verify_data(data):
  if len(data) != 37:
    print('Wrong packet size')
    return False
  if sum(data[0:36]) % 256 != data[36]:
    print('Packet checksum check failed')
    return False
  return True

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    data = bytearray(data)
    data_len = len(data)

    if not verify_data(data):
      print('Skipping packet...')
      continue

    armstatus, requested_event_nr, event_nr, century, year, month, day, hour, minute, event_group, partition, event_1, event_2, serial_nr, label_type, label, b_35, checksum = struct.unpack('BBHBBBBBBBBBB4sB16sBB', data)

    print("armstatus: %d, event_group: %d, event_1: %d, partition: %d, label_type: %d, label: %s" % (armstatus, event_group, event_1, partition, label_type, label.decode('utf-8').strip()))
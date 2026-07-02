# -*- coding: utf-8 -*-
"""
       tcpserver.py

       TCP server side code to receive detected objects data

   NOTE:
       (C) Copyright 2021 Texas Instruments, Inc.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

     Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

     Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the
     distribution.

     Neither the name of Texas Instruments Incorporated nor the names of
     its contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""

#!/usr/bin/env python3

import socket
import struct
from ctypes import *

## USER SHOULD CHANGE THIS ACCORDINGLY
HOST = '192.168.1.200'  # Local IP Address
##
PORT = 7        # Port to listen on

def hex2(x):
    return ('0' * (len(x) % 2)) + x

def convert(s):
    i = int(s, 16) # convert from hex to a Python int
    cp = pointer(c_int(i)) # make this into a c integer
    fp = cast(cp, POINTER(c_float)) # cast the int pointer to a float pointer
    return fp.contents.value # dereference the pointer, get the float

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        while True:
            numObj = conn.recv(4) # Receive number of objects
# exit tcpserver.py after sensorStop

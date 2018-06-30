# -*- coding: utf-8 -*-
###########################################################################
## Python code generated with ScadaPy Creator v.3.4.3  29.06.2018 
## Modbus Client by Jack Mas
## http://scadapy.ln-group.ru/
##
## PLEASE DO ALL YOU NEED THIS FILE!
########################################################################### 
import time
import modbus_tk
import modbus_tk.defines as cst
import modbus_tk.modbus_tcp as modbus_tcp
from modbus_tk import modbus_rtu
import serial
import gc
import socket
import threading
import json
def Proc_4(f=1):
     startAdr=[]
     rangeAdr=[]
     rtuAddress=[]
     varname=[]
     timeOut=[]
     reg=[]
     unitCount=7
     for i in range(0,unitCount+1):
         rtuAddress.append(i)
         reg.append(i)
         startAdr.append(i)
         rangeAdr.append(i)
         varname.append(i)
         timeOut.append(i)
     try:
         master = modbus_tcp.TcpMaster(host='127.0.0.1', port=int('502'))
         master.set_timeout(2)
     except Exception as e:
          pass
     x=0; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Discret_1','1','READ_DISCRETE_INPUTS','0','5','0.5')
     x=1; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Analog_1','1','READ_INPUT_REGISTERS','0','5','0.5')
     x=2; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Discret_2','1','READ_DISCRETE_INPUTS','10','5','0.5')
     x=3; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Discret_3','1','READ_DISCRETE_INPUTS','20','5','0.5')
     x=4; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Analog_2','1','READ_INPUT_REGISTERS','10','5','0.5')
     x=5; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Analog_3','1','READ_INPUT_REGISTERS','20','1','1')
     x=6; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Analog_4','1','READ_INPUT_REGISTERS','21','1','1')
     while True:
         for i in range(0,unitCount):
             try:
                 if(reg[i]=='READ_INPUT_REGISTERS'):   c=cst.READ_INPUT_REGISTERS
                 if(reg[i]=='READ_DISCRETE_INPUTS'):   c=cst.READ_DISCRETE_INPUTS
                 if(reg[i]=='READ_COILS'):             c=cst.READ_COILS
                 if(reg[i]=='READ_HOLDING_REGISTERS'): c=cst.READ_HOLDING_REGISTERS
                 varNameData= master.execute(int(rtuAddress[i]), c, int(startAdr[i]), int(rangeAdr[i]) )
                 sock_udp.sendto(  json.dumps( {'name':varname[i],'data':varNameData} ).encode('utf-8'), server_address_udp)
                 sock_udp_arch.sendto(  json.dumps( {'name':varname[i],'data':varNameData} ).encode('utf-8'), server_address_udp_arch)
                 sock_udp_vk.sendto(  json.dumps( {'name':varname[i],'data':varNameData} ).encode('utf-8'), server_address_udp_vk)
             except Exception as e:
                 varNameData=None
                 sock_udp.sendto(  json.dumps( {'name':varname[i],'data':'Error'} ).encode('utf-8'), server_address_udp)
                 sock_udp_arch.sendto(  json.dumps( {'name':varname[i],'data':'Error'} ).encode('utf-8'), server_address_udp_arch)
                 sock_udp_vk.sendto(  json.dumps( {'name':varname[i],'data':'Error'} ).encode('utf-8'), server_address_udp_vk)
                 master = modbus_tcp.TcpMaster(host='127.0.0.1', port=int('502'))
             time.sleep(float(timeOut[i]))
             gc.collect()
def Proc_5(f=1):
     startAdr=[]
     rangeAdr=[]
     rtuAddress=[]
     varname=[]
     timeOut=[]
     reg=[]
     unitCount=3
     for i in range(0,unitCount+1):
         rtuAddress.append(i)
         reg.append(i)
         startAdr.append(i)
         rangeAdr.append(i)
         varname.append(i)
         timeOut.append(i)
     try:
         master = modbus_tcp.TcpMaster(host='127.0.0.1', port=int('502'))
         master.set_timeout(2)
     except Exception as e:
          pass
     x=0; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Unit_2_Discret','1','READ_DISCRETE_INPUTS','30','5','0.5')
     x=1; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Analog_22','1','READ_INPUT_REGISTERS','20','5','0.5')
     x=2; varname[x],rtuAddress[x],reg[x],startAdr[x],rangeAdr[x],timeOut[x]=('Analog_23','1','READ_INPUT_REGISTERS','40','5','0.5')
     while True:
         for i in range(0,unitCount):
             try:
                 if(reg[i]=='READ_INPUT_REGISTERS'):   c=cst.READ_INPUT_REGISTERS
                 if(reg[i]=='READ_DISCRETE_INPUTS'):   c=cst.READ_DISCRETE_INPUTS
                 if(reg[i]=='READ_COILS'):             c=cst.READ_COILS
                 if(reg[i]=='READ_HOLDING_REGISTERS'): c=cst.READ_HOLDING_REGISTERS
                 varNameData= master.execute(int(rtuAddress[i]), c, int(startAdr[i]), int(rangeAdr[i]) )
                 sock_udp.sendto(  json.dumps( {'name':varname[i],'data':varNameData} ).encode('utf-8'), server_address_udp)
                 sock_udp_arch.sendto(  json.dumps( {'name':varname[i],'data':varNameData} ).encode('utf-8'), server_address_udp_arch)
                 sock_udp_vk.sendto(  json.dumps( {'name':varname[i],'data':varNameData} ).encode('utf-8'), server_address_udp_vk)
             except Exception as e:
                 varNameData=None
                 sock_udp.sendto(  json.dumps( {'name':varname[i],'data':'Error'} ).encode('utf-8'), server_address_udp)
                 sock_udp_arch.sendto(  json.dumps( {'name':varname[i],'data':'Error'} ).encode('utf-8'), server_address_udp_arch)
                 sock_udp_vk.sendto(  json.dumps( {'name':varname[i],'data':'Error'} ).encode('utf-8'), server_address_udp_vk)
                 master = modbus_tcp.TcpMaster(host='127.0.0.1', port=int('502'))
             time.sleep(float(timeOut[i]))
             gc.collect()
if __name__ == "__main__":
     try:
         try:
             print( 'UDP sender start')
             sock_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
             server_address_udp = ('localhost', 64000)
             sock_udp_arch = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
             server_address_udp_arch = ('localhost', 64001)
             sock_udp_vk = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
             server_address_udp_vk = ('localhost', 64002)
         except Exception as e:
             print('UDP fail ',e)
         print( 'Starting modbus client')
         time.sleep(1.0)
  ########################### treads block
         modb_4 = threading.Thread(target=Proc_4,args=(1,))
         modb_4.daemon = True
         modb_4.start()
         modb_5 = threading.Thread(target=Proc_5,args=(1,))
         modb_5.daemon = True
         modb_5.start()

         modb_4.join()
         modb_5.join()

     except Exception as e:
         print(e)

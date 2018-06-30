﻿# iec104server
iec104server for UDP JSON
```
Now version 2.8
```
iec104server
```
Use syntax for run: server104.exe file.json
```

Json file structure like this:
```
-[
-  { "Server":
-    {   "UdpPort"   :"64002",
-        "UdpIp"     :"0.0.0.0",
-        "Iec104Port":"2402",
-        "Iec104Ip"  :"127.0.0.1",
-        "Debug"     :"1"
-      }
-  },
-  {
-   "SinglePoint":
-     {
-       "VarType":"bool",
-      "VarName":"Discret",
-      "IecAddress":"101",
-      "OffSet":"0",
-      "ByteCount":"1",
-      "ByteSequence":"1" 
-     }
-  },
-  {
-   "MeasureValue":
-     { 
-      "VarType":"int32(float or int)",
-      "VarName":"Analog",
-      "IecAddress":"101",
-      "OffSet":"0",
-      "ByteCount":"2",
-      "ByteSequence":"12", 
-      "Koef":"0.1"
-     }
-  }
- ]

- UDP packet structure:
- { "name":"VariableName","data":[123,0,2,3.14] }

iec104client send UDP JSON
```
Now version 2.8
```
term104 iec104 client made on ncurses
```

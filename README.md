# creator
ScadaPy Creator for modbus client
```
В настоящий момент актуальная версия 3.4.2
```
Программа генерирует файлы:

- modbus.py -- драйвер опроса modbus устройств;
- mclient.py -- драйвер опроса счетчиков Меркурий-230
- mercury.py -- библиотека для драйвера счетчиков Меркурий-230
- jserver.py -- json http сервер, порт 8080 
- dbserver.py -- драйвер подсистемы архивиривания, СУБД Postgresql
- jclient.html -- web клиент scada
- image.svg -- svg файл визуального отображения структуры.
- vkclient.py -- драйвер сообщений ВКонтакте
- start.sh -- bash скрипт запуска в фоне (Linux)
- stop.sh -- bash скрипт остановка фоновых процессов (Linux)

iec104server
```
Use syntax for run: 
```
server104.exe file.json

Json file structure like this:
```
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


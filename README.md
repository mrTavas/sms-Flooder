
# SMS Flooder
![Иллюстрация к проекту](https://user-images.githubusercontent.com/42433097/72919852-3f345780-3d59-11ea-94bf-745236e72a6b.png)

## Installation

```
sudo apt-get install libcurl4-openssl-dev
```
--------
## Compilation for Linux
```
gcc sms-flooder.c -lcurl -o sms-flooder
```
## Compilation for Windows
For compilation you may use mingw32. 
- Dynamic link compilation:
```
i686-w64-mingw32-g++ -static-libgcc -static-libstdc++ sms-flooderWin.c -lcurl -o Flooder.exe
```
________
For static linking, 3 more libraries from that repository will be needed: libssh2 , nghttp2 , openssl. Also, for the assembly will need zlib.The collected libcurl and its dependencies can be found here in this repository: https://bintray.com/vszakats/generic
- Static link compilation example:
```
i686-w64-mingw32-windres my.rc -O coff -o my.res
```
```
i686-w64-mingw32-gcc sms-flooderWin.c -o Flooder.exe my.res -DCURL_STATICLIB -I /usr/i686-w64-mingw32/include/ -L /usr/i686-w64-mingw32/lib/ --static -static-libgcc -lcurl -lssl -lcrypto -lssh2 -lnghttp2 -lz -lws2_32 -lwinmm -lwldap32 -lcrypt32
```
-------
## Start
For Linux:
```
sudo ./sms-flooder
```
For Windows:
1. Download zip.
2. Use Windows XP Service Pack 3 (SP3) compatibility mode. 
3. Start Flooder.exe as administrator.
-------
## Disclaimer
This repository is for academic purposes, the use of this software is your responsibility.

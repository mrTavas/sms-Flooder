
## SMS Flooder

Sms flooder writed with "c" language and libcurl library.

### Installation

```
sudo apt-get install libcurl4-openssl-dev
```
### Compilation for Linux
```
gcc sms-flooder.c -lcurl -o sms-flooder
```
### Compilation for Windows
For compilation you may use mingw32. 
Dynamic link compilation:
```
i686-w64-mingw32-g++ -static-libgcc -static-libstdc++ sms-flooderWin.c -lcurl -o Flooder.exe
```
For static linking of this version of curl , 3 more libraries from that repository will be needed: libssh2 , nghttp2 , openssl. Also, for the assembly will need zlib.The collected libcurl and its dependencies can be found here in this repository: https://bintray.com/vszakats/generic
Static link compilation example:
```
i686-w64-mingw32-gcc sms-flooderWin.c -o Flooder.exe -DCURL_STATICLIB -I /usr/i686-w64-mingw32/include/ -L /usr/i686-w64-mingw32/lib/ --static -static-libgcc -lcurl -lssl -lcrypto -lssh2 -lnghttp2 -lz -lws2_32 -lwinmm -lwldap32 -lcrypt32
```
### Start
```
sudo ./sms-flooder
```
### Disclaimer
This repository is for academic purposes, the use of this software is your responsibility.

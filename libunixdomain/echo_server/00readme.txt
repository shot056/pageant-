
# build

## cygwin
- cygwin����
- mkdir build_cygwin
- cd build_cygwin
- cmake ..
- make

## visual studio
- windows�̊�(cmd,powershell)��
- mkdir build_vc
- cd build_vc
- cmake ..
- make

## msys2
- msys2����
- mkdir build_msys2
- cd build_msys2
- cmake ..
- make

## mingw
- mingw����
  - path C:\Qt\Tools\mingw530_32\bin
  - path C:\Program Files\CMake\bin;%PATH%
- mkdir build_mingw
- cd build_mingw
- cmake -G"MinGW Makefiles" ..
- mingw32-make

# test
- echo_server���N��
- netcat�Őڑ�
    - unix domain socket�̂Ƃ� `nc -U socket_path`�Őڑ�
    - tcp socket �̂Ƃ� `nc localhost [port]`�Őڑ�
- netcat�����瑗�M����

# hard test
- ls -R > /tmp/ls.txt
- ls -R | nc -U /tmp/test.sock > /tmp/nc.txt
- ls -R | nc -U /tmp/test.sock | nc -U /tmp/test.sock | nc -U /tmp/test.sock | nc -U /tmp/test.sock | nc -U /tmp/test.sock | nc -U /tmp/test.sock | nc -U /tmp/test.sock | nc -U /tmp/test.sock| nc -U /tmp/test.sock| nc -U /tmp/test.sock| nc -U /tmp/test.sock> /tmp/nc2.txt

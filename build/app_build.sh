#!/bin/sh

# dash不支持直接 a="ll b"
url_sh="$(which sh)"
echo $(ls -l ${url_sh})

cd ..

# dash 只支持=
if [ $# -gt 0 ] && [ $1 = "clean" ]; then
    rm -f *.o 
    rm -f moc_*
    rm -f ui_*.h
else
    if [ $# = 0 ] || [ $1 -ne "clean" ]; then 
        make
        ./qt5*
    fi
fi

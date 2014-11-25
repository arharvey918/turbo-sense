#!/bin/bash

mv $1 $1.old
indent $1.old -o $1 -nbad -bap -nbc -bbo -hnl -br -brf -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i2 -ip0 -l80 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts2 -il1 -nut

exit

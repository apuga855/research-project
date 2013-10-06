udo tcpdump -i lo -vvv -X -s0 -w tmpfile port 2000 '((ip[2:2] - ((ip[0:1] & 0x0f) << 2) - ((tcp[12:1] & 0xf0) >> 2)) + ip[12:20] + tcp[0:8] != 0)'

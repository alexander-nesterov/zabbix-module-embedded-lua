## Embedded Lua in loadable module of Zabbix

Note: Only *nix

Official Documentation: 

https://www.zabbix.com/documentation/3.4/manual/config/items/loadablemodules

to build this module you need:

1) Install Lua
```bash
sudo apt-get install lua5.3 liblua5.3-dev
```
```bash
alexander@dev:~$ which lua
/usr/bin/lua
alexander@dev:~$ lua -v
Lua 5.3.1  Copyright (C) 1994-2015 Lua.org, PUC-Rio
alexander@dev:~$ ls -la /usr/include/lua5.3
-rw-r--r--  1 root root  8433 Aug 14  2015 lauxlib.h
-rw-r--r--  1 root root 14825 Aug 14  2015 lua.h
-rw-r--r--  1 root root   191 Aug 14  2015 lua.hpp
-rw-r--r--  1 root root 21016 Apr 14  2016 luaconf.h
-rw-r--r--  1 root root  1173 Aug 14  2015 lualib.h
alexander@dev:~$ ls -la /usr/lib/x86_64-linux-gnu/liblua*
-rw-r--r-- 1 root root 450014 Apr 14  2016 /usr/lib/x86_64-linux-gnu/liblua5.3-c++.a
lrwxrwxrwx 1 root root     22 Apr 14  2016 /usr/lib/x86_64-linux-gnu/liblua5.3-c++.so -> liblua5.3-c++.so.0.0.0
lrwxrwxrwx 1 root root     22 Apr 14  2016 /usr/lib/x86_64-linux-gnu/liblua5.3-c++.so.0 -> liblua5.3-c++.so.0.0.0
-rw-r--r-- 1 root root 224208 Apr 14  2016 /usr/lib/x86_64-linux-gnu/liblua5.3-c++.so.0.0.0
-rw-r--r-- 1 root root 428960 Apr 14  2016 /usr/lib/x86_64-linux-gnu/liblua5.3.a
lrwxrwxrwx 1 root root     18 Apr 14  2016 /usr/lib/x86_64-linux-gnu/liblua5.3.so -> liblua5.3.so.0.0.0
lrwxrwxrwx 1 root root     18 Apr 14  2016 /usr/lib/x86_64-linux-gnu/liblua5.3.so.0 -> liblua5.3.so.0.0.0
-rw-r--r-- 1 root root 224104 Apr 14  2016 /usr/lib/x86_64-linux-gnu/liblua5.3.so.0.0.0
```

2) Download source code of Zabbix and then run the ```./configure``` command (without arguments) in the root of Zabbix source tree

3) Build this module
```bash
alexander@dev:~$ gcc -fPIC -shared -o zbx_lua.so zbx_lua.c -I../../../include -llua5.3 -lm
```

4) Check
```bash
alexander@dev:~$ grep zbx_lua.so /var/log/zabbix/zabbix_agentd.log
20174:20171001:210332.109 loaded modules: zbx_lua.so
```
```bash
alexander@dev:~$ /usr/sbin/zabbix_agentd --print | grep lua.version
lua.version                                   [s|Lua 5.3
```
and then I will check the following script
```lua
function sum (...)
	local s = 0
	for i, v in ipairs{...} do
		s = s + v
	end
	return s
end
function main ()
	local s = sum(10, 59, 69)
	print(string.format("%d", s))
end
main()
```



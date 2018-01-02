# mini_cache
this is a mini memory cache

## usage
```
make server
./server
```

### set 
```
telnet 127.0.0.1 12345
set a1 xxx
```
### get 
```
telnet 127.0.0.1 12345
get a1
```
### del
```
telnet 127.0.0.1 12345
del a1
```

## list 列表操作
### lpush
```
// 往列表里面添加数据
lpush a1 xxx
```
### llen
```
// 查看列表的长度
llen a1
```
### lrange 
```
// 在列表里面取个范围
lrange a1 0 10
```
### list
```
// 展示列表里面所有数据
list a1
```

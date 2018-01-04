## mini_cache
this is a high performance and stable memory cache, based on epoll and mutil thread

## author
- name:johnny
- email:leiyuqing_jing@163.com

## usage
```
make server
./server
```

## 客户端
```
telnet 127.0.0.1 12345
```

## 键值对
### set 
```
set a1 xxx
```
### get 
```
get a1
```
### del
```
// 可删除所有类型的数据,(键值对，列表，哈希表)
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
### ldel 
```
// 删除列表中指定的数据
ldel a1 1
```

## hashmap 哈希表
### hmset
```
hmset key1 child_key xxxx
```
### hmget
```
hmget key1 child_key
```
### hmdel
```
hmdel key1 child_key
```

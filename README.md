## mini_cache
这是一个高速缓存服务器, 基于 **linux**, **epoll**, **tcp**, **hashmap** and **mutil thread**

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

## 登录
```
// 首先必须要登录
// 其次这个账号和密码暂时是固定的，写在代码里面的
login admin admin@minicache@123
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

## 订阅模式
### 订阅
```
subsribe que1
```
### 发布
```
publish que1 xxxx
```

## next to do
- 权限控制
- log
- 实现php扩展客户端(php extension client)

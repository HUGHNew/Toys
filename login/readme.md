# Login C/S

## overview

Socket编程练习：用户注册与登录 
任务1. User Registration - 用户注册 

服务器返回注册相关信息

任务2. User Authentication - 用户登录认证

服务器返回注册相关信息



## struct

| 字段   |      |
| ------ | ---- |
| Header |      |
| Body   |      |

### Header

| name        | bytes | type   | desc |
| ----------- | ----- | ------ | ---- |
| totalLength | 4     | Uint32 |      |
| commandID   | 4     | Uint32 |      |

| commandID | desc                          |
| --------- | ----------------------------- |
| 1         | registration **request** msg  |
| 2         | registration **response** msg |
| 3         | login **request** msg         |
| 4         | login **response** msg        |

### Body

#### request

| name       | bytes | type         | desc |
| ---------- | ----- | ------------ | ---- |
| UserName   | 20    | Octet String |      |
| UserPassWd | 30    | Octet String |      |

#### response

| name        | bytes | type         | desc                                       |
| ----------- | ----- | ------------ | ------------------------------------------ |
| status      | 1     | Octet String | login status<br/>0 -- fall<br/>1-- succeed |
| description | 64    | Octet String | desc of the result                         |

## Impl

-   ResponseServer
-   RequestClient

主要的结果代码 在这里面 可以直接运行
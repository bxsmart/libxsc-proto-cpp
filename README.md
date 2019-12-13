# libxsc-proto-cpp

[XSC](http://www.dev5.cn/x_msg_im/start/xsc/)协议的c++实现. 另见[java实现](https://github.com/dev5cn/libxsc-proto-java).

`XSC`(X Server Communication)协议栈是一个高度可扩展, 向前向后兼容, 低冗余的电信级通信协议栈, 它分为三层: `传输层`, `事务控制层`, `应用层`.

`XSC`协议栈为[X-MSG-IM](https://github.com/dev5cn/x-msg-im)设计, 但是它同样`非常`适合用于其它高实时性通信场景, 如: 物联网设备控制, 网络游戏, 微服务间的rpc.

![img](http://www.dev5.cn/x_msg_im/start/xsc/img/xsc-protocol-stack.svg)

承载`XSC`协议栈的底层协议可能具有不同的`Qos`: `tcp`, `websocket`, `udp`, `rudp`, `sctp`, etc.

---

##### 传输层

![img](http://www.dev5.cn/x_msg_im/start/xsc/img/xsc-protocol-transmission.svg)

* `indicator`, 指示部分. 用于身后的字段标识. 

    * 最低位A位: 标识是否存在下面的header部分.

    * C, B位: 标识下面的长度部分所占的字节数, 可能的值有: 
        
        * `00`: length占用一个字节.

        * `01`: length占用两个字节.

        * `10`: length占用三个字节.

        * `11`: length占用四个字节.

    * D, E, F位: RESERVED.

    * 最高位H位: 心跳(PING/PONG)标识.

        * 为`1`时表示心跳, 此时整个`indicator`的全部有效位是`H G`, 整个XSC协议报文也只有这一个字节:
            
            * `H G`位可能的值有: `10`: `PING`, `11`: `PONG`. 
        
        * 为`0`时非心跳报文.

* `length`, XSC报文长度部分. 它可以是1 ~ 4个字节. 它表示的长度包含了整个XSC协议报文, 即`传输层` + `事务层` + `应用层`.

* `header`, 可选的头部. 它是一个`TLV`结构. 可能包含以下部分:

    * 报文`QoS`.

    * 域内域间消息路由.

    * 分布式信令跟踪`Distributed Signalling Tracing`支持.

    * 上层协议报文安全保障.

    * 流.
    
---

##### 事务控制层

* XSC-事务控制层的灵魂是`ITU-T Q.773`, 即众所周知的TCAP`Transaction Capabilities Application Part`协议.

* `TCAP`协议在GSM核心网中扮演着非常重要的角色, 它为移动终端的`附着`, `位置更新`, `呼叫`, `短信`等大量重要信令流程提供事务控制层面的支撑.

* `TCAP`协议通常出现在这样的协议栈`SIGTRAN`中:

![img](http://www.dev5.cn/x_msg_im/start/xsc/img/xsc-protocol-transaction.svg)

* XSC-事务控制层对`TCAP`协议作了一些改造和扩展. 去掉了一些冗余和不常用的设计, 引入了更强大的语义, 且更容易实现.

---

##### 应用层

* XSC-应用层默认的协议是`Google Protocol Buffers`, 这是一个强大的, 跨语言的数据交换协议, 谁用谁知道.

* 应用层在有净荷数据`payload`时, 包含两个`TLV`字段:

    * `message name`, 它是业务层消息的名称`或者是与名称的对应关系`.

    * `message body`, 它是业务层消息序列化后的二进制报文.

* 应用层在没有净荷数据`payload`时, 这通常在事务层的`END`, `CANCEL`, `ABORT`等结束性事务原语中出现, 此时包含另外两个`TLV`字段:

    * `return code`, 返回值, 或者叫错误码.

    * `description`, 对返回值更详细的描述.


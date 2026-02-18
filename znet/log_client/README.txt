-------------------------------------------------------------------------------------------------------------
日志客户端使用说明
-------------------------------------------------------------------------------------------------------------
	使用方法:
		1.在宿主代码conf配置文件中添加

[Log2UdpClient]
type                    =       udp
port                    =       23568
address                 =       172.31.2.39
so_sndbuf               =       16384
so_rcvbuf               =       16384
ibuffermax              =       16384
obuffermax              =       1048576
accumulate              =       1048576

[Log2TcpClient]
type                    =       tcp
port                    =       23568
address                 =       172.31.2.39
so_sndbuf               =       16777216
so_rcvbuf               =       16384
accumulate              =       16777216
ibuffermax              =       16384
obuffermax              =       1048576
accumulate              =       1048576


		修改与logserver匹配的端口和ip地址

		2.宿主代码include "logger.hpp"文件

		3.初始化与日志服务器的连接
			读入conf配置文件后初始化连接 
			LOG::Log_Init();

		4.发送日志接口函数
        		void LOG::Log(const GNET::Protocol *protocol, bool reliability = true);
		        void LOG::Log(const GNET::Protocol &protocol, bool reliability = true);
			reliability=true为通过tcp连接传输

		5.名字空间为LOG,库文件名为libLogClient.a

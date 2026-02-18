-------------------------------------------------------------------------------------------------------------
日志索引和查询开发接口说明
-------------------------------------------------------------------------------------------------------------
(1)索引建立
	1.继承超类Index,实现自己的索引规则(index.hpp).
	2.调用IndexMaker 方法 add(), void IndexMaker::add(Index * idx);
	3.调用IndexMaker 方法 make().
	3.编译程序
	4.命令行参数规则说明:
		demoindexmaker application.xml file_list mail|tail
		application.xml	应用xml配置文件(读默认目录)
		file_list	源日志文件列表(tail时只对应一个), file_list一律用""括起来,
				如果file_list为空,程序根据application.xml配置,根据添加进来index的type找到对应的logname文件名,
					根据这个文件名在转储目录下找所有匹配的日志文件.
		mail		建立完索引退出(源文件为转储目录下的日志文件),多个文件列表表示: "file1 file2 file3"
		tail		建立实时索引,循环等待(源文件为logserver正在记录的日志文件)
	
	5.索引文件名规则:
		A.执行转储后的日志文件建索引	serial_logname.startime.type.keyname.idx
			例如:日志文件00005_login.20070126160342.log
			建立索引:协议类型(type)为2004, 关键字名称(keyname)为logintime
			则相应的日志索引文件名:
				00005_login.20070126160342.2004.logintime.idx
		B.对logserver当前正在读写的文件建立索引(实时索引)	logname.starttime.type.keyname.idx
			例如:日志文件login.log
			建立索引:协议类型(type)为2004, 关键字名称(keyname)为logintime
			则相应的日志索引文件名:
				login.20070126160342.2004.logintime.idx
	6.实时索引处理:
		命令行参数为tail是为实时索引,此时的文件列表为一个logserver当前正在记录的一个文件
		程序建索引到文件尾不退车,实时监测日志文件
		当程序收到转储程序送的信号时,程序当前循环结束,重新设置环境,重新打开实时日志文件
		实时索引使用时可以考虑用logserver的方式后台运行,在/etc/rc.d/ 下加Kxx 和Sxx 启动和停止服务.停止服务发信号2.(如果为tail,会注册SIGINT信号)
			
(2)索引查询
	1.继承超类query,实现自己的查询规则(query.hpp).
	2.调用IndexQuery类方法 add,void IndexQuery::add(Query * query);
	3.调用IndexQuery类方法 go().
	3.编译程序
	4.命令行参数规则说明:
		demoindexquery application.xml 
	5.索引查询规则:
		根据query类提供的type和keyname,timerang,在索引目录中找到所有匹配的idx文件,再根据key,找相应的keyvalue,根据用户提供的回调process_protocol处理.
		如果都没有找到,确认query类的enable_scan_if_not_found_idx方法是否为true,
			如果提供目的日志文件串,用这个串文件scan文件;
			如果没有提供,则根据配置文件取一个与query->get_type()相匹配的日志文件列表,包括日志转储目录和实时记录目录的日志文件
		如果有根据日志文件串顺序扫描记录,根据用户提供的回调process_protocol处理.
	6.索引有提供同时对多个query查询的功能,	建议一个查询对应一个query

(3)需要系统命令lsof和logserver执行程序文件名

ndex,实现自己的索引规则(index.hpp).
	2.调用IndexMaker 方法 add(), void IndexMaker::add(Index * idx);
	3.调用IndexMaker 方法 make().
	3.编译程序
	4.命令行参数规则说明:
		demoindexmaker application.xml file_list mail|tail
		application.xml	应用xml配置文件(读默认目录)
		file_list	源日志文件列表(tail时只对应一个), file_list一律用""括起来,
				如果file_list为空,程序根据application.xml配置,根据添加进来index的type找到对应的logname文件名,
					根据这个文件名在转储目录下找所有匹配的日志文件.
		mail		建立完索引退出(源文件为转储目录下的日志文件),多个文件列表表示: "file1 file2 file3"
		tail		建立实时索引,循环等待(源文件为logserver正在记录的日志文件)
	
	5.索引文件名规则:
		A.执行转储后的日志文件建索引	serial_logname.startime.type.keyname.idx
			例如:日志文件00005_login.20070126160342.log
			建立索引:协议类型(type)为2004, 关键字名称(keyname)为logintime
			则相应的日志索引文件名:
				00005_login.20070126160342.2004.logintime.idx
		B.对logserver当前正在读写的文件建立索引(实时索引)	logname.starttime.type.keyname.idx
			例如:日志文件login.log
			建立索引:协议类型(type)为2004, 关键字名称(keyname)为logintime
			则相应的日志索引文件名:
				login.20070126160342.2004.logintime.idx
	6.实时索引处理:
		命令行参数为tail是为实时索引,此时的文件列表为一个logserver当前正在记录的一个文件
		程序建索引到文件尾不退车,实时监测日志文件
		当程序收到转储程序送的信号时,程序当前循环结束,重新设置环境,重新打开实时日志文件
		实时索引使用时可以考虑用logserver的方式后台运行,在/etc/rc.d/ 下加Kxx 和Sxx 启动和停止服务.停止服务发信号2.(如果为tail,会注册SIGINT信号)
			
(2)索引查询
	1.继承超类query,实现自己的查询规则(query.hpp).
	2.调用IndexQuery类方法 add,void IndexQuery::add(Query * query);
	3.调用IndexQuery类方法 go().
	3.编译程序
	4.命令行参数规则说明:
		demoindexquery application.xml 
	5.索引查询规则:
		根据query类提供的type和keyname,timerang,在索引目录中找到所有匹配的idx文件,再根据key,找相应的keyvalue,根据用户提供的回调process_protocol处理.
		如果都没有找到,确认query类的enable_scan_if_not_found_idx方法是否为true,
			如果提供目的日志文件串,用这个串文件scan文件;
			如果没有提供,则根据配置文件取一个与query->get_type()相匹配的日志文件列表,包括日志转储目录和实时记录目录的日志文件
		如果有根据日志文件串顺序扫描记录,根据用户提供的回调process_protocol处理.
	6.索引有提供同时对多个query查询的功能,	建议一个查询对应一个query

(3)需要系统命令lsof和logserver执行程序文件名

(4)如果提供文件列表进行处理,文件和目录的规则一律用绝对路径(文件名)

-------------------------------------------------------------------------------------------------------------
io库更改说明
-------------------------------------------------------------------------------------------------------------
	1.io/protocol.h
	以下 private: 改为protected;	
		 protected:
                        struct Stream : public Marshal::OctetsStream
                        {
                                Session *session;
                                mutable bool check_policy;
                                Stream(Session *s) : session(s), check_policy(true) { }
                        };      
                        Session(Manager *m) : sid(session_id++), is(this), os_bytes(0), manager(m), state(m->GetInitState()){ }                     
                        Session(const Session &rhs) : NetSession(rhs), sid(session_id++), is(this), os_bytes(rhs.os_bytes), 
					manager(rhs.manager), state(rhs.state) { }
                        typedef gnet_map<ID, Session *> Map;
                        static ID session_id;
                        ID sid;
                        Stream is;
                        std::deque<Octets>os;
                        size_t os_bytes;
                        Manager *manager;
                        const State *state;
                        Timer timer;
                        NetSession *Clone () const      { return new Session(*this); }
	

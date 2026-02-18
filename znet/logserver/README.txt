-------------------------------------------------------------------------------------------------------------
日志服务端使用说明
-------------------------------------------------------------------------------------------------------------
	1.启动登记日志服务命令行: logserver logger.conf application.xml
	2.启动转储服务命令行: logrotate logger.conf application.xml
		转储文件名规则:
			serial_logname.starttime.log
			例如: 日志文件login.log,执行转储后为
				00005_login.20070126160342.log
				00005				表示该文件在转储目录中的serial
				20070126160342	表示该文件数据的开始时间
			
		当日志文件达到转储条件后,处理步骤:
			1.转储程序查找到该日志文件打开的pid(除logserver外)
			2.发一条控制协议给logserver,通知logserver,由logsrever新建一个日志文件
			3.根据pid在日志索引目录找到这个pid打开的所有实时索引文件,根据serial,重命名这些实时索引文件为正常索引文件
			4.发一个信号给这些pid,通知文件已经转储.
			5.日志文件小于LOG_PAGESIZE不转储

		serial的产生,以转储目录为准

	3.服务的启停配置
chkconfig --add logserver
#!/bin/bash
#
# logserver        Starts logserver.
#
#
# chkconfig: 345 80 05
# description: logserver  
### BEGIN INIT INFO
# Provides: $logserver
### END INIT INFO

# Source function library.
. /etc/init.d/functions

#[ -f /sbin/syslogd ] || exit 0
#[ -f /sbin/klogd ] || exit 0

# Source config

RETVAL=0
LOGSERVER_HOME=/home/lijinhua
LOGSERVER=logserver
USER_NAME=lijinhua

if [ ! -f $LOGSERVER_HOME/logsysv3/logserver/logserver ]
then
        echo "Logserver startup: cannot start"
exit
fi

start() {
        echo -n $"Starting system logger: "
        daemon su - $USER_NAME -c $LOGSERVER_HOME/do.sh
        #daemon su - $USER_NAME -c "$LOGSERVER_HOME/logsysv3/logserver/logserver \
        #                       $LOGSERVER_HOME/logsysv3/logger.conf $LOGSERVER_HOME/logsysv3/application.xml"
        RETVAL=$?
        return $RETVAL
}

stop() {
        echo -n $"Shutting down logserver: "
        killproc $LOGSERVER -2
        echo
        RETVAL=$?
        return $RETVAL
}
restart() {
        stop
        start
}
case "$1" in
  start)
        start
        ;;
  stop) 
        stop
        ;;
  restart)
        restart
        ;;
  *)    
        echo $"Usage: $0 {start|stop|restart|}"
        exit 1
esac

exit $?

	4.日志服务端代码需要更改io库,具体见后文
	5.需要申请一个协议id作为控制协议id, 现在程序暂定type=12000
	6.日志服务端执行文件为logserver,不能更改,其他模块有用

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
                                Stream(Session *s) : session(s),
check_policy(true) { }
                        };      
                        Session(Manager *m) : sid(session_id++), is(this),
os_bytes(0), manager(m), state(m->GetInitState()){ }                     
                        Session(const Session &rhs) : NetSession(rhs),
sid(session_id++), is(this), os_bytes(rhs.os_bytes), 
					manager(rhs.manager), state(rhs.state)
{ }
                        typedef gnet_map<ID, Session *> Map;
                        static ID session_id;
                        ID sid;
                        Stream is;
                        std::deque<Octets>os;
                        size_t os_bytes;
                        Manager *manager;
                        const State *state;
                        Timer timer;
                        NetSession *Clone () const      { return new
Session(*this); }

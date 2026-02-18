#ifndef __GNET_GLINKCLIENT_HPP
#define __GNET_GLINKCLIENT_HPP

#include "protocol.h"
#include "timersender.h"
#include "keepalive.hpp"
#include "roleinfo"
#include "clientdef.h"
#include "itimer.h"
#include "gamedatasend.hpp"
namespace GNET
{
	enum{
		GLoginClient,
		GSelectRoleClient,
		GDataExchgClient,
	};
	using namespace C2S;

	enum EClientState
	{
		ECS_PREPARE_CONNECT,// 准备去连接link
		ECS_CONNECTED,// link连接ok
		ECS_PREPARE_SEND_RESPONSE, // 向link发送用户名和密码
		ECS_PREPARE_SEND_ROLELIST,// 发送申请角色列表请求
		ECS_PREPARE_SELECTROLE, // 发送选择角色请求
		ECS_ENTER_WORLD, // 登录游戏
		ECS_COUNT,
	};

	static const char* const szClientState[ECS_COUNT] = {
		"准备去连link，如果是此状态，表示link连接不上",
		"link连接ok",
		"准备向link发送用户名密码,如果是此状态,有可能是用户名或密码没有验证通过，或是gdelivery或是AU有问题",
		"发送列表申请请求，gdelivery或是数据库服务器问题",
		"发送角色选择请求，逻辑服务器可能有问题",
		"登录成功",	
	};

	class GLinkClient : public Protocol::Manager
	{
		static GLinkClient instance;
		size_t      accumulate_limit;
		Session::ID sid;
		bool        conn_state;
		Thread::Mutex   locker_state;
		const Session::State *GetInitState() const;
		bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
		void OnAddSession(Session::ID sid);
		void OnDelSession(Session::ID sid);
		void OnAbortSession(Session::ID sid);
		void OnAbortSession(const SockAddr &sa);
		void OnCheckAddress(SockAddr &) const;
		TimerSender timer_sender;
	public:
		static GLinkClient *GetInstance() { return &instance; }
		std::string Identification() const { return "GLinkClient"; }
		void SetChoice(char* ch){
			memset(choice,0,sizeof(choice));
			strncpy(choice,ch,sizeof(choice)-1);
		}
		Conf* GetConf() { return m_conf; }
		void SetConf(Conf *conf) { m_conf = conf; }
		char* GetChoice(){return choice;}
		int GetRoomID(){return roomid;}
		void SetRoomID(int room){roomid=room;}
		void SetUsername(const std::string & str){username=str;}
		const std::string & GetUsername(){return username;}
		void SetAccumulate(size_t size) { accumulate_limit = size; }
		void SetLineId(int lineid){m_lineid=lineid;}
		void SetOccup(int occup){m_occup=occup;}
		EClientState GetClientState() { return m_eClientState; }
		void SetClientState(EClientState state) { m_eClientState = state; }
		GLinkClient() :
			accumulate_limit(0),
			conn_state(false),
			locker_state("glinkclient::locker_state"),
			timer_sender(this) ,
			locker_rolenamemap("glinkclient::locker_rolenamemap"),
			roleid(0),
			m_wMoveStamp(0),
			m_fSpeed(4.2),
			m_nInterval(1),
			m_eStatus(EPLAYER_STATUS_WAITINITPOS),
			m_wander_change(false),
			m_conf(NULL),
			m_eClientState(ECS_PREPARE_CONNECT)
		{
			time_t _now; time(&_now);
			m_nWanderTime = _now+m_nInterval;
		}

		bool SendProtocol(const Protocol &protocol) { return conn_state && Send(sid, &protocol); }
		bool SendProtocol(const Protocol *protocol) { return conn_state && Send(sid, protocol); }

		void SetTimerSenderSize(size_t size) { timer_sender.SetSizeLimit(size); }
		void RunTimerSender(size_t ticks=1) { timer_sender.Run(ticks); }
		void AccumulateSend(Protocol *protocol) { if (conn_state) timer_sender.SendProtocol(sid,protocol); }

		void SetPosition(float x, float y, float z) { m_position.x=x,m_position.y=y,m_position.z=z; }

		void InitWanderState()
		{
			int _scene_id = atoi(m_conf->find(Identification(), "scene_id").c_str());
                        int _init_x = atoi(m_conf->find(Identification(), "init_x").c_str());	
			float _init_y = atof(m_conf->find(Identification(), "init_y").c_str());
                        int _init_z = atoi(m_conf->find(Identification(), "init_z").c_str());
			m_position.x = _init_x;
                        m_position.z = _init_z;
                        m_position.y = _init_y;
			SendDebugCmd(2008,3,_scene_id,_init_x,(int)_init_z);
			//SendDebugCmd(2007,4,_scene_id,_init_x,(int)_init_y,_init_z);
		}


		void WanderDirection(void)
		{
			int _min_x = atoi(m_conf->find(Identification(), "min_x").c_str());
			int _min_z = atoi(m_conf->find(Identification(), "min_z").c_str());
			int _max_x = atoi(m_conf->find(Identification(), "max_x").c_str());
			int _max_z = atoi(m_conf->find(Identification(), "max_z").c_str());

			int _circle_count = 10;

			while (_circle_count)
			{
				RandomDirection();
				if (m_wander_direction.x*2 + m_position.x > _min_x  && m_wander_direction.x*2 + m_position.x < _max_x && m_wander_direction.z*2 + m_position.z > _min_z && m_wander_direction.z*2 + m_position.z < _max_z)
					break;

				_circle_count--;
			}

			m_wander_change = true;
		}

		void RandomDirection(void)
		{
			float _direction = (rand() % 360) * 3.1415926f / 180.0f;
			m_wander_direction.x = cos(_direction);
			m_wander_direction.z = sin(_direction);
		}

		void RandomWalking()
		{
			if(m_eStatus == EPLAYER_STATUS_WAITINITPOS)
			{
				InitWanderState();
				m_eStatus = EPLAYER_STATUS_WAITMOVESTART;
				//printf("curroos(%f,%f,%f)\n",m_position.x,m_position.y,m_position.z);
				return;
			}
			
			if(m_eStatus == EPLAYER_STATUS_WAITMOVESTART)
			{
				//printf("SendCmdStartMove\n");
				SendCmdStartMove();
				m_eStatus = EPLAYER_STATUS_WAITMOVEEND;
				time_t _now; time(&_now);
                       		m_nWanderTime = _now+m_nInterval;
			}

			time_t _now; time(&_now);
			if(m_nWanderTime < _now && m_eStatus == EPLAYER_STATUS_WAITMOVEEND)
			{
                               	WanderDirection();

				A3DVECTOR _position_des;

	                        m_position.x = _position_des.x = m_position.x + m_wander_direction.x*2 ;
        	                m_position.z = _position_des.z = m_position.z + m_wander_direction.z*2 ;
                	        m_position.y = _position_des.y = m_position.y ;

                        	//printf("curroos(%f,%f,%f) despos(%f,%f,%f) wander(%f,%f,%f) m_fSpeed:%f,m_nInterval:%d\n",m_position.x,m_position.y,m_position.z, _position_des.x,_position_des.y,_position_des.z,m_wander_direction.x,m_wander_direction.y,m_wander_direction.z,m_fSpeed,m_nInterval);
                        	//SendCmdPlayerMove(m_position, _position_des,500,4,_iMoveMode|GP_MOVE_DEAD,m_wMoveStamp++);
                        	SendCmdStopMove(_position_des,m_fSpeed,MOVEENV_WATER|GP_MOVE_DEAD,0,m_wMoveStamp++,m_nInterval*1000);  
				//SendCmdStopMove(_position_des,4.5,33,0,m_wMoveStamp++,900);	
				m_eStatus = EPLAYER_STATUS_WAITMOVESTART;

				m_nWanderTime = _now+m_nInterval;
			}

		}

		void SendCmdStartMove()
		{
			using namespace C2S;

			int iSize = sizeof (cmd_header);
			BYTE* pBuf = new BYTE [iSize];
			if (!pBuf)
				return;
	
			((cmd_header*)pBuf)->cmd = START_MOVE;
			SendGameData(pBuf, iSize);

			delete [] pBuf;
		}

		void SendDebugCmd(int iCmd, int iNumParams, ...)
		{
			using namespace C2S;

			int iSize = sizeof (cmd_header) + iNumParams * sizeof (int);
			BYTE* pBuf = new BYTE [iSize];
			if (!pBuf)
				return;

			((cmd_header*)pBuf)->cmd = iCmd;

			/*cmd_jump_move* pCmd = (cmd_jump_move*)(pBuf + sizeof(cmd_header));
			pCmd->tag = 16;
			pCmd->x = 95;
			pCmd->y = 42;
			pCmd->z = -99;*/

			if (iNumParams)
			{
				va_list vaList;
				va_start(vaList, iNumParams);

				int* pData = (int*)(pBuf + sizeof (cmd_header));
				for (int i=0; i < iNumParams; i++)
					*pData++ = va_arg(vaList, int);

				va_end(vaList);
			}
			//printf("send cmddebug\n");
			SendGameData(pBuf, iSize);

			delete [] pBuf;
		}

		void SendCmdPlayerMove(const A3DVECTOR& vCurPos, const A3DVECTOR& vDest, 
						   int iTime, float fSpeed, int iMoveMode, unsigned short wStamp)
		{
			using namespace C2S;

			int iSize = sizeof (cmd_header) + sizeof (cmd_player_move);
			BYTE* pBuf = new BYTE [iSize];
			if (!pBuf)
				return;
	
			((cmd_header*)pBuf)->cmd = PLAYER_MOVE;

			cmd_player_move* pCmd = (cmd_player_move*)(pBuf + sizeof (cmd_header));
			pCmd->move_mode	= (BYTE)iMoveMode;
			pCmd->use_time	= (unsigned short)iTime;
			pCmd->vCurPos	= vCurPos;
			pCmd->vNextPos	= vDest;
			pCmd->sSpeed	= FLOATTOFIX8(fSpeed);
			pCmd->stamp		= wStamp;

			SendGameData(pBuf, iSize);

			delete [] pBuf;
		}

		//	Send C2S::STOP_MOVE command
		void SendCmdStopMove(const A3DVECTOR& vDest, float fSpeed, int iMoveMode,
								 BYTE byDir, unsigned short wStamp, int iTime)
		{
			using namespace C2S;

			int iSize = sizeof (cmd_header) + sizeof (cmd_stop_move);
			BYTE* pBuf = new BYTE [iSize];
			if (!pBuf)
				return;
			
			((cmd_header*)pBuf)->cmd = STOP_MOVE;

			cmd_stop_move* pCmd = (cmd_stop_move*)(pBuf + sizeof (cmd_header));
			pCmd->move_mode	= (BYTE)iMoveMode;
			pCmd->sSpeed	= FLOATTOFIX8(fSpeed);
			pCmd->dir		= byDir;
			pCmd->vCurPos	= vDest;
			pCmd->stamp		= wStamp;
			pCmd->use_time	= (unsigned short)iTime;

			SendGameData(pBuf, iSize);

			delete [] pBuf;
		}
		bool SendGameData(void* pData, int iSize)
		{

			GamedataSend p;
			p.data.replace(pData, iSize);
			return SendProtocol(p);
		}

		Session::ID GetActiveSid() { return sid; }

		typedef GNET::RpcDataVector<RoleInfo> RoleInfoVector;
		typedef std::map<int/*roleid*/,Octets> RolenameMap;
		Thread::Mutex locker_rolenamemap;
		RolenameMap rolenamemap;

		Octets identity;
		Octets password;
		int userid;
		int tid;  //transaction id
		bool blkickuser;
		int roleid;
		char choice[64];
		std::string username;
		int roomid;
		Octets m_nonce;
		RoleInfoVector m_vRoleinfo;
		int m_lineid;
		int m_occup;
		A3DVECTOR m_position; // 玩家当前位置
		unsigned short m_wMoveStamp;		//	Move stamp
		float m_fSpeed;//移动速度
		int m_nInterval;// 发送移动数据包的时间间隔
		EPlayerStatus m_eStatus;// 移动状态
		A3DVECTOR m_wander_direction; // 下一次随机目标距离
		bool m_wander_change; // 行走目标改变
		time_t m_nWanderTime;// 开始发送移动数据包时间
		Conf *m_conf;
		EClientState m_eClientState;
	};

	class KeepAliveTask : public Thread::Runnable
	{
		unsigned int delay;
		public:
		KeepAliveTask(unsigned int _delay,unsigned int priority=1) : Runnable(priority),delay(_delay) { }
		~KeepAliveTask() { }
		void Run()
		{
			GLinkClient::GetInstance()->SendProtocol(KeepAlive((unsigned int)PROTOCOL_KEEPALIVE));
			Thread::HouseKeeper::AddTimerTask(this,delay);
		}
	};

	class RandomWalkTask : public Thread::Runnable
	{
		GLinkClient* m_pClient;
		int64_t m_start;
		int m_delay;
	public:
		void bindClient(GLinkClient* pClient) { m_pClient = pClient; }
		void Run( )
		{		
			//int64_t _now = IntervalTimer::GetTick();
			if(m_pClient)
				m_pClient->RandomWalking();
			
			//Thread::Pool::AddTask(this);						
			Thread::HouseKeeper::AddTimerTask(this,1);	

		}

		RandomWalkTask(int t,GLinkClient* pClient)
		{
			m_delay = t;
			m_pClient = pClient;
			m_start = IntervalTimer::GetTick();
			
		}
	};
	
	class PrintClientStateTask : public Thread::Runnable
	{
		void log(char* str)
		{
			FILE* fp = NULL;
			char* path = "server.log";
			if((fp = fopen(path, "a")) == NULL)
			{
				fprintf(stderr, "无法打开日志文件[%s] %s\n", path, strerror(errno));
				fp = stderr;
			}
			int fd;
			fd = fileno(fp);
			fcntl(fd, F_LOCK, 01);
			fprintf(fp, "%s", str);
			fflush(fp);
			fcntl(fd, F_ULOCK, 01);

		}
	public:
		GLinkClient* m_pClient;
		PrintClientStateTask(GLinkClient* pClient) { m_pClient = pClient;  }
		~PrintClientStateTask() { }
		void Run()
		{
			//std::string str;
			//str.append("IP:").append(m_pClient->GetConf()->find("GLinkClient", "address").c_str()).append(",Port:").append(m_pClient->GetConf()->find("GLinkClient", "port").c_str()).append(",State:").append(itos(m_pClient->GetClientState()));

			char buf[200];
			memset(buf,0,200);
			sprintf(buf, "IP:%s,Port:%d,state:%d,%s\n", m_pClient->GetConf()->find("GLinkClient", "address").c_str(), atoi(m_pClient->GetConf()->find("GLinkClient", "port").c_str()),m_pClient->GetClientState(),szClientState[m_pClient->GetClientState()]);
			log(buf);
		}
	};

};
#endif

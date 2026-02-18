#ifndef __ONLINEG_GAME_GS_PLAYER_TRASH_BOX_H__
#define __ONLINEG_GAME_GS_PLAYER_TRASH_BOX_H__

#include "item.h"
#include "item_list.h"
#include "config.h"
#include <string.h>
#include <vector.h>

class player_trashbox
{
	size_t 	  	_money;			//仓库金钱数目
	item_list 	_box;			//仓库内容
	item_list	_mafia_box;		
	unsigned char	_passwd[16];		//密码的MD5
	unsigned short	_change_counter;	//仓库修改计数标志
	bool		_has_passwd;		//是否有密码
	char		_open_flag;		//物品箱打开标志
public:
	enum
	{
		MAX_PASSWORD_SIZE = 24,
		MAX_TRASHBOX_SIE =  TRASHBOX_MAX_SIZE,
	};
	static bool IsPasswordValid(const char * str, size_t size)
	{
		static char pass_char[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()+-_='`~;:,<.>/? \"";
		if(size == 0) return true;
		if(size > MAX_PASSWORD_SIZE) return false;
		for(size_t i = 0; i < size;i ++)
		{
			if(strchr(pass_char,str[i]) == NULL) return false;
		}
		return true;
	}

	void SetPassword(const char * str, size_t size);
	void SetPasswordMD5(const char * str, size_t size);
	bool CheckPassword(const char * str, size_t size);

	bool HasPassword()
	{
		return _has_passwd;
	}

	const char * GetPassword(size_t & size)
	{
		if(_has_passwd)
		{
			size = 16;
			return (const char*) _passwd;
		}
		else
		{
			size = 0;
			return NULL;
		}
	}
	
	enum
	{
		TRASHBOX_OPEN = 1,
		MAFIA_TB_OPEN = 2,
	};
	
public:
	player_trashbox():_money(0),_box(item::BACKPACK,TRASHBOX_BASE_SIZE),
			  _mafia_box(item::BACKPACK, MAFIA_TRASHBOX_BASE_SIZE),
			  _change_counter(1),_has_passwd(false),_open_flag(0)
	{
	}

	inline void Open(int type =  TRASHBOX_OPEN)
	{
		ASSERT(type == TRASHBOX_OPEN  || type == MAFIA_TB_OPEN);
		_open_flag = type;
		IncChangeCounter();
	}

	inline void Close()
	{
		_open_flag = 0;
	}

	inline bool IsOpened()
	{
		return _open_flag;
	}
	
	inline bool IsNormalBoxOpen()
	{
		return _open_flag == TRASHBOX_OPEN;
	}

	inline bool IsMafiaBoxOpen()
	{
		return _open_flag == MAFIA_TB_OPEN;
	}

	inline unsigned short GetChangeCounter()
	{
		return _change_counter;
	}

	inline void ClrChangeCounter()
	{
		_change_counter = 0;
	}

	inline bool IsTrashBoxChanged()
	{
		return _change_counter;
	}

	inline void IncChangeCounter()
	{
		_change_counter ++;          //这时即认为物品箱发生了变化
		if(_change_counter == 0) _change_counter = 1;
	}

	
	void SetOwner(gactive_imp * obj)
	{
		_box.SetOwner(obj);
		_mafia_box.SetOwner(obj);
	}

	size_t & GetMoney() { return _money; }
	item_list & Backpack() { return _box;}
	item_list & MafiaBackpack() { return _mafia_box;}
	item_list & GetCurBackpack() 
	{
		if(IsMafiaBoxOpen()) 
			return _mafia_box;
		else
			return _box;
	}

	int GetTrashBoxSize()
	{
		return _box.Size();
	}

	int GetMafiaTrashBoxSize()
	{
		return _mafia_box.Size();
	}

	int GetCurTrashBoxSize()
	{
		return GetCurBackpack().Size();
	}

	void SetTrashBoxSize(int size)
	{
		if((int)_box.Size() >= size) return ;
		if(size > MAX_TRASHBOX_SIE) return;
		_box.SetSize(size);
	}

	void SetMafiaTrashBoxSize(int size)
	{
		if((int)_mafia_box.Size() >= size) return ;
		if(size > MAX_TRASHBOX_SIE) return;
		_mafia_box.SetSize(size);
	}
	

	bool Save(archive & ar)
	{
		ar << _money;
		_box.Save(ar);
		ar << _has_passwd;
		ar.push_back(_passwd,16);
		return true;
	}

	bool Load(archive & ar)
	{
		ar >> _money;
		_box.Load(ar);
		ar >> _has_passwd;
		ar.pop_back(_passwd,16);
		return true;
	}

	void Swap(player_trashbox & rhs)
	{
		abase::swap(_money,rhs._money);
		_box.Swap(rhs._box);
		_mafia_box.Swap(rhs._mafia_box);
		abase::swap(_has_passwd,rhs._has_passwd);
		abase::swap(_change_counter,rhs._change_counter);
		abase::swap(_open_flag,rhs._open_flag);

		unsigned char	passwd[16];
		ASSERT(sizeof(passwd) == sizeof(_passwd));
		memcpy(passwd,_passwd,sizeof(_passwd));
		memcpy(_passwd,rhs._passwd,sizeof(_passwd));
		memcpy(rhs._passwd,passwd,sizeof(_passwd));
	}
};

#endif


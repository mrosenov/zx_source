#ifndef _GENERAL_INDEXER_H_
#define _GENERAL_INDEXER_H_

extern bool IsGlobalIndex(int idx);
extern bool IsLineIndex(int idx);
extern bool IsMapIndex(int idx);

class GeneralIndexer
{
public:
    GeneralIndexer(bool check_permission=true):_check_perm(check_permission),_world_tag(-1),_world_index(-1) {}
    void SetParam(int world_tag, int world_index, int param3) 
    {
	_world_tag = world_tag;
	_world_index = world_index;
	_param3 = param3;
    }
    bool Set(int index, int value);
    bool Modify(int index, int offset);
    bool Get(int index, int & value);
private:
    bool _check_perm;
    int  _world_tag;
    int  _world_index;
    int  _param3;
    bool _IsAccessible(int index);
};
#endif

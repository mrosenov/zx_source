
#ifndef __ONLINEGAME_GAME_GS_COMMON_DATA_IF_H_
#define __ONLINEGAME_GAME_GS_COMMON_DATA_IF_H_

class CommonData;

class CommonDataInterface
{
      CommonData* _pcd;
public:

      class Iterator
      {       
      public: 
	      Iterator(){}
	      virtual ~Iterator(){} 
	      virtual void operator()(int data_type, int key, int value)=0;
      };      
      CommonDataInterface(int data_type=0);

      bool IsConsistent();

      bool GetData(int index, int& val);
      void SetData(int index, int val);

      bool GetUserData(int index, void *data, int *size);
      void SetUserData(int index, void *data, int size);

      void IncData (int index);
      void DecData (int index);
 
      void AddData (int index, int diff_val);
      void SubData (int index, int diff_val);

      void ForEach(CommonDataInterface::Iterator &it);
};


//Rpc用于精确控制某个value. 
class  CommonDataRpcArg
{
public:
      int _data_type;		//数据空间
      int _index;		//索引
      int _diff_val;		//增量（可以为负值）
      int _range_low;		//下限
      int _range_high;		//上限,如果delivery上的数值在设置增量后，超出合理值，则rpc操作失败

      CommonDataRpcArg(){}

      CommonDataRpcArg(int data_type, int index, int diff_val, int range_low, int range_high):
       _data_type(data_type),_index(index), _diff_val(diff_val), 
       _range_low(range_low),_range_high(range_high) { }
};

enum op_mode {
        op_mode_inc,    //自增
        op_mode_dec,    //自减
        op_mode_set,    //设置
        op_mode_add,    //加上某值
        op_mode_sub,    //减去某值
        op_mode_xchg,   //交换
};

class CommonDataRpcObject
{
protected:
      int _op;
      CommonDataRpcArg _arg;
public:
      CommonDataRpcObject(int op = op_mode_add): _op(op){}
      virtual ~CommonDataRpcObject() {}

      void SetArg(const CommonDataRpcArg& arg) { _arg = arg;}
      int  GetOp () { return _op;}
      void SetOp (int op) {_op = op;}

      enum {
	      RESULT_OK,		//成功
	      RESULT_OUT_OF_RANGE, 	//加之后，数值会超过边界
	      RESULT_TIMEOUT,		//超时
	      RESULT_DEST_IS_LOCAL,	//所操作的目标是内部空间的数据
      };
      /*
       *  获得结果的回调
       *  @param ret_val  返回的值，对交换操作，是原先的值。其它操作，是当前的值
       *  @param result   结果
       *  @note  在收到OnGetResult后，一次调用的过程结束。可以再次将对象发出，或者自我释放。
       */
      virtual void OnGetResult(int ret_val,int result) =0;
};

/*
 * 将Rpc对象发出
 * @param obj 
 * @param arg 所需要的参数
 * @note 在收到返回前，不能释放obj
 */
void SendCommonDataRpcObject (CommonDataRpcObject *obj, const CommonDataRpcArg& arg);

enum {
	CMN_DATA_LOCAL   = 0,  //内部的使用的，非共享
	CMN_DATA_SHARE	 = 1,  //在GS间共享
	CMN_DATA_NEED_DB = 2,  //需要存盘
	CMN_DATA_DEFAULT = 3,  //默认 (CMN_DATA_SHARE | CMN_DATA_NEED_DB)
};
/*
 * 创建数据空间
 * @param data_type 需要创建的数据空间类型
 * @param flag  数据空间的属性, 创建一个local型可以用 CMN_DATA_LOCAL
 */
bool CommonDataSpaceCreate (int data_type, int flag = CMN_DATA_SHARE | CMN_DATA_NEED_DB);

/**
 * 销毁已经创建的数据空间
 * @param data_type, 需要删除的数据空间类型
 */
bool CommonDataSpaceDestroy(int data_type);
#endif

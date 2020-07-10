#pragma once
#include <vector>
#include <string>
#include <future>

//电机及运动相关

using namespace std;

#define CFGDIR(X) "D:/Auto_Sloder_Welding/config/"##X

enum hardwaretype	
{
	motion_card,
	io_card,
	extend_card,
	none_card
};

enum hardwarevender
{
	test,
	leisai,
	adlink,
	none
};

//错误代码
enum EM_ERR_CODE
{
    RETURN_OK = 0,                  //正常返回
};

//class Semaphore {
//public:
//    Semaphore (int count_ = 0)
//    : count(count_) 
//    {
//    }
//    
//    inline void notify( int tid ) {
//        std::unique_lock<std::mutex> lock(mtx);
//        count++;
//        cout << "thread " << tid <<  " notify" << endl;
//        //notify the waiting thread
//        cv.notify_one();
//    }
//    inline void wait( int tid ) {
//        std::unique_lock<std::mutex> lock(mtx);
//        while(count == 0) {
//            cout << "thread " << tid << " wait" << endl;
//            //wait on the mutex until notify is called
//            cv.wait(lock);
//            cout << "thread " << tid << " run" << endl;
//        }
//        count--;
//    }
//private:
//    std::mutex mtx;
//    std::condition_variable cv;
//    int count;
//};

//IO类型
enum EM_IO_TYPE
{
	EM_IO_START = 0,		//开始io
	EM_IO_RESET,			//复位io
	EM_IO_ESTOP,			//急停
	EM_IO_PAUSE,			//暂停
	EM_IO_STOP,				//停止操作IO
	EM_IO_USUALLY,			//常用IO
	EM_IO_BRAKE,			//刹车IO
	EM_IO_OTHER,			//其它
};

//卡信息
struct STM_CARD  
{
	unsigned short cardIndex;   //卡索引
	unsigned short axisCnt;     //轴个数
	unsigned short ioCnt;       //主io位数
	unsigned short extCnt;      //扩展模块个数
	unsigned short cntType;     //连接卡连接方式
	STM_CARD()
	{
		cardIndex = 0;
		axisCnt = 0;
		ioCnt = 0;
		extCnt = 0;
		cntType = 0;
	}
};

enum EM_AXIS_STATUS_TYPE
{
	EM_AXIS_ALARM = 1,			//驱动报警
	EM_AXIS_LIMIT_P = 5,		//限位触发
	EM_AXIS_LIMIT_L = 6,		//限位触发
	EM_AXIS_ESTOP = 8,			//急停io
	EM_AXIS_ENABLE = 9,			//使能标志
	EM_AXIS_MOVING = 10,		//规划器运动标志
};
//坐标ID，每个卡只能有2个
enum EM_CRDID
{
	EM_CRD_FST = 1,
	EM_CRD_SEC,
};

//轴类型
enum EM_AXIS_TYPE
{
	EM_AXIS_SERVO = 0,      //伺服
	EM_AXIS_STEPPER,        //步进电机
	EM_AXIS_DDMOTOR,        //DD马达
	EM_AXIS_FLOW,           //流水线皮带，不需要回原
};

//IO信号类型
enum EM_GPIO_TYPE
{
	EM_GPO = 0,     //通用输出  输出
	EM_GPI,         //通用输入
	EM_LIMIT_POS,   //正限位
	EM_LIMIT_NEG,   //负限位
	EM_ALARM,       //报警信号
	EM_HOME,        //原点信号
	EM_ENABLE,      //伺服使能  输出
	EM_CLEAR,       //清除报警  输出
	EM_ADC,         //模拟量
	EM_RDY,		    //read信号
	EM_INP		    //inp 轴到位信号
};
//回限位方式
enum EM_LIMIT_TYPE
{
	EM_AXIS_LIMITN = 0,     //回负限位
	EM_AXIS_LIMITP,         //回正限位
	EM_AXIS_NOLIMIT,        //无限位
};

//回原方式
enum EM_HOME_TYPE
{
	EM_HOME_NORMAL = 0,     //正常回原
	EM_HOME_SPECIAL1,       //硬件捕获 特殊方式1
	EM_HOME_SPECIAL2,       //index+home 特殊方式2
	EM_HOME_SELF,           //自定义回原
};

//速度结构
typedef struct _TSpeed
{
	double vel;
	double acc;
	double dec;
} TSpeed;

//IO参数
typedef struct _IOInfo
{
	string name;
	int		cardType;		//0=运动控制卡本身，1=IOC0640,3=Can总线
	int		cardIndex;		//卡号
	int		cardNode;		//Can节点，运动控制卡下面的节点号
	int		ioIndex;		//io索引
	int		sense;			//1=高电平有效，0-低电平有效
	int		group;			//分组

	_IOInfo()
	{
		name = "";
		cardType = 0;
		cardIndex = 0;
		cardNode = 0;
		ioIndex = 0;
		sense = 1;
		group = 0;
	}

	_IOInfo& operator=(const _IOInfo& other)
	{
		name = other.name;
		cardType = other.cardType;
		cardIndex = other.cardIndex;
		cardNode = other.cardNode;
		ioIndex = other.ioIndex;
		sense = other.sense;
		group = other.group;
		return *this;
	}

}IOInfo, *IOInfoPtr;

//轴参数
typedef struct _AxisParam
{
	int			cardIndex;				    //卡索引
	int			axisIndex;				    //轴索引
	int			motorType;				    //参考 EM_AXIS_TYPE 0-伺服, 1步进, 2 dd马达
	string		axisName;				    //轴名称
	string      axisDes;					//轴描述
	
	double		vel;						//默认工作速度  
	double		acc;						//加速
	double		dec;						//减速
	double		homeVel;					//回原速度 速度加速度 都需要 / 1000
	double		homeAcc;					//加速
	double		homeDec;					//减速
	double      homeVelToLimit;				//回零时，轴向限位移动的速度
	double      homeAccToLimit;				//回零时，轴向限位移动的加速度
	double      homeDecToLimit;				//回零时，轴向限位移动的减速度

	double		limitN;						//负软限位脉冲
	double		limitP;						//正软限位脉冲
	int			homeDir;					//回限位方式，EM_LIMIT_TYPE 0 负限位 1 回正限位 2 无限位
	int			homeType;				    //回原方式 参考 EM_HOME_TYPE 0 正常 1 硬件捕获 2 index 3 自定义 dd马达类型时表示 回原操作IO 8:null,8:card,8:ioindex,8:extmodle
	double		searchHomePos;				//回原搜索的距离 脉冲
	double		inHomePosOffset;			//原点偏移量 回原前，若处于原点位置，则偏移
	double		afterHomePosOffset;			//回原后偏移量
	int			pulseInMode;				//脉冲类型
	int			MMPerround;					//齿轮比 MMGearRatio
	int			PulseOneMM;					//1mm对应多少脉冲
	int			pulseperround;				//一圈多少脉冲
	int			limitPinLogic;				//限位有效电平
	bool		enableSLimit;				//是否启用软限位
	bool		enableAlm;					//报警启用/禁止模式
	int			almPinLogic;				//触发报警信号的有效电平类型
	int			pulseOutMode;				//脉冲输出类型（0:非AB相 1:AB相1倍 2:AB相2倍 3:AB相4倍)
	int			homePinLogic;				//触发原点信号的有效电平类型

	bool		enable;
	_AxisParam()
	{
		cardIndex = 0;
		axisIndex = 0;
		motorType = 0;
		axisName = "";
		axisDes = "";

		vel = 0;
		acc = 0;
		dec = 0;
		homeVel = 0;
		homeAcc = 0;
		homeDec = 0;		
		homeVelToLimit = 0;
		homeAccToLimit = 0;
		homeDecToLimit = 0;

		limitN = 0;
		limitP = 0;
		homeDir = 0;
		homeType = 0;
		searchHomePos = 0;
		inHomePosOffset = 0;
		afterHomePosOffset = 0;
		pulseInMode = 0;
		MMPerround = 0;
		PulseOneMM = 0;
		pulseperround = 0;
		limitPinLogic = 0;
		enableSLimit = false;
		enableAlm = false;
		almPinLogic = 0;
		pulseOutMode = 0;
		homePinLogic = 0;

		enable = false;
	}

	_AxisParam& operator = (const _AxisParam& other)
	{
		cardIndex = other.cardIndex;
		axisIndex = other.axisIndex;
		motorType = other.motorType;
		axisName = other.axisName;
		axisDes = other.axisDes;

		vel = other.vel;
		acc = other.acc;
		dec = other.dec;
		homeVel = other.homeVel;
		homeAcc = other.homeAcc;
		homeDec = other.homeDec;
		homeVelToLimit = other.homeVelToLimit;
		homeAccToLimit = other.homeAccToLimit;
		homeDecToLimit = other.homeDecToLimit;

		limitN = other.limitN;
		limitP = other.limitP;
		homeDir = other.homeDir;
		homeType = other.homeType;	
		searchHomePos = other.searchHomePos;
		inHomePosOffset = other.inHomePosOffset;
		afterHomePosOffset = other.afterHomePosOffset;
		pulseInMode = other.pulseInMode;
		MMPerround = other.MMPerround;
		PulseOneMM = other.PulseOneMM;
		pulseperround = other.pulseperround;
		limitPinLogic = other.limitPinLogic;
		enableSLimit = other.enableSLimit;
		enableAlm = other.enableAlm;
		almPinLogic = other.almPinLogic;
		pulseOutMode = other.pulseOutMode;
		homePinLogic = other.homePinLogic;

		enable = other.enable;

		return *this;
	}

} AxisParam, *AxisParamPtr;

typedef struct _ExtIOCard
{
	int		extID;		//扩展卡号
	int		address;	//地址
	bool	bInit;		//是否初始化

	_ExtIOCard()
	{
		extID = 0;
		address = 0x00000;
		bInit = false;
	}

	_ExtIOCard& operator = (const _ExtIOCard& other)
	{
		extID = other.extID;
		address = other.address;
		bInit = other.bInit;

		return *this;
	}
}ExtIOCard, *ExtIOCardPtr;

typedef struct _CardInfo
{
	std::string cardName;
	int cardIndex;
	std::vector<AxisParam>	vecAxisParam;
	bool bInit;

	_CardInfo()
	{
		bInit = false;
	}

	_CardInfo& operator = (const _CardInfo& other)
	{
		cardName = other.cardName;
		cardIndex = other.cardIndex;
		vecAxisParam = other.vecAxisParam;
		bInit = other.bInit;
		return *this;
	}

}CardInfo, *CardInfoPtr;

enum CardType
{
	MOTION,
	IO,
	CAN_IO,
	RS232,
	NONE
};

typedef struct newCardInfo
{
	CardType card_type;
	std::string card_name;
	int card_id;
	int card_port;
	string card_version;
}NewCardInfo;



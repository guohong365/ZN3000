#pragma once

#pragma pack(1)

struct Packet
{
	BYTE Header;                     //帧头 0xCA
    UINT16 FrameCount;               //帧计数(大端格式)
    BYTE Mode;                       //工作模式0--检测 1--校准
    INT16 Feedback;                  //反馈电压值（原始数据，大端格式）
    INT16 Admittance;                //导纳通道数据（原始数据，大端格式）
    INT16 Differential;              //微分通道数据（原始数据，大端格式）
    INT16 ECG;                       //心电通道数据（原始数据，大端格式）
    BYTE CheckSum;                   //和校验
    BYTE Tail;
};
typedef union _tagDataBuffer
{
	BYTE Buffer[sizeof(Packet)];
	Packet Paket;
} DataBuffer;
bool checkPacket(const DataBuffer * packet);
struct CommandPacket
{
	BYTE Header;                   //帧头0xCB
    BYTE Mode;                   //0---检测   1---校准
    UINT16 Reserved;               //保留
    BYTE CheckSum;               //和校验
    BYTE Tail;                   //帧尾0xF1
};

#define CHECK_COMMAND {0xCB, 1, 0, 0xCC, 0xF1}
#define EXAMINE_COMMAND {0xCB, 0, 0, 0xCB, 0xF1}

union CommandBuffer
{
	BYTE Buffer[sizeof(CommandPacket)];
	CommandPacket Paket;
};
#pragma pack()


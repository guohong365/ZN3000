#pragma once

#pragma pack(1)

struct Packet
{
	BYTE Header;                     //帧头 0xCA
    UINT16 FrameCount;               //帧计数(大端格式)
    BYTE Mode;                       //工作模式0--检测 1--校准
    UINT16 Feedback;                  //反馈电压值（原始数据，大端格式）
    UINT16 Admittance;                //导纳通道数据（原始数据，大端格式）
    UINT16 Differential;              //微分通道数据（原始数据，大端格式）
    UINT16 ECG;                       //心电通道数据（原始数据，大端格式）
    BYTE CheckSum;                   //和校验
    BYTE Tail;
};
typedef union _tagDataBuffer
{
	BYTE Buffer[sizeof(Packet)];
	Packet Paket;
} DataBuffer;


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
inline bool checkPacket(const DataBuffer* packet)
{
	BYTE sum =0;
	for(int i=0; i< sizeof(Packet) - 2; i++){
		sum +=packet->Buffer[i];
	}
	return sum==packet->Paket.CheckSum;
}
inline void swap2Bytes(BYTE* p)
{
	BYTE byte = p[0];
	p[1] = p[0];
	p[0] = p[1];
}
inline void revertPacket(DataBuffer* pPacket)
{
	swap2Bytes(pPacket->Buffer + 1);
	swap2Bytes(pPacket->Buffer + 4);
	swap2Bytes(pPacket->Buffer + 6);
	swap2Bytes(pPacket->Buffer + 8);
	swap2Bytes(pPacket->Buffer + 10);
}


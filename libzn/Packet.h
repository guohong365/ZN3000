#pragma once

#pragma pack(1)

struct Packet
{
	BYTE Header;                     //֡ͷ 0xCA
    UINT16 FrameCount;               //֡����(��˸�ʽ)
    BYTE Mode;                       //����ģʽ0--��� 1--У׼
    INT16 Feedback;                  //������ѹֵ��ԭʼ���ݣ���˸�ʽ��
    INT16 Admittance;                //����ͨ�����ݣ�ԭʼ���ݣ���˸�ʽ��
    INT16 Differential;              //΢��ͨ�����ݣ�ԭʼ���ݣ���˸�ʽ��
    INT16 ECG;                       //�ĵ�ͨ�����ݣ�ԭʼ���ݣ���˸�ʽ��
    BYTE CheckSum;                   //��У��
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
	BYTE Header;                   //֡ͷ0xCB
    BYTE Mode;                   //0---���   1---У׼
    UINT16 Reserved;               //����
    BYTE CheckSum;               //��У��
    BYTE Tail;                   //֡β0xF1
};

#define CHECK_COMMAND {0xCB, 1, 0, 0xCC, 0xF1}
#define EXAMINE_COMMAND {0xCB, 0, 0, 0xCB, 0xF1}

union CommandBuffer
{
	BYTE Buffer[sizeof(CommandPacket)];
	CommandPacket Paket;
};
#pragma pack()


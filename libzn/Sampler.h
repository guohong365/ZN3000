#pragma once
#include "SignalBuffer.h"
#include <vector>
#include <map>

/**�����߳���
 *
 *�����ȡ���ݷ���Ԥ��׼���õĻ�����
 */
class Sampler 
{
public:
	enum SamplerState
	{
		STOPPED,
		RUNNING,
		PAUSED
	};
	virtual ~Sampler(){}
	/**��ʼ�����߳�
	 *
	 *@return true - �ɹ��� false ��ʼ������������߳�ʧ��
	 */
	virtual bool begin()=0;
	/**��ͣ����
	 *
	 *��������߳��Ѿ�������ͣ״̬���߳���ֹͣ����÷������ı�����״̬
	 */
	virtual void pause()=0;
	/**�ظ���ͣ�Ĳ����߳�
	 *
	 *��������߳��Ѿ���������״̬���߳���ֹͣ����÷������ı�����״̬
	 */
	virtual void resume()=0;
	/**�˳������߳�
	 *
	 *�˳�������Ҫ�����ֳ�����ȫ�������߳�ʵ�ʽ��������ͺ����˳���������ʱ�̡�
	 *quit�����Ѿ����ã��Ե�ǰ����������κβ������ǲ��ɿ��ģ��������ܻ���ɳ��������
	 */
	virtual void quit()=0;
	/**�ȴ������߳̽���
	 *
	 *@param timeout ��ʱʱ�䡣�ò������Ƶȴ�ʱ�䡣���С��0����ʾ���޵ȴ���
	 *join����һ�����ã������߳̽�һֱ�����������߳��˳����ߵȴ�ʱ�䳬��timeoutָ���ĳ�ʱʱ�䡣
	 */
	virtual unsigned long join(int timeout)=0;
	/**��ȡ�����߳�����״̬
	 *@see SapmlerState
	 */
	virtual SamplerState getState()=0;
	/**������л�����
	 *
	 */
	virtual void resetAll()=0;

	/**��ָ��ͨ������ͨ������������
	 *
	 *@param channel ͨ�����������ڵ���0��С��ͨ������getChannelCount������ȡ����
	 *@param pBuffer Ԥ�ȷ�������ݻ����������ڴ洢channelָ��������ͨ��������
	 *@remark ��ͨ������ȡֵ�����Ϸ���Χʱ���÷���ֱ�ӷ��أ������κ���������
	 *δ���ӻ�������ͨ������������ݽ�������
	 */
	virtual void attach(int channel, SignalBuffer<float> * pBuffer)=0;
	/**��ȡָ��ͨ������������
	 *
	 *@param channel ͨ�����������ڵ���0��С��ͨ������getChannelCount������ȡ����
	 *@return ��ǰchannelָ����ͨ������������ָ�롣ͨ��δ���ӻ�������ͨ������
	 *channelȡֵ�����Ϸ���Χʱ����nullptr
	 */
	virtual SignalBuffer<float> * get(int channel) =0;
	/**��ȡͨ������
	 *
	 *@return ͨ���ܸ�������ֵͨ����ʵ���๹�캯��ָ���������в�֧���޸�
	 */
	virtual SIZE_T getCount() const=0;
    /**���ù���ģʽ
     *
     *@param mode ����ģʽ
     */
    virtual void setMode(int mode)=0;
};



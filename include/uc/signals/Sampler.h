#pragma once
#include <uc/signals/SignalBuffer.h>

namespace uc
{
    namespace signals
    {
        /**采样线程类
        *
        *负责读取数据放入预先准备好的缓冲区
        */
        class LIB_CORE_API Sampler 
        {
        public:
            enum SamplerState
            {
                STOPPED,
                RUNNING,
                PAUSED
            };
            virtual ~Sampler(){}
            /**开始采样线程
            *
            *@return true - 成功， false 初始化错误或启动线程失败
            */
            virtual bool begin()=0;
            /**暂停采样
            *
            *如果采样线程已经处于暂停状态或线程已停止，则该方法不改变现有状态
            */
            virtual void pause()=0;
            /**回复暂停的采样线程
            *
            *如果采样线程已经处于运行状态或线程已停止，则该方法不改变现有状态
            */
            virtual void resume()=0;
            /**退出采样线程
            *
            *退出过程需要清理现场并安全结束，线程实际结束可能滞后于退出方法调用时刻。
            *quit方法已经调用，对当前采样对象的任何操作都是不可靠的，甚至可能会造成程序崩溃。
            */
            virtual void quit()=0;
            /**等待采样线程结束
            *
            *@param timeout 超时时间。该参数控制等待时间。如果小于0，表示无限等待。
            *join方法一经调用，调用线程将一直阻塞至采样线程退出或者等待时间超过timeout指定的超时时间。
            */
            virtual unsigned long join(int timeout)=0;
            /**获取采样线程运行状态
            *@see SapmlerState
            */
            virtual SamplerState getState()=0;
            /**清空所有缓冲区
            *
            */
            virtual void resetAll()=0;

            /**对指定通道附加通道缓冲区对象
            *
            *@param channel 通道索引，大于等于0，小于通道数（getChannelCount方法获取）。
            *@param pBuffer 预先分配的数据缓冲区，用于存储channel指定的数据通道的数据
            *@remark 当通道索引取值超出合法范围时，该方法直接返回，不做任何其他处理。
            *未附加缓冲区的通道，其采样数据将被抛弃
            */
            virtual void attach(int channel, SignalBuffer<double> * pBuffer)=0;
            /**获取指定通道缓冲区对象
            *
            *@param channel 通道索引，大于等于0，小于通道数（getChannelCount方法获取）。
            *@return 当前channel指定的通道缓冲区对象指针。通道未附加缓冲区或通道索引
            *channel取值超出合法范围时返回nullptr
            */
            virtual SignalBuffer<double> * get(int channel) =0;
            /**获取通道个数
            *
            *@return 通道总个数。该值通常由实现类构造函数指定。运行中不支持修改
            */
            virtual size_t getCount() const=0;
            /**设置工作模式
            *
            *@param mode 工作模式
            */
            virtual void setMode(int mode)=0;
        };

    }
}


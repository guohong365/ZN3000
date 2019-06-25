#pragma once
#include <uc/libuc.h>
#include <uc/dsp/Sampler.h>

namespace uc
{
    namespace dsp
    {
        class LIB_UC_API AbstractSampler : public Sampler
        {
        public:
            explicit AbstractSampler(size_t channelCount);
            ~AbstractSampler();
            virtual bool begin();
            virtual void pause();
            virtual void resume();
            virtual void quit();
            virtual unsigned long join(int timeout);
            virtual SamplerState getState();
            virtual void resetAll();
            virtual void attach(int channel, SignalBuffer<double>* pBuffer);
            virtual SignalBuffer<double>* get(int channel) ;
            virtual size_t getCount() const;
            virtual void setMode(int mode);
        protected:
            virtual bool onBeforeStart(){ return true; }
            virtual void onBeforeLoop(){}
            virtual void onResume(){}
            virtual void doSampler()=0;
            /**�����߳�
            *
            *@param lpParam �������Ϊthisָ��
            *@return 0 - ����ֹͣ >1000 ����
            */
            static unsigned long __stdcall samplerFunc(void* lpParam);
        private:
            void* _hThread;
            void* _hResume;
            unsigned long _dwThreadId;
            SignalBuffer<double> **_pBuffers;
            size_t _channelCount;
            bool _quit;
            bool _paused;
        };

    }
}
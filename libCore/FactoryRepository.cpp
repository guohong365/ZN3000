#include "stdafx.h"
#include <uc/signals/RecordReaderFactory.h>
#include <uc/signals/RecordWriterFactory.h>
#include "FactoryRepository.h"
#include <algorithm>

namespace uc
{
    namespace signals
    {
        template <class _PtrType>
        class FactoryDeleter
        {             
        public:
            void operator()(std::pair<TSTRING, _PtrType> item){
                delete item.second;
            }
        };
        class FactoryRepositoryImpl : public FactoryRepository
        {
            typedef std::map<TSTRING, RecordReaderFactory*> READER_FACTORIES;
            typedef std::map<TSTRING, RecordWriterFactory*> WRITER_FACTORIES;
        public:
            FactoryRepositoryImpl();
            virtual ~FactoryRepositoryImpl();
            virtual void registerReader(const TSTRING& name, RecordReaderFactory* pFactory);
            virtual void registerWriter(const TSTRING& name, RecordWriterFactory* pFactory);
            virtual RecordReader* getReader(const TSTRING& name);
            virtual RecordWriter* getWriter(const TSTRING& name);
        private:
            READER_FACTORIES _readerFactories;
            WRITER_FACTORIES _writerFactories;
        };

        FactoryRepositoryImpl::FactoryRepositoryImpl()
        {
        }


        FactoryRepositoryImpl::~FactoryRepositoryImpl()
        {
            READER_FACTORIES::iterator readIt=_readerFactories.begin();
            std::for_each(_readerFactories.begin(), _readerFactories.end(), FactoryDeleter<RecordReaderFactory*>());
            std::for_each(_writerFactories.begin(), _writerFactories.end(), FactoryDeleter<RecordWriterFactory*>());
        }


        void FactoryRepositoryImpl::registerReader(const TSTRING& name, RecordReaderFactory* pFactory)
        {
            _readerFactories[name] = pFactory;
        }

        void FactoryRepositoryImpl::registerWriter(const TSTRING& name, RecordWriterFactory* pFactory)
        {
            _writerFactories[name] = pFactory;
        }

        RecordReader* FactoryRepositoryImpl::getReader(const TSTRING& name)
        {
            READER_FACTORIES::iterator iterator = _readerFactories.find(name);
            if (iterator != _readerFactories.end())
            {
                return iterator->second->create();
            }
            return nullptr;
        }

        RecordWriter* FactoryRepositoryImpl::getWriter(const TSTRING& name)
        {
            WRITER_FACTORIES::iterator iterator= _writerFactories.find(name);
            if(iterator !=_writerFactories.end())
            {
                return iterator->second->create();
            }
            return nullptr;
        }
        class InstanceHolder
        {
            static InstanceHolder  holder;
            FactoryRepository * _ptr;
        public:
            InstanceHolder()
            {
                _ptr = new FactoryRepositoryImpl();
            }
            static FactoryRepository& get()
            {
                return *(holder._ptr);
            }
            ~InstanceHolder()
            {
                delete _ptr;
            }
        };
        InstanceHolder InstanceHolder::holder;

        FactoryRepository& FactoryRepository::instance()
        {
            return InstanceHolder::get();
        }

    }
}

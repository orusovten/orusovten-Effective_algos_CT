#ifndef MEMORY_MANAGER_HEAD_H_2022_02_17
#define MEMORY_MANAGER_HEAD_H_2022_02_17

namespace lab618
{
    template <class T>
    class CMemoryManager
    {
    private:
        struct block
        {
            // Массив данных блока
            T* pdata;
            // Адрес следующего блока
            block* pnext;
            // Первая свободная ячейка
            int firstFreeIndex;
            // Число заполненных ячеек
            int usedCount;
        };
    public:
        class CException
        {
        public:
            CException()
            {
            }
        };

    public:
        CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false):
            m_blkSize(_default_block_size), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct),
            m_pBlocks(0), m_pCurrentBlk(0)
        {
        }

        virtual ~CMemoryManager()
        {
            clear();
        }

        // Получить адрес нового элемента из менеджера
        T* newObject()
        {
            if (!m_pBlocks) 
            {
                m_pBlocks = newBlock();
                m_pCurrentBlk = m_pBlocks;
            }
            if (m_pCurrentBlk->usedCount < m_blkSize) 
            {
                T* p = m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex;
                memset(reinterpret_cast<void*>(p), 0, sizeof(T));
                ::new(reinterpret_cast<void*>(p)) T;
                m_pCurrentBlk->usedCount += 1;
                // что делать с firstFreeIndex   
            }
        }

        // Освободить элемент в менеджере
        bool deleteObject(T* p)
        {
            block* tmpBlock = m_pBlocks;
            while (tmpBlock->pnext) 
            {
                if (p - tmpBlock->pdata < sizeof(T) * m_blkSize) 
                {
                    p->~T();
                    memset(reinterpret_cast<void*>(p), 0, sizeof(T));
                    // обновление firstFreeIndex и m_pCurrentBlk???
                    return true;
                }
                tmpBlock = tmpBlock->pnext;
            }
            return false;
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear()
        {
            block* tmpBlock = m_pBlocks;
            if (!m_isDeleteElementsOnDestruct) 
            {
                while (tmpBlock->pnext) 
                {
                    if (tmpBlock->usedCount > 0) 
                    {
                        throw CException();
                    }
                    tmpBlock = tmpBlock->pnext;
                }
            }
            else 
            {
                while (tmpBlock->pnext) 
                {
                    deleteBlock(tmpBlock);
                    tmpBlock = tmpBlock->pnext;
                }
            }
        }
    private:

        // Создать новый блок данных. применяется в newObject
        block* newBlock()
        {
            char* pcd = new char[m_blkSize * sizeof(T)];
            block* newBlock = new block();
            newBlock->pdata = reinterpret_cast<T*>(pcd);
            return newBlock;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block *p)
        {
            // удаляется массив данных, а что делать с int-ами???
            char* pd = reinterpret_cast<char*>(p->pdata);
            delete[] pd;
        }

        // Размер блока
        int m_blkSize;
        // Начало списка блоков
        block* m_pBlocks;
        // Текущий блок
        block *m_pCurrentBlk;
        // Удалять ли элементы при освобождении
        bool m_isDeleteElementsOnDestruct;
    };
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2022_02_17

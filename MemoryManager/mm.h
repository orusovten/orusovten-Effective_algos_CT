#ifndef MEMORY_MANAGER_HEAD_H_2022_02_17
#define MEMORY_MANAGER_HEAD_H_2022_02_17
#include <cstring>

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
            block* tmp = m_pCurrentBlk;
            if (tmp->usedCount == m_blkSize) 
            {
                tmp = m_pBlocks;
                while (tmp->pnext) 
                {
                    if (tmp->usedCount < m_blkSize) 
                    {
                        break;
                    }
                    tmp = tmp->pnext;
                }
                // Заходим в этот if только в случае, когда все блоки заполнены
                if (tmp->usedCount == m_blkSize) 
                {
                    block* m_newBlock = newBlock();
                    tmp->pnext = m_newBlock;
                    tmp = m_newBlock;
                }
                m_pCurrentBlk = tmp;
            }
            T* p = m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex;
            int* pi = reinterpret_cast<int*>(p);
            m_pCurrentBlk->firstFreeIndex = *pi;
            memset(reinterpret_cast<void*>(p), 0, sizeof(T));
            ::new(reinterpret_cast<void*>(p)) T;
            m_pCurrentBlk->usedCount += 1;
            return p;
        }

        // Освободить элемент в менеджере
        bool deleteObject(T* p)
        {
            block* tmpBlock = m_pBlocks;
            while (tmpBlock) 
            {
                int diff = p - tmpBlock->pdata;
                if (0 <= diff && diff < m_blkSize) 
                {
                    p->~T();
                    memset(reinterpret_cast<void*>(p), 0, sizeof(T));
                    int* pi = reinterpret_cast<int*>(p);
                    *pi = tmpBlock->firstFreeIndex;
                    tmpBlock->firstFreeIndex = diff;
                    tmpBlock->usedCount -= 1;
                    return true;
                }
                tmpBlock = tmpBlock->pnext;
            }
            return false;
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear()
        {
            if (!m_pBlocks) {
                return;
            }
            block* tmpBlock = m_pBlocks;
            if (!m_isDeleteElementsOnDestruct) 
            {
                while (tmpBlock) 
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
                while (tmpBlock) 
                {
                    block* next = tmpBlock->pnext;
                    if (tmpBlock->usedCount > 0) 
                    {
                        deleteBlock(tmpBlock);
                    }
                    tmpBlock = next;
                }
            }
            m_pBlocks = nullptr;
            m_pCurrentBlk = nullptr;
        }
    private:

        // Создать новый блок данных. применяется в newObject
        block* newBlock()
        {
            char* pcd = new char[m_blkSize * sizeof(T)];
            block* m_newBlock = new block();
            m_newBlock->pdata = reinterpret_cast<T*>(pcd);
            T* pp = m_newBlock->pdata;
            for (int i = 0; i < m_blkSize - 1; ++i) 
            {
                int* pi = reinterpret_cast<int*>(pp + i);
                *pi = i + 1;
            }
            int* pi = reinterpret_cast<int*>(pp + m_blkSize - 1);
            *pi = -1;
            return m_newBlock;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block *p)
        {
            // удаляется массив данных, а что делать с int-ами???
            char* pd = reinterpret_cast<char*>(p->pdata);
            delete[] pd;
            delete p;
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

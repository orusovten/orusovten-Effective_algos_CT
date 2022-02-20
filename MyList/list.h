#ifndef TEMPLATES_LIST_2022_02_03
#define TEMPLATES_LIST_2022_02_03

namespace lab618
{
    template<class T>
    class CSingleLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf* pnext;
            leaf(T& _data, leaf* _pnext):
                    data(_data), pnext(_pnext)
            {
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator()
                    : m_pCurrent(0), m_pBegin(0)
            {
            }

            CIterator(leaf* p)
                    : m_pCurrent(p), m_pBegin(0)
            {
            }

            CIterator(const CIterator& src)
                    : m_pCurrent(src.m_pCurrent), m_pBegin(src.m_pBegin)
            {
            }

            ~CIterator()
            {
                m_pBegin = nullptr;
                m_pCurrent = nullptr;
            }

            CIterator& operator = (const CIterator& src)
            {
                m_pCurrent = src.m_pCurrent;
                m_pBegin = src.m_pBegin;
                return *this;
            }

            bool operator != (const CIterator& it) const
            {
                return (it.m_pCurrent != m_pCurrent) || (it.m_pBegin != m_pBegin);
            }

            void operator++()
            {
                if (!m_pBegin) {
                    m_pCurrent = m_pCurrent->pnext;
                } 
                else {
                    m_pCurrent = m_pBegin;
                    m_pBegin = nullptr;
                }
            }

            T& getData()
            {
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return m_pCurrent->data;
            }

            leaf* getLeaf()
            {
                return m_pCurrent;
            }

            void setLeaf(leaf* p)
            {
                m_pCurrent = p;
                m_pBegin = nullptr;
            }

            void setLeafPreBegin(leaf* p)
            {
                m_pCurrent = nullptr;
                m_pBegin = p;
            }

            bool isValid() {
                return m_pCurrent;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
        };

    public:

        CSingleLinkedList()
                : m_pBegin(0), m_pEnd(0)
        {
        }

        virtual ~CSingleLinkedList()
        {
            clear();
        }

        void pushBack(T& data)
        {
            leaf* newLeaf = new leaf(data, nullptr);
            if (!m_pBegin) {
                m_pBegin = newLeaf;
                m_pEnd = newLeaf;
                return;
            }
            m_pEnd->pnext = newLeaf;
            m_pEnd = newLeaf;
        }

        void pushFront(T& data)
        {
            leaf* newLeaf = new leaf(data, nullptr);
            if (!m_pBegin) {
                m_pBegin = newLeaf;
                m_pEnd = newLeaf;
                return;
            }
            leaf* tmp = m_pBegin;
            m_pBegin = newLeaf;
            m_pBegin->pnext = tmp;
        }

        T popFront()
        {
            T value = m_pBegin->data;
            if (m_pEnd == m_pBegin) {
                m_pEnd = nullptr;
            }
            leaf* pnext = m_pBegin->pnext;
            delete m_pBegin;
            m_pBegin = pnext;
            return value;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            leaf* it_leaf = it.getLeaf();
            if (it_leaf == m_pBegin) {
                it.setLeafPreBegin(it_leaf->pnext);
                m_pBegin = m_pBegin->pnext;
            }
            else {
                leaf* tmp = m_pBegin;
                while (tmp->pnext != it_leaf) {
                    tmp = tmp->pnext;
                }
                tmp->pnext = it_leaf->pnext;
                it.setLeaf(tmp);
                if (it_leaf == m_pEnd) {
                    m_pEnd = tmp;
                }
            }
            delete it_leaf;
            if (!m_pBegin) {
                m_pEnd = nullptr;
            }
        }

        int getSize()
        {
            if (!m_pBegin) {
                return 0;
            }
            int size = 1;
            leaf* tmp = m_pBegin;
            while (tmp != m_pEnd) {
                ++size;
                tmp = tmp->pnext;
            }
            return size;
        }

        void clear()
        {
            if (!m_pBegin) {
                return;
            }
            leaf* tmp = m_pBegin;
            while (tmp != m_pEnd) {
                leaf* pnext = tmp->pnext;
                delete tmp;
                tmp = pnext;
            }
            delete tmp;
            m_pBegin = nullptr;
            m_pEnd = nullptr;
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

    private:
        leaf* m_pBegin;
        leaf* m_pEnd;
    };

    template<class T>
    class CDualLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext, *pprev;
            leaf(T& _data, leaf* _pprev, leaf* _pnext):
                data(_data), pprev(_pprev), pnext(_pnext)
            {
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator()
                    : m_pCurrent(0), m_pBegin(0), m_pEnd(0)
            {
            }

            CIterator(leaf* p)
                    : m_pCurrent(p), m_pBegin(0), m_pEnd(0)
            {
            }

            CIterator(const CIterator& src)
                    : m_pCurrent(src.m_pCurrent), m_pBegin(src.m_pBegin), m_pEnd(src.m_pEnd)
            {
            }

            ~CIterator()
            {
                m_pBegin = nullptr;
                m_pCurrent = nullptr;
                m_pEnd = nullptr;
            }

            CIterator& operator = (const CIterator& src)
            {
                m_pCurrent = src.m_pCurrent;
                m_pBegin = src.m_pBegin;
                m_pEnd = src.m_pEnd;
                return *this;
            }

            bool operator != (const CIterator& it) const
            {
                return (it.m_pCurrent != m_pCurrent) || (it.m_pBegin != m_pBegin) || (it.m_pEnd != m_pEnd);
            }

            void operator++()
            {
                if (!m_pBegin) {
                    m_pCurrent = m_pCurrent->pnext;
                } 
                else {
                    m_pCurrent = m_pBegin;
                    m_pBegin = nullptr;
                }
            }

            void operator--()
            {
                if (!m_pEnd) {
                    m_pCurrent = m_pCurrent->pprev;
                } 
                else {
                    m_pCurrent = m_pEnd;
                    m_pEnd = nullptr;
                }
            }

            T& getData()
            {
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return m_pCurrent->data;
            }

            leaf* getLeaf()
            {
                return m_pCurrent;
            }

            // применяется в erase и eraseAndNext
            void setLeaf(leaf* p)
            {
                 m_pCurrent = p;
                 m_pBegin = nullptr;
            }

            // применяется в erase и eraseAndNext
            void setLeafPreBegin(leaf* p)
            {
                m_pBegin = p;
                m_pCurrent = nullptr;
                m_pEnd = nullptr;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf* p)
            {
                m_pEnd = p;
                m_pCurrent = nullptr;
                m_pBegin = nullptr;
            }

            bool isValid() {
                return m_pCurrent;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
            //храним конец списка, если мы находимся после конца
            leaf* m_pEnd;
        };

    public:

        CDualLinkedList()
        : m_pBegin(0), m_pEnd(0)
        {
        };

        virtual ~CDualLinkedList()
        {
            clear();
        };

        void pushBack(T& data)
        {
            leaf* newLeaf = new leaf(data, nullptr, nullptr);
            if (!m_pBegin) {
                m_pBegin = newLeaf;
                m_pEnd = newLeaf;
                return;
            }
            m_pEnd->pnext = newLeaf;
            newLeaf->pprev = m_pEnd;
            m_pEnd = newLeaf;
        }

        T popBack()
        {
            T value = m_pEnd->data;
            if (m_pEnd == m_pBegin) {
                m_pBegin = nullptr;
            }
            leaf* pprev = m_pEnd->pprev;
            delete m_pEnd;
            m_pEnd = pprev;
            if (m_pEnd) {
                m_pEnd->pnext = nullptr;
            }
            return value;
        }

        void pushFront(T& data)
        {
            leaf* newLeaf = new leaf(data, nullptr, nullptr);
            if (!m_pBegin) {
                m_pBegin = newLeaf;
                m_pEnd = newLeaf;
                return;
            }
            m_pBegin->pprev = newLeaf;
            newLeaf->pnext = m_pBegin;
            m_pBegin = newLeaf;
        }

        T popFront()
        {
            T value = m_pBegin->data;
            if (m_pEnd == m_pBegin) {
                m_pEnd = nullptr;
            }
            leaf* pnext = m_pBegin->pnext;
            delete m_pBegin;
            m_pBegin = pnext;
            if (m_pBegin) {
                m_pBegin->pprev = nullptr;
            }
            return value;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            leaf* it_leaf = it.getLeaf();
            if (it_leaf == m_pBegin) {
                it.setLeafPreBegin(it_leaf->pnext);
                m_pBegin = m_pBegin->pnext;
            }
            else {
                leaf* tmp = it_leaf->pprev;
                it.setLeaf(tmp);
                if (it_leaf == m_pEnd) {
                    tmp->pnext = nullptr;
                    m_pEnd = m_pEnd->pprev;
                }
                else {
                    tmp->pnext = it_leaf->pnext;
                    (it_leaf->pnext)->pprev = tmp;
                }
            }
            delete it_leaf;
            if (!m_pBegin || !m_pEnd) {
                m_pBegin = nullptr;
                m_pEnd = nullptr;
            }
        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator& it)
        {
            leaf* it_leaf = it.getLeaf();
            if (it_leaf == m_pBegin) {
                it.setLeafPreBegin(it_leaf->pnext);
                m_pBegin = m_pBegin->pnext;
            }
            else if (it_leaf == m_pEnd) {
                it.setLeafPostEnd(it_leaf->pprev);
                m_pEnd = m_pEnd->pprev;
            }
            else {
                leaf* tmp = it_leaf->pnext;
                tmp->pprev = it_leaf->pprev;
                (it_leaf->pprev)->pnext = tmp;
                it.setLeaf(tmp);
            }
            delete it_leaf;
            if (!m_pBegin || !m_pEnd) {
                m_pBegin = nullptr;
                m_pEnd = nullptr;
            }
        }

        int getSize()
        {
            if (!m_pBegin) {
                return 0;
            }
            int size = 1;
            leaf* tmp = m_pBegin;
            while (tmp != m_pEnd) {
                ++size;
                tmp = tmp->pnext;
            }
            return size;
        }

        void clear()
        {
            if (!m_pBegin) {
                return;
            }
            leaf* tmp = m_pBegin;
            while (tmp != m_pEnd) {
                leaf* pnext = tmp->pnext;
                delete tmp;
                tmp = pnext;
            }
            delete tmp;
            m_pBegin = nullptr;
            m_pEnd = nullptr;
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

        CIterator end()
        {
            return CIterator(m_pEnd);
        }

    private:
        leaf* m_pBegin;
        leaf* m_pEnd;
    };
};
#endif //#ifndef TEMPLATES_LIST_2022_02_03

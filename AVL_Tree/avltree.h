#ifndef AVL_HEAD_H_2022_03_24
#define AVL_HEAD_H_2022_03_24

#include "mm.h"

using namespace lab618;


#include <iostream>

namespace templates
{
    template <class T, int(*Compare)(const T *pElement, const T* pElement2) >
    class CAVLTree
    {
    private:
        struct leaf
        {
            T* pData;
            leaf* pLeft;
            leaf* pRight;
            int balanceFactor;
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
        CAVLTree(): m_pRoot(nullptr), m_Memory(5, true)
        {
        }

        virtual ~CAVLTree()
        {
            clear();
        }

        bool add(T* pElement)
        {
            if (findLeaf(pElement)) {
                return false;
            }
            add_in(m_pRoot, pElement);
            return true;
        }

        bool update(T* pElement)
        {
            leaf* tmpLeaf = findLeaf(pElement);
            if (!tmpLeaf) {
                add_in(m_pRoot, pElement);
                return false;
            }
            tmpLeaf->pData = pElement;
            return true;
        }
      
        T* find(const T& pElement)
        {
            leaf* tmp = findLeaf(&pElement);
            return tmp ? tmp->pData : nullptr;
        }

        bool remove(const T& element)
        {
            bool is_inside = false;
            remove_in(m_pRoot, &element, &is_inside);
            return is_inside;
        }

        void clear()
        {
            do_null(m_pRoot);
            m_Memory.clear();
        }

        void print() {
            std::cout << "tree start" << std::endl;
            print_Tree(m_pRoot, 0);
            std::cout << "tree end" << std::endl;
        }    

    private:

        void print_Tree(leaf* p,int level)
        {
            if (p)
            {
                print_Tree(p->pRight,level + 1);
                for (int i = 0; i < level; ++i) std::cout << "     ";
                std::cout << *(p->pData) << "(" << p->balanceFactor << ")" << std::endl;         
                print_Tree(p->pLeft,level + 1);
            }
        }

        void do_null(leaf*& p) {
            if (!p) 
            {
                return;
            }
            do_null(p->pLeft);
            do_null(p->pRight);
            p = nullptr;
        }
        
        int max(int a, int b) {
            return a < b ? b : a;
        }

        leaf* small_left_rotation(leaf* p)
        {
            leaf* q = p->pRight;
            if (!q)
            {
                return p;
            }
            p->pRight = q->pLeft;
            q->pLeft = p;
            int k = p->balanceFactor;
            int m = q->balanceFactor;
            if (m >= 0) 
            {
                p->balanceFactor = k - 1 - m;
                q->balanceFactor = k - 1 - max(1, k - m);
            }
            else 
            {
                p->balanceFactor = k - 1;
                q->balanceFactor = m + k - 1 - max(1, k);
            }
            return q;
        }

        leaf* small_right_rotation(leaf* p)
        {
            leaf* q = p->pLeft;
            if (!q)
            {
                return p;
            }
            p->pLeft = q->pRight;
            q->pRight = p;
            int k = p->balanceFactor;
            int m = q->balanceFactor;
            if (m <= 0) 
            {
                p->balanceFactor = k + 1 - m;
                q->balanceFactor = max(1, m - k) + 1 + k;
            } 
            else 
            {           
                p->balanceFactor = k + 1;
                q->balanceFactor = max(1, -k) + 1 + k + m;
            }
            return q;
        }

        leaf* balance(leaf* p)
        {
            if (!p)
            {
                return p;
            }
            if (p->balanceFactor == 2)
            {
                if ((p ->pRight)->balanceFactor < 0)
                {
                    p->pRight = small_right_rotation(p->pRight);
                }
                return small_left_rotation(p);
            }
            if (p->balanceFactor == -2)
            {
                if ((p->pLeft)->balanceFactor > 0)
                {
                    p->pLeft = small_left_rotation(p->pLeft);
                }
                return small_right_rotation(p);
            }
            return p;
        }

        bool add_in(leaf*& p, T* pElement)
        {

            if (!p)
            {
                leaf* newLeaf = m_Memory.newObject();
                newLeaf->pData = pElement;
                newLeaf->pLeft = nullptr;
                newLeaf->pRight = nullptr;
                newLeaf->balanceFactor = 0;
                p = newLeaf;
                return true;
            }
            bool do_change;
            if (Compare(pElement, p->pData) < 0)
            {
                do_change = add_in(p->pLeft, pElement);
                if (do_change) 
                {
                    p->balanceFactor -= 1;
                }
            }
            else
            {
                do_change = add_in(p->pRight, pElement);
                if (do_change) 
                {
                    p->balanceFactor += 1;
                }
            }
            p = balance(p);
            return do_change && p->balanceFactor != 0;
        }
        
        leaf* findLeaf(const T* pElement)
        {
            leaf* tmp = m_pRoot;
            while (tmp) 
            {
                int diff = Compare(pElement, tmp->pData);
                if (diff == 0) 
                {
                    return tmp;
                }
                else if (diff < 0) 
                {
                    tmp = tmp->pLeft;
                } 
                else
                {
                    tmp = tmp->pRight;
                }
            }
            return nullptr;
        }
        
        leaf* findMin(leaf* p) // поиск узла с минимальным значением в правом поддереве
        {
            while (p)
            {
                p = p->pLeft;
            }
            return p;
        }

        leaf* findMax(leaf* p) // поиск узла с максимальным значением в левом поддереве
        {
            while (p)
            {
                p = p->pRight;
            }
            return p;
        }

        leaf* removeMin(leaf* p)
        {
            if (!p->pLeft)
            {
                return p->pRight;
            }
            p->pLeft = removeMin(p->pLeft);
            return balance(p);
        }

        leaf* removeMax(leaf* p)
        {
            if (!p->pRight)
            {
                return p->pLeft;
            }
            p->pRight = removeMin(p->pRight);
            return balance(p);
        }

        bool remove_in(leaf*& p, const T* pElement, bool* is_inside)
        {
            if (!p)
            {
                return false;
            }
            int diff = Compare(pElement, p->pData);
            bool do_change;
            if (diff > 0)
            {
                do_change = remove_in(p->pRight, pElement, is_inside);
                if (do_change) {
                    p->balanceFactor -= 1;
                }
            }
            else if (diff < 0)
            {
                do_change = remove_in(p->pLeft, pElement, is_inside);
                if (do_change) {
                    p->balanceFactor += 1;
                }
            }
            else
            {
                *is_inside = true;
                leaf* r = p->pRight;
                leaf* l = p->pLeft;
                int bFactor = p->balanceFactor;
                m_Memory.deleteObject(p);
                if (!r)
                {
                    p = l;
                }
                else if (!l)
                {
                    p = r;
                }
                else if (bFactor >= 0)
                {
                    p = findMin(r);
                    p->pRight = removeMin(r);
                    p->pLeft = l;
                }
                else
                {
                    p = findMax(l);
                    p->pLeft = removeMax(l);
                    p->pRight = r;
                }
                p = balance(p);
                return true;
            }
            p = balance(p);
            return do_change && p->balanceFactor == 0;
        }

        leaf* m_pRoot;
        CMemoryManager<leaf> m_Memory;
    };

}; // namespace templates

#endif // #define AVL_HEAD_H_2015_03_31

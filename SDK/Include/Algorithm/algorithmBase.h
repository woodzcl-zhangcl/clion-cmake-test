// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/2.
//

#ifndef SDK_ALGORITHMBASE_H
#define SDK_ALGORITHMBASE_H

#include<iostream>
#include<cassert>

#include "CMemBlock.h"

namespace Algorithm {
    // list
    template<typename T>
    struct Node {
        T _data;
        struct Node *next;

        Node() {
            next = nullptr;
        }
    };

    template<typename T>
    class LinkList {
    private:
        Node<T> *_head;

    public:
        LinkList() { _head = nullptr; }

        ~LinkList();

    public:
        void clear();

        bool isEmpty() { return _head == nullptr; }

        size_t length();

        bool getElem(size_t i, T &e);

        size_t locateElem(T e);

        bool priorElem(T cur_e, T &pre_e);

        bool nextElem(T cur_e, T &next_e);

        bool insertElem(size_t i, T e);

        bool delElem(size_t i, T &e);

        Node<T> *reverse();
    };

    // clear memory
    template<typename T>
    LinkList<T>::~LinkList() {
        Node<T> *p = nullptr;
        while (_head) {
            p = _head;
            _head = _head->next;
            delete (p);
        }
    }

    // same as destructor
    template<typename T>
    void LinkList<T>::clear() {
        Node<T> *p = nullptr;
        while (_head) {
            p = _head;
            _head = _head->next;
            delete (p);
        }
    }

    // get length of list
    template<typename T>
    size_t LinkList<T>::length() {
        Node<T> *p = _head;
        size_t len = 0;
        while (nullptr != p) {
            len++;
            p = p->next;
        }
        return len;
    }

    // get element index i
    template<typename T>
    bool LinkList<T>::getElem(size_t i, T &e) {
        Node<T> *p = _head;
        size_t j = 0;
        while (p && j < i) {
            p = p->next;
            j++;
        }
        if (nullptr == p) return false;
        e = p->_data;
        return true;
    }

    // locate element
    template<typename T>
    size_t LinkList<T>::locateElem(T e) {
        size_t i = 0;
        Node<T> *p = _head;
        while (nullptr != p) {
            if (p->_data == e)
                return i;
            else p = p->next;
            i++;
        }
        std::cout << "has no item in list" << std::endl;
        return SIZE_MAX;
    }

    // get prior element
    template<typename T>
    bool LinkList<T>::priorElem(T cur_e, T &pre_e) {
        Node<T> *p = _head;
        //it is head that has not prior element
        if (p->_data == cur_e) return false;
        while (nullptr != p->next) {
            if (p->next->_data == cur_e) {
                pre_e = p->_data;
                return true;
            } else
                p = p->next;
        }
        // only head or not exist
        return false;
    }

    // get next element
    template<typename T>
    bool LinkList<T>::nextElem(T cur_e, T &next_e) {
        Node<T> *p = _head;
        if (nullptr == _head || nullptr == _head->next) return false;
        while (nullptr != p->next) {
            if (p->_data == cur_e) {
                next_e = p->next->_data;
                return true;
            } else
                p = p->next;
        }
        return false;
    }

    // insert element at index i
    template<typename T>
    bool LinkList<T>::insertElem(size_t i, T e) {
        Node<T> *p = _head, *s;
        size_t j = 0;
        if (0 == i) {
            s = new Node<T>();
            s->_data = e;
            s->next = p;
            _head = s;
            return true;
        }
        while (p && j < i - 1) {
            p = p->next;
            j++;
        }
        // come to tail
        if (nullptr == p)
            return false;
        s = new Node<T>();
        s->_data = e;
        s->next = p->next;
        p->next = s;
        return true;
    }

    // delete element at index i, return data in this element
    template<typename T>
    bool LinkList<T>::delElem(size_t i, T &e) {
        Node<T> *p = _head, *s;
        if (nullptr == p) return false;
        size_t j = 0;
        if (0 == i) {
            _head = _head->next;
            e = p->_data;
            delete p;
            p = nullptr;
            return true;
        }
        while (p && j < i - 1) {
            j++;
            p = p->next;
        }
        if (nullptr == p)
            return false;
        s = p->next;
        p->next = p->next->next;
        e = s->_data;
        delete s;
        s = nullptr;
        return true;
    }

    template<typename T>
    Node<T> *LinkList<T>::reverse() {
        if (nullptr == _head || nullptr == _head->next) return _head;
        Node<T> *p = _head, *q = _head->next, *r;
        _head->next = nullptr;
        while (q) {
            r = q->next;
            q->next = p;
            p = q;
            q = r;
        }
        _head = p;
        return _head;
    }

    //queue for loop
    template<typename T>
    class Queue {
    public:
        Queue(int maxsize = 10);

        ~Queue();

    public:
        bool empty() const;

        bool isFull() const;

        size_t size() const;

        void push(const T &data);

        void pop();

        T &front();

        T front() const;

        T &back();

        T back() const;

    private:
        T *_array;
        ssize_t _front;
        ssize_t _rear;
        ssize_t _capacity;
    };

    template<typename T>
    Queue<T>::Queue(int maxsize) :_front(0), _rear(0), _capacity(maxsize) {
        _array = new T[maxsize];
        assert(nullptr != _array);
    }

    template<typename T>
    Queue<T>::~Queue() {
        delete[] _array;
    }

    template<typename T>
    bool Queue<T>::empty() const {
        // queue of loop, thus, it is empty when front==rear
        return _front == _rear;
    }

    template<typename T>
    bool Queue<T>::isFull() const {
        // add from rear, get from front
        return (_rear + 1) % _capacity == _front;
    }

    template<typename T>
    size_t Queue<T>::size() const {
        return (_rear - _front + _capacity) % _capacity;
    }

    template<typename T>
    void Queue<T>::push(const T &data) {
        if (!isFull()) {
            _array[_rear] = data;
            _rear = (_rear + 1) % _capacity;
        } else {
            // expend capacity
            T *newarray = new T[2 * _capacity];
            for (size_t i = 0; i < _capacity - 1; i++) {
                newarray[i] = this->front();
                this->pop();
            }
            delete[] _array;
            _array = newarray;
            _front = 0;
            _array[_capacity - 1] = data;
            _rear = _capacity;
            _capacity = 2 * _capacity;
        }
    }

    template<typename T>
    void Queue<T>::pop() {
        if (!empty()) {
            _front = (_front + 1) % _capacity;
        } else
            std::cout << "empty queue!" << std::endl;
    }

    template<typename T>
    T &Queue<T>::front() {
        if (empty())
            std::cerr << "Error, queue is empty!";
        return _array[_front];
    }

    template<typename T>
    T Queue<T>::front() const {
        if (empty())
            std::cerr << "Error, queue is empty!";
        return _array[_front];
    }

    template<typename T>
    T &Queue<T>::back() {
        if (empty())
            std::cerr << "Error, queue is empty!";
        return _array[_rear - 1];
    }

    template<typename T>
    T Queue<T>::back() const {
        if (empty())
            std::cerr << "Error, queue is empty!";
        return _array[_rear - 1];
    }

    //BinaryTree
    template<typename T>
    class BinaryTree {
    public:
        BinaryTree() = default;

        BinaryTree(const BinaryTree &bt);

        BinaryTree(const T &theElement);

        virtual ~BinaryTree();

        void insert(const T &theElement);

        void remove(const T &theElement);

        void makeEmpty();

        bool isFind(const T &theElement) const;

        void preOrder() const;

        void inOrder() const;

        void postOrder() const;

        virtual void do_withElement(const T &theElement) const;


    private:

        struct BinaryNode {
            T element;
            BinaryNode *leftNode;
            BinaryNode *rightNode;

            BinaryNode(const T &ele, BinaryNode *left, BinaryNode *right)
                    : element(ele), leftNode(left), rightNode(right) {};

            BinaryNode(const T &ele)
                    : element(ele), leftNode(nullptr), rightNode(nullptr) {};

            BinaryNode(const BinaryNode *bNode)
                    : element(bNode->element), leftNode(bNode->leftNode), rightNode(bNode->rightNode) {
            }
        };

        BinaryNode *root;

        BinaryNode *clone(const BinaryNode *bNode);

        void insert(const T &theElement, BinaryNode *&t);

        void remove(const T &theElement, BinaryNode *&t);

        void makeEmpty(BinaryNode *&t);

        bool isFind(const T &theElement, BinaryNode *t) const;

        BinaryNode *findMin(BinaryNode *bNode) const;

        BinaryNode *findMax(BinaryNode *bNode) const;

        void preOrder(BinaryNode *bNode) const;

        void inOrder(BinaryNode *bNode) const;

        void postOrder(BinaryNode *bNode) const;
    };

    template<typename T>
    BinaryTree<T>::BinaryTree(const BinaryTree &bt) {
        if (nullptr != bt.root) {
            this->root = clone(bt.root);
        } else {
            root = nullptr;
        }
    }

    template<typename T>
    BinaryTree<T>::BinaryTree(const T &theElement) {
        root = new BinaryNode(theElement);
    }

    template<typename T>
    BinaryTree<T>::~BinaryTree() {
        makeEmpty(root);
    }

    template<typename T>
    void BinaryTree<T>::insert(const T &theElement) {
        insert(theElement, root);
    }

    template<typename T>
    void BinaryTree<T>::remove(const T &theElement) {
        remove(theElement, root);
    }

    template<typename T>
    void BinaryTree<T>::makeEmpty() {
        makeEmpty(root);
    }

    template<typename T>
    bool BinaryTree<T>::isFind(const T &theElement) const {
        return isFind(theElement, root);
    }

    template<typename T>
    void BinaryTree<T>::preOrder() const {
        preOrder(root);
    }

    template<typename T>
    void BinaryTree<T>::inOrder() const {
        inOrder(root);
    }

    template<typename T>
    void BinaryTree<T>::postOrder() const {
        postOrder(root);
    }

    template<typename T>
    void BinaryTree<T>::do_withElement(const T &theElement) const{
        std::cout << theElement << std::endl;
    }

    template<typename T>
    typename BinaryTree<T>::BinaryNode *BinaryTree<T>::clone(const BinaryNode *r) {
        if (nullptr == r) {
            return nullptr;
        } else {
            return new BinaryNode(r->element, clone(r->leftNode), clone(r->rightNode));
        }
    }

    template<typename T>
    void BinaryTree<T>::insert(const T &theElement, BinaryNode *&t) {
        if (nullptr == t) {
            t = new BinaryNode(theElement);
        } else if (theElement < t->element) {
            insert(theElement, t->leftNode);
        } else if (theElement > t->element) {
            insert(theElement, t->rightNode);
        } else {//no repeat data into tree
        }
    }

    template<typename T>
    void BinaryTree<T>::remove(const T &theElement, BinaryNode *&t) {
        if (nullptr == t) {
            return;
        } else {
            if (theElement < t->element) {
                remove(theElement, t->leftNode);
            } else if (theElement > t->element) {
                remove(theElement, t->rightNode);
            } else if (nullptr != t->leftNode && nullptr != t->rightNode) {  // delete both childs

                t->element = findMin(t->rightNode)->element;
                remove(t->element, t->rightNode);
            } else {
                BinaryNode *oldNode = t;
                t = (nullptr != t->leftNode) ? t->leftNode : t->rightNode;
                delete oldNode;
            }
        }
    }

    template<typename T>
    void BinaryTree<T>::makeEmpty(BinaryNode *&t) {
        if (nullptr != t) {
            makeEmpty(t->leftNode);
            makeEmpty(t->rightNode);
            delete t;
            t = nullptr;
        }
    }

    template<typename T>
    bool BinaryTree<T>::isFind(const T &theElement, BinaryNode *t) const {
        if (nullptr == t) {
            return false;
        } else if (theElement < t->element) {
            return isFind(theElement, t->leftNode);
        } else if (theElement > t->element) {
            return isFind(theElement, t->rightNode);
        } else { // match
            return true;
        }
    }

    template<typename T>
    typename BinaryTree<T>::BinaryNode *BinaryTree<T>::findMin(BinaryNode *bNode) const {
        if (nullptr != bNode) {
            while (nullptr != bNode->leftNode) {
                bNode = bNode->leftNode;
            }
        }
        return bNode;
    }

    template<typename T>
    typename BinaryTree<T>::BinaryNode *BinaryTree<T>::findMax(BinaryNode *bNode) const {
        if (nullptr != bNode) {
            while (nullptr != bNode->rightNode) {
                bNode = bNode->rightNode;
            }
        }
        return bNode;
    }

    template<typename T>
    void BinaryTree<T>::preOrder(BinaryNode *bNode) const {
        if (nullptr != bNode) {
            do_withElement(bNode->element);
            preOrder(bNode->leftNode);
            preOrder(bNode->rightNode);
        }
    }

    template<typename T>
    void BinaryTree<T>::inOrder(BinaryNode *bNode) const {
        if (nullptr != bNode) {
            inOrder(bNode->leftNode);
            do_withElement(bNode->element);
            inOrder(bNode->rightNode);
        }
    }

    template< typename T>
    void BinaryTree<T>::postOrder( BinaryNode *bNode ) const {
        postOrder(bNode->leftNode);
        postOrder(bNode->rightNode);
        do_withElement(bNode->element);
    }
#ifdef __cplusplus
    extern "C" {
#endif

    extern uint64_t version;

#ifdef __cplusplus
    }
#endif

}

#endif //SDK_ALGORITHMBASE_H

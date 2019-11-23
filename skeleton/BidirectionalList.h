//
// Created by tsofi on 21/04/2019.
//

#ifndef WET1DATASTRUCTURES_BIDIRECTIONALLIST_H
#define WET1DATASTRUCTURES_BIDIRECTIONALLIST_H

#include <istream>

using std::ostream;

template<class T>
struct Node {
    T data;
    Node *next;
    Node *previous;
};

template<class T>
class BiDirectionalList {
private:
    Node<T> *head;
    Node<T> *tail;
    //friend ListIterator;

public:

    class ListIterator {
    private:
        BiDirectionalList *list;
        Node<T> *current;
    public:
        ListIterator(BiDirectionalList *lst, bool hOrT) : list(lst) {
            if (lst == NULL)
               return; // throw NullArgument();
            if (lst->head == NULL)
                current = NULL;
            else if (hOrT) {
                current = (lst)->head;
            } else {
                current = (lst)->tail;
            }
        }

        ListIterator &operator++() {
            if (current == NULL)
                return *this;
            current = current->next;
            return *this;
        }

        ListIterator &operator--() {
            if (current == NULL)
                return *this;
            current = current->previous;
            return *this;
        }

        T &operator*() {
            return current->data;
        }

    };

    BiDirectionalList(Node<T> *h = NULL, Node<T> *t = NULL) : head(h), tail(t) {}

    Node<T> *push(const T &data) {
        Node<T> *n = new Node<T>;
        n->data = data;
        if (head == NULL) {
            tail = n;
            head = n;
            n->next = NULL;
            n->previous = NULL;
        } else {
            head->previous=n;
            n->next = head;
            n->previous = NULL;
            head = n;
        }
        return n;

    }


    Node<T>* getHead(){
        return head;
    }

    Node<T>* getTail(){
        return tail;
    }

    Node<T> *removeNode(const Node<T> *toRemove) {

        if (toRemove == NULL) {
            return NULL; //or throw exception?
        }
        if (toRemove->next) {
            if (toRemove->previous) {
                (toRemove->next)->previous = toRemove->previous;
                (toRemove->previous)->next = toRemove->next;
                delete (toRemove);
                return head;
            } else {
                head = toRemove->next;
                head->previous = NULL;
                delete (toRemove);
                return head;
            }
        }
        if (!toRemove->next) {
            if (toRemove->previous) {
                tail = (toRemove->previous);
                tail->next = NULL;
                delete (toRemove);
                return head;
            } else { //if (!toRemove->previous)
                delete (toRemove);
                head = NULL;
                tail = NULL;
                return NULL;
            }
        }
        return head;
    }


    friend ostream &operator<<(ostream &os, BiDirectionalList<T> &list) {
        os << "direction 1: " << std::endl;
        os << "BEGIN <-> ";
        Node<T> *ptr = list.head;
        while (ptr != NULL) {
            os << ptr->data << " <-> ";
            ptr = ptr->next;
        }
        os << "END";
        return os;
    }


    ~BiDirectionalList() {
        Node<T> *currentPos = head;
        while (currentPos != NULL) {
            Node<T> *nextPos = currentPos->next;
            delete (currentPos);
            currentPos = nextPos;
        }
    }

    //I made the two following function return ListIterators, not referenecs... hope it'll do, if not- flip back to &s
    ListIterator beginForward() {
        return ListIterator(this, true);
    }

    ListIterator beginBackward() {
        return ListIterator(this, false);
    }

    int size() {
        int s = 0;
        Node<T> *ptr = head;
        while (ptr != NULL) {
            s++;
            ptr = ptr->next;
        }
        return s;
    }

};


#endif //WET1DATASTRUCTURES_BIDIRECTIONALLIST_H

#pragma once
#include <sys/types.h>

namespace cpplib
{
template <typename value_type>
class list
    {
    private:
        list <value_type> &operator = ( const list<value_type> &other ) = delete;
        class list_node
            {
            public:
                list_node *previous, *next;
                value_type data;
            };

        list_node *first, *last;
        size_t count;

    public:
        class iterator
            {
                template <typename value_type> friend class list;
            protected:
                list_node *current;
                const list <value_type> *owner;

                iterator ( const list *new_owner, list_node *new_current );
            public:
                iterator ( void );
                iterator ( const iterator &other );
                iterator &operator ++ ( void );
                iterator &operator -- ( void );
                iterator operator++ ( int );
                iterator operator-- ( int );
                bool operator == ( const iterator &other ) const;
                bool operator != ( const iterator &other ) const;
                iterator &operator = ( const iterator &other );
                value_type &operator * ( void ) const;
                value_type *operator -> ( void ) const;
                iterator operator + ( const off_t delta ) const;
                value_type &get ( void ) const;
                bool is_valid ( void ) const;
            };

        list ( void );
        ~list ( void );
        iterator add_at_end ( const value_type &new_value );
        iterator add_at_begin ( const value_type &new_value );
        iterator add_after ( iterator &reference_iterator, const value_type &new_value );
        iterator add_before ( iterator &reference_iterator, const value_type &new_value );
        iterator erase ( const iterator &reference_iterator );
        void clear ( void );

        iterator find ( const value_type &value ) const;
        iterator begin ( void ) const;
        iterator end ( void ) const;
        const value_type &front ( void ) const;
        value_type &front ( void );
        const value_type &back ( void ) const;
        value_type &back ( void );
        size_t get_size ( void ) const;
        bool is_empty ( void ) const;
        iterator push_back ( const value_type &new_value );
        iterator push_front ( const value_type &new_value );
        void pop_back ( void );
        void pop_front ( void );
    };


template <typename value_type>
list<value_type>::iterator::iterator ( const list *new_owner, list_node *new_current )
    {
    owner = new_owner;
    current = new_current;
    }

template <typename value_type>
list<value_type>::iterator::iterator ( void )
    {
    owner = nullptr;
    current = nullptr;
    }

template <typename value_type>
list<value_type>::iterator::iterator ( const list<value_type>::iterator &other ) : current ( other.current ), owner ( other.owner ) {}

template <typename value_type>
typename list<value_type>::iterator &list<value_type>::iterator::operator ++ ( void )
    {
    if ( current )
        current = current->next;
    return *this;
    }

template <typename value_type>
typename list<value_type>::iterator &list<value_type>::iterator::operator -- ( void )
    {
    if ( current )
        current = current->previous;
    return *this;
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::iterator::operator++ ( int )
    {
    iterator tmp ( *this );
    operator++();
    return tmp;
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::iterator::operator-- ( int )
    {
    iterator tmp ( *this );
    operator--();
    return tmp;
    }

template <typename value_type>
bool list<value_type>::iterator::operator == ( const iterator &other ) const
    {
    return ( current == other.current );
    }

template <typename value_type>
bool list<value_type>::iterator::operator != ( const iterator &other ) const
    {
    return ( current != other.current );
    }

template <typename value_type>
typename list<value_type>::iterator &list<value_type>::iterator::operator = ( const iterator &other )
    {
    owner = other.owner;
    current = other.current;
    return *this;
    }

template <typename value_type>
value_type &list<value_type>::iterator::operator * ( void ) const
    {
    return current->data;
    }

template <typename value_type>
value_type *list<value_type>::iterator::operator -> ( void ) const
    {
    return & ( current->data );
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::iterator::operator + ( const off_t delta ) const
    {
    iterator target = *this;
    if ( delta > 0 )
        {
        for ( off_t cont = 0; cont < delta; ++cont, ++target );
        }
    if ( delta < 0 )
        {
        for ( off_t cont = 0; cont < delta; ++cont, --target );
        }
    return target;
    }

template <typename value_type>
value_type &list<value_type>::iterator::get ( void ) const
    {
    return current->data;
    }

template <typename value_type>
bool list<value_type>::iterator::is_valid ( void ) const
    {
    return current != nullptr;
    }

template <typename value_type>
list<value_type>::list ( void )
    {
    first = last = nullptr;
    count = 0;
    }

template <typename value_type>
list<value_type>::~list ( void )
    {
    clear();
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::add_at_end ( const value_type &new_value )
    {
    list_node *new_node = new list_node;
    new_node->data = new_value;
    new_node->next = nullptr;

    // Connects this node to the last one
    new_node->previous = last;

    // Connects the last one ( if existant ) to this one
    if ( last != nullptr )
        last->next = new_node;

    // Mark this one as the last one
    last = new_node;

    // In case of an empty list, also mark it as the first one
    if ( first == nullptr )
        first = new_node;

    ++count;
    return iterator ( this, new_node );
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::add_at_begin ( const value_type &new_value )
    {
    list_node *new_node = new list_node;
    new_node->data = new_value;
    new_node->previous = nullptr;

    // Connects this node to the first one
    new_node->next = first;

    // Connects the first one ( if existant ) to this one
    if ( first != nullptr )
        first->previous = new_node;

    // Mark this one as the first one
    first = new_node;

    // In case of an empty list, also mark it as the last one
    if ( last == nullptr )
        last = new_node;

    ++count;
    return iterator ( this, new_node );
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::add_after ( iterator &reference_iterator, const value_type &new_value )
    {
    list_node *new_node = new list_node;
    new_node->data = new_value;

    // Connects this node to the adjacent ones
    new_node->previous = reference_iterator;
    new_node->next = reference_iterator->next;
    reference_iterator->next = new_node;
    if ( new_node->next )
        new_node->next->previous = new_node;
    else
        last = new_node;
    ++count;
    return iterator ( this, new_node );
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::add_before ( iterator &reference_iterator, const value_type &new_value )
    {
    list_node *new_node = new list_node;
    new_node->data = new_value;

    // Connects this node to the adjacent ones
    new_node->next = reference_iterator;
    new_node->previous = reference_iterator->previous;
    reference_iterator->previous = new_node;
    if ( new_node->previous )
        new_node->previous->next = new_node;
    else
        first = new_node;
    ++count;
    return iterator ( this, new_node );
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::erase ( const iterator &reference_iterator )
    {
    if ( reference_iterator.owner != this )
        throw "calling erase with invalid iterator";
    if ( reference_iterator.is_valid() == false )
        throw "calling erase with invalid iterator";
    if ( reference_iterator == end() )
        return end();
    // Connect adjacent nodes to each other
    if ( reference_iterator.current->previous )
        reference_iterator.current->previous->next = reference_iterator.current->next;

    if ( reference_iterator.current->next )
        reference_iterator.current->next->previous = reference_iterator.current->previous;

    // Updates the list first node if this is it
    if ( first == reference_iterator.current )
        first = reference_iterator.current->next;

    if ( last == reference_iterator.current )
        last = reference_iterator.current->previous;
    --count;
    iterator next ( this, reference_iterator.current->next );
    delete reference_iterator.current;
    return next;
    }

template <typename value_type>
void list<value_type>::clear ( void )
    {
    list_node *node;
    for ( node = first; node != nullptr; )
        {
        list_node *next_node = node->next;
        delete node;
        node = next_node;
        }
    first = last = nullptr;
    count = 0;
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::find ( const value_type &value ) const
    {
    list_node *node;
    for ( node = first; node != nullptr; node = node->next )
        {
        if ( node->data == value )
            return iterator ( this, node );
        }
    return end();
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::begin ( void ) const
    {
    return iterator ( this, first );
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::end ( void ) const
    {
    return iterator();
    }

template <typename value_type>
const value_type &list<value_type>::front ( void ) const
    {
    return first->data;
    }

template <typename value_type>
value_type &list<value_type>::front ( void )
    {
    return first->data;
    }

template <typename value_type>
const value_type &list<value_type>::back ( void ) const
    {
    return last->data;
    }

template <typename value_type>
value_type &list<value_type>::back ( void )
    {
    return last->data;
    }

template <typename value_type>
size_t list<value_type>::get_size ( void ) const
    {
    return count;
    }

template <typename value_type>
bool list<value_type>::is_empty ( void ) const
    {
    return ( count == 0 );
    }


template <typename value_type>
typename list<value_type>::iterator list<value_type>::push_back ( const value_type &new_value )
    {
    return add_at_end ( new_value );
    }

template <typename value_type>
typename list<value_type>::iterator list<value_type>::push_front ( const value_type &new_value )
    {
    return add_at_begin ( new_value );
    }

template <typename value_type>
void list<value_type>::pop_back ( void )
    {
    if ( !last )
        return;
    erase ( last );
    }

template <typename value_type>
void list<value_type>::pop_front ( void )
    {
    if ( !first )
        return;
    erase ( first );
    }
}

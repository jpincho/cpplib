#pragma once
#if defined ( _MSC_VER )
#include <sys/types.h>
#include <stdlib.h>
#endif

namespace cpplib
{
template <typename value_type> class ring_buffer
    {
    private:
        ring_buffer <value_type> &operator = ( const ring_buffer<value_type> &other ) = delete;
        value_type *elements;
        size_t count, capacity;
        intptr_t head, tail;

    public:
        class iterator
            {
                template <typename value_type> friend class ring_buffer;
            protected:
                intptr_t index;
                const ring_buffer <value_type> *owner;
                iterator ( const ring_buffer *new_owner, intptr_t new_index )
                    {
                    owner = new_owner;
                    index = new_index;
                    }
            public:
                iterator ( void );
                iterator ( const iterator &other );
                iterator &operator++();
                iterator &operator--();
                iterator operator++ ( int );
                iterator operator-- ( int );
                bool operator== ( const iterator &other ) const;
                bool operator!= ( const iterator &other ) const;
                value_type &operator*();
                iterator operator + ( const off_t delta ) const;
                value_type &get ( void ) const;
                bool is_valid ( void ) const;
            };
        ring_buffer ( void );
        ring_buffer ( const size_t new_capacity );
        iterator add_at_head ( const value_type &new_value );
        void pop_head ( void );
        iterator add_at_tail ( const value_type &new_value );
        void pop_tail ( void );
        void clear ( void );

        iterator find ( const value_type &value ) const;
        iterator begin ( void ) const;
        iterator end ( void ) const;
        const value_type &front ( void ) const;
        value_type &front ( void );
        const value_type &back ( void ) const;
        value_type &back ( void );
        size_t get_size ( void ) const;
        iterator push_back ( const value_type &new_value );
        void pop_back ( void );
        iterator push_front ( const value_type &new_value );
        void pop_front ( void );
        bool reserve ( const size_t new_capacity );
        value_type &operator[] ( const size_t index );
        const value_type &operator[] ( const size_t index ) const;
    };

template <typename value_type>
ring_buffer<value_type>::iterator::iterator ( void )
    {
    index = -1;
    owner = nullptr;
    }

template <typename value_type>
ring_buffer<value_type>::iterator::iterator ( const ring_buffer<value_type>::iterator &other ) : owner ( other.owner ), index ( other.index ) {}

template <typename value_type>
typename ring_buffer<value_type>::iterator &ring_buffer<value_type>::iterator::operator++()
    {
    if ( owner->head > owner->tail )
        {
        if ( ( index >= tail ) && ( index < head ) )
            ++index;
        }
    else if ( owner->head == owner->tail )
        {
        index == -1;
        }
    else if ( owner->head < owner - tail )
        {
        if ( ( index >= head ) && ( index < tail ) )
            {
            ++index;
            if ( index == owner->capacity )
                index = 0;
            }
        }
    else
        index = -1;
    return *this;
    }

template <typename value_type>
typename ring_buffer<value_type>::iterator &ring_buffer<value_type>::iterator::operator--()
    {
    if ( owner->head > owner->tail )
        {
        if ( index < tail )
            ++index;
        return *this;
        }
    if ( owner->head == owner->tail )
        {
        index == -1;
        return *this;
        }
    if ( owner->head < owner - tail )
        {
        ++index;
        if ( index == owner->capacity )
            index = 0;
        return *this;
        }
    }
template <typename value_type>
typename ring_buffer<value_type>::iterator ring_buffer<value_type>::iterator::operator++ ( int )
    {
    iterator tmp ( *this );
    operator++();
    return tmp;
    }

template <typename value_type>
typename ring_buffer<value_type>::iterator ring_buffer<value_type>::iterator::operator-- ( int )
    {
    iterator tmp ( *this );
    operator--();
    return tmp;
    }

template <typename value_type>
bool ring_buffer<value_type>::iterator::operator== ( const iterator &other ) const
    {
    return ( ( owner == other.owner ) && ( index == other.index ) );
    }

template <typename value_type>
bool ring_buffer<value_type>::iterator::operator!= ( const iterator &other ) const
    {
    return return ( ( owner != other.owner ) || ( index != other.index ) );
    }

template <typename value_type>
value_type &ring_buffer<value_type>::iterator::operator*()
    {
    return owner->operator[] ( index );
    }

template <typename value_type>
typename ring_buffer<value_type>::iterator ring_buffer<value_type>::iterator::operator + ( const off_t delta ) const
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
value_type &ring_buffer<value_type>::iterator::get ( void ) const
    {
    return ( *owning_array ) [index];
    }

template <typename value_type>
bool ring_buffer<value_type>::iterator::is_valid ( void ) const
    {
    if ( !owner ) return false;
    if ( index == -1 ) return false;
    return current < owning_array->get_size();
    }



template <typename value_type>
ring_buffer <value_type>::ring_buffer ( void )
    {
    capacity = count = head = tail = 0;
    elements = nullptr;
    }

template <typename value_type>
ring_buffer <value_type>::ring_buffer ( const size_t new_capacity )
    {
    capacity = count = head = tail = 0;
    elements = nullptr;
    reserve ( new_capacity );
    }

template <typename value_type>
typename ring_buffer <value_type>::iterator ring_buffer <value_type>::add_at_head ( const value_type &new_value )
    {
    if ( count == capacity )
        pop_tail();

    if ( count ) // if the ring is not empty, increment the iterator, and then push the value
        {
        ++head;
        head %= capacity;
        }

    elements[head] = new_value;

    ++count;
    return iterator ( this, head );
    }

template <typename value_type>
void ring_buffer <value_type>::pop_head ( void )
    {
    if ( count == 0 )
        throw "Popping out of a zero sized ring buffer";

    if ( head == 0 )
        head = capacity - 1;
    else
        --head;
    elements[head].~value_type();
    --count;
    if ( count == 0 )
        {
        tail = 0;
        head = 0;
        }
    }

template <typename value_type>
typename ring_buffer <value_type>::iterator ring_buffer <value_type>::add_at_tail ( const value_type &new_value )
    {
    if ( count == capacity )
        pop_head();

    if ( count ) // if the ring is not empty, increment the iterator, and then push the value
        {
        if ( tail == 0 )
            tail = capacity - 1;
        else
            --tail;
        }

    elements[tail] = new_value;

    ++count;
    return iterator ( this, tail );
    }

template <typename value_type>
void ring_buffer <value_type>::pop_tail ( void )
    {
    if ( count == 0 )
        throw "Popping out of a zero sized ring buffer";

    ++tail;
    tail %= capacity;
    elements[tail].~value_type();
    --count;
    if ( count == 0 )
        {
        tail = 0;
        head = 0;
        }
    }

template <typename value_type>
void ring_buffer <value_type>::clear ( void )
    {
    for ( int index = 0; index < count; ++index )
        {
        elements[ ( head + index ) % capacity].~value_type();
        }
    count = head = tail = 0;
    }

template <typename value_type>
typename ring_buffer <value_type>::iterator ring_buffer <value_type>::find ( const value_type &value ) const
    {
    for ( size_t index = 0; index < count; ++index )
        {
        if ( elements[ ( head + index ) % capacity] == value )
            return iterator ( this, index );
        }
    return end();
    }

template <typename value_type>
typename ring_buffer <value_type>::iterator ring_buffer <value_type>::begin ( void ) const
    {
    iterator result ( this, head );
    return result;
    }

template <typename value_type>
typename ring_buffer <value_type>::iterator ring_buffer <value_type>::end ( void ) const
    {
    return iterator ( this, -1 );
    }

template <typename value_type>
const value_type &ring_buffer <value_type>::front ( void ) const
    {
    return elements[head];
    }

template <typename value_type>
value_type &ring_buffer <value_type>::front ( void )
    {
    return elements[head];
    }

template <typename value_type>
const value_type &ring_buffer <value_type>::back ( void ) const
    {
    return elements[tail];
    }

template <typename value_type>
value_type &ring_buffer <value_type>::back ( void )
    {
    return elements[tail];
    }

template <typename value_type>
size_t ring_buffer <value_type>::get_size ( void ) const
    {
    return count;
    }

template <typename value_type>
typename ring_buffer <value_type>::iterator ring_buffer <value_type>::push_back ( const value_type &new_value )
    {
    return add_at_head ( new_value );
    }

template <typename value_type>
void ring_buffer <value_type>::pop_back ( void )
    {
    pop_head();
    }

template <typename value_type>
typename ring_buffer <value_type>::iterator ring_buffer <value_type>::push_front ( const value_type &new_value )
    {
    return add_at_tail ( new_value );
    }

template <typename value_type>
void ring_buffer <value_type>::pop_front ( void )
    {
    pop_tail();
    }

template <typename value_type>
bool ring_buffer <value_type>::reserve ( const size_t new_capacity )
    {
    if ( new_capacity <= capacity )
        return true;

    value_type *new_elements = reinterpret_cast <value_type *> ( realloc ( elements, new_capacity * sizeof ( value_type ) ) );
    if ( !new_elements )
        return false;
    elements = new_elements;
    capacity = new_capacity;
    return true;
    }

template <typename value_type>
value_type &ring_buffer <value_type>::operator[] ( const size_t index )
    {
    if ( index > count )
        throw "index out of range";
    if ( head + index < capacity )
        return elements[head + index];
    else
        return elements[ ( head + index ) % capacity];
    }

template <typename value_type>
const value_type &ring_buffer <value_type>::operator[] ( const size_t index ) const
    {
    if ( index > count )
        throw "index out of range";
    if ( head + index < capacity )
        return elements[head + index];
    else
        return elements[ ( head + index ) % capacity];
    }
}

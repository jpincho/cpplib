#pragma once
#include <stdint.h>
#include <assert.h>

namespace cpplib
{
template <typename value_type> class vectorized_container
    {
    private:
        class list_node
            {
            public:
                intptr_t previous, next;
                value_type data;
            };

        typedef struct list_struct
            {
            intptr_t first, last;
            size_t count;
            } list_struct;

        list_node *elements;
        list_struct free_nodes, used_nodes;
        size_t capacity;

        intptr_t get_node_from_list ( list_struct &list );
        void remove_node_from_list ( list_struct &list, const intptr_t index );
        void add_node_to_list ( list_struct &list, const intptr_t index );

    public:
        class iterator
            {
                template <typename value_type> friend class vectorized_container;
            protected:
                intptr_t index;
                const vectorized_container <value_type> *owner;
                iterator ( const vectorized_container *new_owner, intptr_t new_index );
            public:
                iterator ( const iterator &other );
                iterator &operator ++ ( void );
                iterator &operator -- ( void );
                iterator operator++ ( int );
                iterator operator-- ( int );
                bool operator == ( const iterator &other ) const;
                bool operator != ( const iterator &other ) const;
                value_type &operator * ( void ) const;
                size_t get_index ( void ) const;
            };

        vectorized_container ( void );
        void reserve ( const size_t new_capacity );
        size_t get_free_index ( void );
        void release_index ( const size_t index );
        size_t size ( void ) const;
        value_type &operator [] ( const size_t index );
        iterator begin ( void ) const;
        iterator end ( void ) const;
        void clear ( void );
        void debug ( void ( *func ) ( const intptr_t, const value_type & ) );
        void debug ( void );
    };

template <typename value_type>
vectorized_container <value_type>::iterator::iterator ( const vectorized_container *new_owner, intptr_t new_index )
    {
    owner = new_owner;
    index = new_index;
    }

template <typename value_type>
vectorized_container <value_type>::iterator::iterator ( const iterator &other ) : index ( other.index ) {}

template <typename value_type>
typename vectorized_container <value_type>::iterator &vectorized_container <value_type>::iterator::operator++()
    {
    if ( index != -1 )
        index = owner->elements[index].next;
    return *this;
    }

template <typename value_type>
typename vectorized_container <value_type>::iterator &vectorized_container <value_type>::iterator::operator--()
    {
    if ( index != -1 )
        index = owner->elements[index].previous;
    return *this;
    }

template <typename value_type>
typename vectorized_container <value_type>::iterator vectorized_container <value_type>::iterator::operator++ ( int )
    {
    iterator tmp ( *this );
    operator++();
    return tmp;
    }

template <typename value_type>
typename vectorized_container <value_type>::iterator vectorized_container <value_type>::iterator::operator-- ( int )
    {
    iterator tmp ( *this );
    operator--();
    return tmp;
    }

template <typename value_type>
bool vectorized_container <value_type>::iterator::operator== ( const iterator &other ) const
    {
    return index == other.index;
    }

template <typename value_type>
bool vectorized_container <value_type>::iterator::operator!= ( const iterator &other ) const
    {
    return index != other.index;
    }

template <typename value_type>
value_type &vectorized_container <value_type>::iterator::operator* ( void ) const
    {
    return owner->elements[index].data;
    }

template <typename value_type>
size_t vectorized_container <value_type>::iterator::get_index ( void ) const
    {
    return index;
    }

template <typename value_type>
intptr_t vectorized_container<value_type>::get_node_from_list ( list_struct &list )
    {
    size_t index = list.first;
    if ( elements[list.first].next != -1 ) // If there is a second node, set its previous value to -1
        elements[elements[list.first].next].previous = -1;
    list.first = elements[list.first].next;
    if ( list.first == -1 ) // If I ran out of nodes
        list.last = -1;
    elements[index].previous = elements[index].next = -1;
    --list.count;
    return index;
    }

template <typename value_type>
void vectorized_container<value_type>::remove_node_from_list ( list_struct &list, const intptr_t index )
    {
    assert ( list.count > 0 );
    if ( elements[index].previous != -1 )
        elements[elements[index].previous].next = elements[index].next;
    if ( elements[index].next != -1 )
        elements[elements[index].next].previous = elements[index].previous;
    if ( list.first == index )
        list.first = elements[index].next;
    if ( list.last == index )
        list.last = elements[index].previous;
    --list.count;
    }

template <typename value_type>
void vectorized_container<value_type>::add_node_to_list ( list_struct &list, const intptr_t index )
    {
    if ( list.count == 0 )
        {
        list.first = list.last = index;
        list.count = 1;
        elements[index].previous = elements[index].next = -1;
        return;
        }

#if defined ( VECTORIZED_CONTAINER_PACK_OBJECTS )
    // if the new handle comes closer to the head of the buffer than the current first free node, update list.first and be done with it
    if ( index < ( size_t ) list.first_node )
        {
        nodes[list.first_node].previous = index;
        nodes[index].next = list.first_node;
        nodes[index].previous = -1;
        list.first_node = index;
        }
    // same thing for the tail of the buffer
    else if ( index > ( size_t ) list.last_node )
        {
        nodes[list.last_node].next = index;
        nodes[index].previous = list.last_node;
        nodes[index].next = -1;
        list.last_node = index;
        }
    else
        {
        // If I want to pack nodes as much as possible, the indices need to be ordered, so that the lower-index free slots are the first to be reused
        size_t current_free_node_index = list.first_node;
        while ( nodes[current_free_node_index].next != -1 )
            {
            if ( index > current_free_node_index )
                {
                current_free_node_index = nodes[current_free_node_index].next;
                continue;
                }
            // when I get here, it's because I've found an index in the free list which is bigger than my own. insert mine right before it
            nodes[index].previous = nodes[current_free_node_index].previous;
            nodes[index].next = current_free_node_index;
            nodes[nodes[current_free_node_index].previous].next = index;
            nodes[current_free_node_index].previous = index;
            break;
            }
        }
#else
    // Adds this index to the end of the list
    elements[list.last].next = index;
    elements[index].previous = list.last;
    elements[index].next = -1;
    list.last = index;
#endif
    ++list.count;
    }

template <typename value_type>
vectorized_container<value_type>::vectorized_container ( void )
    {
    free_nodes.first = free_nodes.last = used_nodes.first = used_nodes.last = -1;
    capacity = free_nodes.count = used_nodes.count = 0;
    elements = nullptr;
    }

template <typename value_type>
void vectorized_container<value_type>::reserve ( const size_t new_capacity )
    {
    if ( new_capacity <= capacity )
        return;
    size_t delta = new_capacity - capacity;
    list_node *new_pointer = ( list_node * ) realloc ( elements, sizeof ( list_node ) * new_capacity );
    assert ( new_pointer );
    elements = new_pointer;

    for ( size_t index = capacity; index < new_capacity; ++index ) // Add all the new nodes to the list
        {
        new ( &elements[index] ) list_node();
        add_node_to_list ( free_nodes, index );
        }
    capacity += delta;
    }

template <typename value_type>
size_t vectorized_container<value_type>::get_free_index ( void )
    {
    if ( free_nodes.count == 0 )
        {
        reserve ( capacity + 10 );
        }
    size_t index = get_node_from_list ( free_nodes );
    add_node_to_list ( used_nodes, index );
    return index;
    }

template <typename value_type>
void vectorized_container<value_type>::release_index ( const size_t index )
    {
    assert ( index < capacity );

    elements[index].~list_node();
    remove_node_from_list ( used_nodes, index );
    add_node_to_list ( free_nodes, index );
    }

template <typename value_type>
size_t vectorized_container<value_type>::size ( void ) const
    {
    return capacity - free_nodes.count;
    }

template <typename value_type>
value_type &vectorized_container<value_type>::operator [] ( const size_t index )
    {
    assert ( index < capacity );
    return elements[index].data;
    }

template <typename value_type>
typename vectorized_container<value_type>::iterator vectorized_container<value_type>::begin ( void ) const
    {
    if ( used_nodes.count == 0 )
        return end();
    return iterator ( this, used_nodes.first );
    }

template <typename value_type>
typename vectorized_container<value_type>::iterator vectorized_container<value_type>::end ( void ) const
    {
    return iterator ( this, -1 );
    }

template <typename value_type>
void vectorized_container<value_type>::clear ( void )
    {
    if ( capacity == 0 )
        return;
    intptr_t index = used_nodes.first;
    while ( index != -1 )
        {
        intptr_t next = elements[index].next;
        elements[index].~list_node();
        index = next;
        }
    used_nodes.first = used_nodes.last = -1;
    used_nodes.count = 0;
    free_nodes.first = 0;
    free_nodes.last = capacity - 1;
    free_nodes.count = capacity;
    for ( size_t index2 = 0; index2 < capacity; ++index2 )
        {
        elements[index2].previous = index2 - 1;
        elements[index2].next = index2 + 1;
        }
    elements[free_nodes.first].previous = -1;
    elements[free_nodes.last].next = -1;
    }

template <typename value_type>
void vectorized_container<value_type>::debug ( void ( *func ) ( const intptr_t, const value_type & ) )
    {
    for ( unsigned index = 0; index < capacity; ++index )
        {
        func ( index, elements[index] );
        printf ( "%Ii %Ii\n", elements[index].previous, elements[index].next );
        }
    intptr_t index = free_nodes.first;
    while ( index != -1 )
        {
        printf ( "%Ii <- %Iu -> %Ii\n", elements[index].previous, index, elements[index].next );
        index = elements[index].next;
        }
    }

template <typename value_type>
void vectorized_container<value_type>::debug ( void )
    {
    printf ( "Complete dump\n" );
    for ( unsigned index = 0; index < capacity; ++index )
        {
        printf ( "%Ii %u %Ii\n", elements[index].previous, index, elements[index].next );
        }
    printf ( "Free node list\n" );
    intptr_t index = free_nodes.first;
    while ( index != -1 )
        {
        printf ( "%Ii <- %Iu -> %Ii\n", elements[index].previous, index, elements[index].next );
        index = elements[index].next;
        }
    }
}

#pragma once
#include <sys/types.h>
#include <stdint.h>
#include <new>

namespace cpplib
{
template <typename value_type> class array
    {
    private:
        value_type *elements;
        size_t count, capacity;
    public:
        class iterator
            {
                template <typename value_type> friend class array;
            protected:
                off_t index;
                const array <value_type> *owner;
                iterator ( const array *new_owner, off_t new_index );
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
                iterator operator + ( const off_t delta ) const;
                value_type &get ( void ) const;
                bool is_valid ( void ) const;
            };

        array ( void );
        array ( const array &other );
        array ( array &&other );
        ~array ( void );
        iterator add_at_end ( const value_type &new_value );
        void remove_from_end ( void );
        iterator erase ( const iterator &reference_iterator, const size_t erase_count = 1 );
        void erase ( const size_t &index, const size_t erase_count = 1 );
        bool reserve ( size_t new_capacity );
        bool resize ( size_t new_size );
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
        void assign ( const value_type *new_elements, const size_t new_length );
        void assign ( value_type const *first_element, value_type const *past_last_element );
        void assign ( const size_t new_length, const value_type new_element );
        value_type &operator [] ( size_t index ) const;
        array <value_type> &operator = ( const array <value_type> &other );
        array <value_type> &operator += ( const array <value_type> &other );
        bool operator == ( const array <value_type> &other ) const;
        bool operator != ( const array <value_type> &other ) const;
        value_type *get_data ( void ) const;

        iterator push_back ( const value_type &new_value );
        void pop_back ( void );
    };

template <typename value_type>
array<value_type>::iterator::iterator ( const array *new_owner, off_t new_index )
    {
    owner = new_owner;
    index = new_index;
    }

template <typename value_type>
array<value_type>::iterator::iterator ( void )
    {
    index = -1;
    owner = nullptr;
    }

template <typename value_type>
array<value_type>::iterator::iterator ( const array<value_type>::iterator &other ) : index ( other.index ), owner ( other.owner ) {}

template <typename value_type>
typename array<value_type>::iterator &array<value_type>::iterator::operator++()
    {
    ++index;
    if ( index >= owner->get_size () )
        {
        index = -1;
        }
    return *this;
    }

template <typename value_type>
typename array<value_type>::iterator &array<value_type>::iterator::operator--()
    {
    if ( index != -1 )
        --index;
    return *this;
    }

template <typename value_type>
typename array<value_type>::iterator array<value_type>::iterator::operator++ ( int )
    {
    iterator tmp ( *this );
    operator++();
    return tmp;
    }

template <typename value_type>
typename array<value_type>::iterator array<value_type>::iterator::operator-- ( int )
    {
    iterator tmp ( *this );
    operator--();
    return tmp;
    }

template <typename value_type>
bool array<value_type>::iterator::operator== ( const iterator &other ) const
    {
    return ( ( owner == other.owner ) && ( index == other.index ) );
    }

template <typename value_type>
bool array<value_type>::iterator::operator!= ( const iterator &other ) const
    {
    return ( ( owner != other.owner ) || ( index != other.index ) );
    }

template <typename value_type>
typename array<value_type>::iterator &array<value_type>::iterator::operator = ( const iterator &other )
    {
    owner = other.owner;
    index = other.index;
    return *this;
    }

template <typename value_type>
value_type &array<value_type>::iterator::operator * ( void ) const
    {
    return owner->operator[] ( index );
    }

template <typename value_type>
typename array<value_type>::iterator array<value_type>::iterator::operator + ( const off_t delta ) const
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
value_type &array<value_type>::iterator::get ( void ) const
    {
    return ( *owner ) [ index ];
    }

template <typename value_type>
bool array<value_type>::iterator::is_valid ( void ) const
    {
    if ( !owner ) return false;
    if ( index == -1 ) return false;
    return index < owner->get_size ();
    }

template <typename value_type>
array<value_type>::array ( void )
    {
    elements = nullptr;
    count = capacity = 0;
    }

template <typename value_type>
array<value_type>::array ( const array &other )
    {
    elements = nullptr;
    count = capacity = 0;
    reserve ( other.count );
    for ( size_t index = 0; index < other.count; ++index )
        push_back ( other.elements[index] );
    }

template <typename value_type>
array<value_type>::array ( array &&other )
    {
    elements = other.elements;
    count = other.count;
    capacity = other.capacity;
    other.capacity = 0;
    other.count = 0;
    other.elements = nullptr;
    }

template <typename value_type>
array<value_type>::~array ( void )
    {
    clear();
    free ( elements );
    elements = nullptr;
    capacity = 0;
    }

template <typename value_type>
typename array<value_type>::iterator array<value_type>::add_at_end ( const value_type &new_value )
    {
    if ( count == capacity )
        reserve ( capacity + 10 );
    new ( elements + count ) value_type ( new_value );
    ++count;
    return iterator ( this, ( off_t ) count - 1 );
    }

template <typename value_type>
void array<value_type>::remove_from_end ( void )
    {
    if ( count > 0 )
        {
        elements[ count ].~value_type ();
        --count;
        }
    }

template <typename value_type>
typename array<value_type>::iterator array<value_type>::erase ( const iterator &reference_iterator, const size_t erase_count )
    {
    if ( ( array * ) reference_iterator.owner != this )
        throw "calling erase with invalid iterator";
    if ( reference_iterator.is_valid() == false )
        throw "calling erase with invalid iterator";
    erase ( reference_iterator.index, erase_count );
    if ( reference_iterator.index < count )
        return iterator ( this, reference_iterator.index );
    return end ();
    }

template <typename value_type>
void array<value_type>::erase ( const size_t &index, const size_t erase_count )
    {
    if ( index > count )
        throw "calling erase with out of bounds index";
    if ( index + erase_count > count )
        throw "calling erase with out of bounds index + erase_count";
    for ( size_t iterator = index; iterator < count - erase_count; ++iterator )
        elements[iterator] = elements[iterator + erase_count];
    count -= erase_count;;
    }

template <typename value_type>
bool array<value_type>::reserve ( size_t new_capacity )
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
bool array<value_type>::resize ( size_t new_size )
    {
    if ( reserve ( new_size ) == false )
        return false;
    count = new_size;
    return true;
    }

template <typename value_type>
void array<value_type>::clear ( void )
    {
    for ( unsigned index = 0; index < count; ++index )
        {
        elements[ index ].~value_type ();
        }
    count = 0;
    }

template <typename value_type>
typename array<value_type>::iterator array<value_type>::find ( const value_type &value ) const
    {
    for ( size_t index = 0; index < count; ++index )
        {
        if ( elements[ index ] == value )
            return iterator ( this, index );
        }
    return end ();
    }

template <typename value_type>
typename array<value_type>::iterator array<value_type>::begin ( void ) const
    {
    return iterator ( this, 0 );;
    }

template <typename value_type>
typename array<value_type>::iterator array<value_type>::end ( void ) const
    {
    return iterator ( this, -1 );
    }

template <typename value_type>
const value_type &array<value_type>::front ( void ) const
    {
    return elements[ 0 ];
    }

template <typename value_type>
value_type &array<value_type>::front ( void )
    {
    return elements[ 0 ];
    }

template <typename value_type>
const value_type &array<value_type>::back ( void ) const
    {
    return elements[ count - 1 ];
    }

template <typename value_type>
value_type &array<value_type>::back ( void )
    {
    return elements[ count - 1 ];
    }

template <typename value_type>
size_t array<value_type>::get_size ( void ) const
    {
    return count;
    }

template <typename value_type>
bool array<value_type>::is_empty ( void ) const
    {
    return count == 0;
    }

template <typename value_type>
void array<value_type>::assign ( const value_type *new_elements, const size_t new_length )
    {
    if ( new_length == 0 )
        return;
    resize ( new_length );
    for ( size_t index = 0; index < new_length; ++index )
        elements[ index ] = new_elements[ index ];
    }

template <typename value_type>
void array<value_type>::assign ( value_type const *first_element, value_type const *past_last_element )
    {
    if ( past_last_element - first_element <= 0 )
        throw "assign with invalid parameters";
    resize ( past_last_element - first_element );
    for ( size_t index = 0; first_element < past_last_element; ++first_element, ++index )
        elements[ index ] = *first_element;
    }

template <typename value_type>
void array<value_type>::assign ( const size_t new_length, const value_type new_element )
    {
    if ( new_length == 0 )
        return;
    resize ( new_length );
    for ( size_t index = 0; index < new_length; ++index )
        elements[ index ] = new_element;
    }

template <typename value_type>
value_type &array<value_type>::operator [] ( size_t index ) const
    {
    if ( index >= count )
        {
        throw "trying to access position out of bounds";
        }
    return elements[ index ];
    }

template <typename value_type>
array <value_type> &array<value_type>::operator = ( const array <value_type> &other )
    {
    clear();
    reserve ( other.count );
    for ( size_t index = 0; index < other.count; ++index )
        push_back ( other.elements[index] );
    return *this;
    }

template <typename value_type>
array <value_type> &array<value_type>::operator += ( const array <value_type> &other )
    {
    reserve ( count + other.count );
    for ( size_t index = 0; index < other.count; ++index )
        push_back ( other.elements[index] );
    return *this;
    }

template <typename value_type>
bool array<value_type>::operator == ( const array <value_type> &other ) const
    {
    if ( count != other.count )
        return false;
    for ( size_t index = 0; index < other.count; ++index )
        {
        if ( elements[index] != other.elements[index] )
            return false;
        }
    return true;
    }

template <typename value_type>
bool array<value_type>::operator != ( const array <value_type> &other ) const
    {
    return !operator== ( other );
    }

template <typename value_type>
value_type *array<value_type>::get_data ( void ) const
    {
    return elements;
    }

template <typename value_type>
typename array<value_type>::iterator array<value_type>::push_back ( const value_type &new_value )
    {
    return add_at_end ( new_value );
    }

template <typename value_type>
void array<value_type>::pop_back ( void )
    {
    remove_from_end();
    }
}

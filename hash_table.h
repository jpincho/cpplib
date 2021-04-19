#pragma once
#include "hash.h"
#include "array.h"

namespace cpplib
{
template <typename key_type, typename value_type>
class hash_table
    {
        //template <typename key_type, typename value_type> friend class hash_table::iterator;
    public:
        class key_value_pair
            {
            public:
                key_type key;
                value_type value;
                key_value_pair() = default;
                key_value_pair ( const key_type &new_key, const value_type &new_value ) : key ( new_key ), value ( new_value )
                    {
                    }
            };
    private:
        typedef struct hash_table_entry
            {
            array <key_value_pair> bucket;
            } hash_table_entry;
        size_t bucket_count;
        hash_table_entry *entries;
    public:
        class iterator
            {
                template <typename key_type, typename value_type> friend class hash_table;
            protected:
                const hash_table *owner;
                size_t bucket_index, entry_index;
                iterator ( const hash_table *new_owner, size_t new_bucket_index, size_t new_entry_index )
                    {
                    owner = new_owner;
                    bucket_index = new_bucket_index;
                    entry_index = new_entry_index;
                    }

            public:
                iterator ( void )
                    {
                    owner = nullptr;
                    bucket_index = 0;
                    entry_index = 0;
                    }
                iterator ( const iterator &other ) : owner ( other.owner ), bucket_index ( other.bucket_index ), entry_index ( other.entry_index ) {}
                iterator &operator++()
                    {
                    if ( owner->entries[bucket_index].bucket.get_size() > entry_index + 1 ) // Still have entries in this bucket, so just move forward in the bucket
                        {
                        ++entry_index;
                        return *this;
                        }
                    // find the next bucket that contains an entry
                    for ( ++bucket_index; bucket_index < owner->bucket_count; ++bucket_index )
                        {
                        if ( owner->entries[bucket_index].bucket.get_size() )
                            {
                            entry_index = 0;
                            return *this;
                            }
                        }

                    // invalidate iterator
                    owner = nullptr;
                    return *this;
                    }
                iterator &operator--()
                    {
                    if ( ( entry_index > 0 ) && ( entries_pointer[bucket_index].bucket.size() > 1 ) ) // Still have entries in this bucket, so just move backward in the bucket
                        {
                        --entry_index;
                        return *this;
                        }
                    // find the previous bucket that contains an entry
                    for ( --bucket_index; bucket_index >= 0 && bucket_index < owner->bucket_count; --bucket_index )
                        {
                        if ( owner->entries[bucket_index].bucket.size() )
                            {
                            entry_index = owner->entries[bucket_index].bucket.size() - 1;
                            return *this;
                            }
                        }
                    // invalidate iterator
                    owner = nullptr;
                    return *this;
                    }
                iterator operator++ ( int )
                    {
                    iterator tmp ( *this );
                    operator++();
                    return tmp;
                    }
                iterator operator-- ( int )
                    {
                    iterator tmp ( *this );
                    operator--();
                    return tmp;
                    }
                bool operator== ( const iterator &other ) const
                    {
                    return ( ( owner == other.owner ) &&
                             ( entry_index == other.entry_index ) &&
                             ( bucket_index == bucket_index ) );
                    }
                bool operator!= ( const iterator &other ) const
                    {
                    return ( ( owner != other.owner ) ||
                             ( entry_index != other.entry_index ) ||
                             ( bucket_index != bucket_index ) );
                    }
                iterator &operator = ( const iterator &other )
                    {
                    owner = other.owner;
                    entry_index = other.entry_index;
                    bucket_index = other.bucket_index;
                    return *this;
                    }
                key_value_pair &operator*()
                    {
                    return owner->entries[bucket_index].bucket[entry_index];
                    }
                iterator operator + ( const off_t delta ) const
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
                key_value_pair &get ( void ) const
                    {
                    return owner->entries[bucket_index].bucket[entry_index];
                    }
                bool is_valid ( void ) const
                    {
                    return ( owner != nullptr );
                    }
            };

        hash_table ( void )
            {
            bucket_count = 128;
            entries = new hash_table_entry[128];
            }
        ~hash_table ( void )
            {
            delete[] entries;
            }
        void add ( const key_type &new_key, const value_type &new_value )
            {
            uint64_t hash_value = hash ( new_key );
            size_t bucket_index = hash_value % bucket_count;
            for ( size_t entry_index = 0; entry_index < entries[bucket_index].bucket.size(); ++entry_index )
                {
                if ( entries[bucket_index].bucket[entry_index].key == new_key )
                    {
                    entries[bucket_index].bucket[entry_index].value = new_value;
                    return;
                    }
                }
            entries[bucket_index].bucket.push_back ( key_value_pair ( new_key, new_value ) );
            }

        void erase ( const key_type &key )
            {
            uint64_t hash_value = hash ( key );
            size_t bucket_index = hash_value % bucket_count;
            for ( size_t entry_index = 0; entry_index < entries[bucket_index].bucket.size(); ++entry_index )
                {
                if ( entries[bucket_index].bucket[entry_index].key == key )
                    {
                    entries[bucket_index].bucket.erase ( entry_index );
                    return;
                    }
                }
            }

        void erase ( const iterator &reference_iterator )
            {
            if ( reference_iterator.owner != this )
                throw "calling erase with invalid iterator";
            if ( reference_iterator.is_valid() == false )
                throw "calling erase with invalid iterator";
            if ( reference_iterator.bucket_index > bucket_count )
                throw "calling erase with iterator with invalid bucket_index";
            if ( reference_iterator.entry_index > entries[reference_iterator.bucket_index].bucket.get_size() )
                throw "calling erase with iterator with invalid entry_index";

            entries[reference_iterator.bucket_index].bucket.erase ( reference_iterator.entry_index );
            }

        void clear ( void )
            {
            for ( size_t bucket_index = 0; bucket_index < bucket_count; ++bucket_index )
                entries[bucket_index].bucket.clear();
            }

        iterator find ( const key_type &key )
            {
            uint64_t hash_value = hash ( key );
            size_t bucket_index = hash_value % bucket_count;
            for ( size_t entry_index = 0; entry_index < entries[bucket_index].bucket.get_size(); ++entry_index )
                {
                if ( entries[bucket_index].bucket[entry_index].key == key )
                    {
                    return iterator ( this, bucket_index, entry_index );
                    }
                }
            return iterator();
            }

        iterator begin ( void ) const
            {
            for ( size_t bucket_index = 0; bucket_index < bucket_count; ++bucket_index )
                {
                if ( entries[bucket_index].bucket.get_size() )
                    return iterator ( this, bucket_index, 0 );
                }
            return iterator();
            }
        iterator end ( void ) const
            {
            return iterator();
            }
        value_type &operator [] ( const key_type &new_key )
            {
            uint64_t hash_value = hash ( new_key );
            size_t bucket_index = hash_value % bucket_count;
            for ( size_t entry_index = 0; entry_index < entries[bucket_index].bucket.get_size(); ++entry_index )
                {
                if ( entries[bucket_index].bucket[entry_index].key == new_key )
                    {
                    return entries[bucket_index].bucket[entry_index].value;
                    }
                }
            entries[bucket_index].bucket.push_back ( key_value_pair ( new_key, value_type() ) );
            return entries[bucket_index].bucket.back().value;
            }
    };
}

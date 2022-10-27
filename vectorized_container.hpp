#pragma once
#if defined ( _MSC_VER )
#include <assert.h>
#endif

namespace cpplib
{
template <typename value_type> class vectorized_container
	{
	private:
		struct list_node
			{
			intptr_t previous, next;
			value_type data;
			};

		struct list_struct
			{
			intptr_t first, last;
			size_t count;
			};

		list_node *elements;
		list_struct free_nodes, used_nodes;
		size_t capacity;

		bool is_node_in_list ( const list_struct &list, const intptr_t index ) const
			{
			for ( intptr_t index_iterator = list.first; index_iterator != -1; index_iterator = elements[index_iterator].next )
				{
				if ( index_iterator == index )
					return true;
				}
			return false;
			}

		intptr_t get_node_from_list ( list_struct &list )
			{
			intptr_t index = list.first;
			if ( elements[list.first].next != -1 ) // If there is a second node, set its previous value to -1
				elements[elements[list.first].next].previous = -1;
			list.first = elements[list.first].next;
			if ( list.first == -1 ) // If I ran out of nodes
				list.last = -1;
			elements[index].previous = elements[index].next = -1;
			--list.count;
			return index;
			}

		void remove_node_from_list ( list_struct &list, const intptr_t index )
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

		void add_node_to_list ( list_struct &list, const intptr_t index )
			{
			if ( list.count == 0 )
				{
				list.first = list.last = index;
				list.count = 1;
				elements[index].previous = elements[index].next = -1;
				return;
				}

			if ( pack_elements )
				{
				// if the new handle comes closer to the head of the buffer than the current first free node, update list.first and be done with it
				if ( index < list.first )
					{
					elements[list.first].previous = index;
					elements[index].next = list.first;
					elements[index].previous = -1;
					list.first = index;
					}
				// same thing for the tail of the buffer
				else if ( index > list.last )
					{
					elements[list.last].next = index;
					elements[index].previous = list.last;
					elements[index].next = -1;
					list.last = index;
					}
				else
					{
					// If I want to pack elements as much as possible, the indices need to be ordered, so that the lower-index free slots are the first to be reused
					intptr_t current_free_node_index = list.first;
					while ( elements[current_free_node_index].next != -1 )
						{
						if ( index > current_free_node_index )
							{
							current_free_node_index = elements[current_free_node_index].next;
							continue;
							}
						// when I get here, it's because I've found an index in the free list which is bigger than my own. insert mine right before it
						elements[index].previous = elements[current_free_node_index].previous;
						elements[index].next = current_free_node_index;
						elements[elements[current_free_node_index].previous].next = index;
						elements[current_free_node_index].previous = index;
						break;
						}
					}
				}
			else
				{
				// Adds this index to the end of the list
				elements[list.last].next = index;
				elements[index].previous = list.last;
				elements[index].next = -1;
				list.last = index;
				}
			++list.count;
			}
		vectorized_container <value_type> &operator = ( const vectorized_container<value_type> &other ) = delete;
	public:
		bool pack_elements;
		class iterator
			{
			protected:
				const vectorized_container <value_type> *owner;
				intptr_t index;
			public:
				iterator ( const vectorized_container *new_owner, intptr_t new_index ) : owner ( new_owner ), index ( new_index ) {}
				iterator ( void ) : owner ( nullptr ), index ( -1 ) {}
				iterator ( const iterator &other ) : owner ( other.owner ), index ( other.index ) {}
				iterator &operator++ ( void )
					{
					if ( index != -1 )
						index = owner->elements[index].next;
					return *this;
					}
				iterator &operator-- ( void )
					{
					if ( index != -1 )
						index = owner->elements[index].previous;
					return *this;
					}
				iterator operator ++ ( int )
					{
					iterator tmp ( *this );
					operator++();
					return tmp;
					}
				iterator operator -- ( int )
					{
					iterator tmp ( *this );
					operator--();
					return tmp;
					}
				bool operator == ( const iterator &other ) const
					{
					return index == other.index;
					}
				bool operator != ( const iterator &other ) const
					{
					return index != other.index;
					}
				iterator &operator = ( const iterator &other )
					{
					index = other.index;
					owner = other.owner;
					return *this;
					}
				explicit operator bool ( void ) const
					{
					return is_valid ();
					}
				value_type &operator* ( void ) const
					{
					return get_value ();
					}
				value_type *operator-> ( void ) const
					{
					return &( owner->elements[index].data );
					}
				size_t get_index ( void ) const
					{
					return index;
					}
				value_type &get_value ( void ) const
					{
					return owner->elements[index].data;
					}
				bool is_valid ( void ) const
					{
					return index != -1;
					}
			};

		vectorized_container ( void )
			{
			free_nodes.first = free_nodes.last = used_nodes.first = used_nodes.last = -1;
			capacity = free_nodes.count = used_nodes.count = 0;
			elements = nullptr;
			pack_elements = true;
			}
		void reserve ( const size_t new_capacity )
			{
			if ( new_capacity <= capacity )
				return;
			size_t delta = new_capacity - capacity;
			list_node *Newpointer = (list_node *) realloc ( (void *) elements, sizeof ( list_node ) * new_capacity );
			assert ( Newpointer );
			elements = Newpointer;

			for ( size_t index = capacity; index < new_capacity; ++index ) // Add all the new nodes to the list
				{
				new ( &elements[index] ) list_node ();
				add_node_to_list ( free_nodes, index );
				}
			capacity += delta;
			}
		size_t get_free_index ( void )
			{
			if ( free_nodes.count == 0 )
				{
				reserve ( capacity + 10 );
				}
			size_t index = get_node_from_list ( free_nodes );
			add_node_to_list ( used_nodes, index );
			return index;
			}
		iterator get_free_slot ( void )
			{
			if ( free_nodes.count == 0 )
				{
				reserve ( capacity + 10 );
				}
			size_t index = get_node_from_list ( free_nodes );
			add_node_to_list ( used_nodes, index );
			return iterator ( this, index );
			}
		void release_index ( const size_t index )
			{
			assert ( index < capacity );

			elements[index].~list_node ();
			remove_node_from_list ( used_nodes, index );
			add_node_to_list ( free_nodes, index );
			}
		void release_slot ( const iterator iterator )
			{
			assert ( iterator.index < capacity );
			if ( !iterator )
				return;
			elements[iterator.index].~list_node ();
			remove_node_from_list ( used_nodes, iterator.index );
			add_node_to_list ( free_nodes, iterator.index );
			}
		size_t get_size ( void ) const
			{
			return capacity - free_nodes.count;
			}
		inline value_type &operator[] ( const size_t index ) const
			{
			assert ( index < capacity );
			return elements[index].data;
			}
		iterator begin ( void ) const
			{
			if ( used_nodes.count == 0 )
				return end ();
			return iterator ( this, used_nodes.first );
			}
		iterator end ( void ) const
			{
			return iterator ( this, -1 );
			}
		iterator first ( void ) const
			{
			return begin ();
			}
		iterator last ( void ) const
			{
			if ( used_nodes.count == 0 )
				return end ();
			return iterator ( this, used_nodes.last );
			}
		iterator push ( const value_type &new_value )
			{
			if ( free_nodes.count == 0 )
				{
				reserve ( capacity + 10 );
				}
			size_t index = get_node_from_list ( free_nodes );
			add_node_to_list ( used_nodes, index );
			elements[index].data = new_value;
			return iterator ( this, index );
			}
		iterator iterator_for_index ( const size_t index ) const
			{
			if ( used_nodes.count == 0 )
				return end ();
			return iterator ( this, index );
			}
		bool is_used ( const size_t index )
			{
			if ( used_nodes.count == 0 )
				return false;
			return is_node_in_list ( used_nodes, index );
			}
		void clear ( void )
			{
			if ( capacity == 0 )
				return;
			intptr_t index = used_nodes.first;
			while ( index != -1 )
				{
				intptr_t next = elements[index].next;
				elements[index].~list_node ();
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
		void debug ( void ( *func ) ( const intptr_t, const value_type & ) )
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
		void debug ( void )
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
	};
}

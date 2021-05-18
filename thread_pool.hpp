#pragma once
#include "thread.hpp"
#include "mutex.hpp"
#include "semaphore.hpp"
#include "list.hpp"

namespace cpplib
{
class thread_pool
    {
        friend class thread_pool_worker;
    public:
        typedef struct task
            {
            uint32_t ( *function ) ( void * );
            void *parameters;
            uint32_t result;
            } task;
    private:
        typedef struct task_holder
            {
            task *pointer;
            task copy;
            task_holder ( void ) : pointer ( nullptr ) {}
            task_holder ( task *new_pointer ) : pointer ( new_pointer ) {}
            task_holder ( uint32_t ( *new_function ) ( void * ), void *new_parameters ) : pointer ( nullptr ), copy{ new_function, new_parameters, 0 } {}
            } task_holder;
        thread *thread_array;
        size_t thread_count;
        bool running;

        list <task_holder> task_list;
        mutex task_list_mutex;
        semaphore new_task_semaphore;

    public:
        thread_pool ( void );
        ~thread_pool ( void );
        bool initialize ( void );
        bool shutdown ( void );
        uint32_t get_thread_count ( void );
        void add_task ( uint32_t ( *function ) ( void * ), void *parameters );
        void clear_tasks ( void );
    };
}

/**
 * @file tpl_os_kernel.c
 *
 * @section descr File description
 *
 * Trampoline kernel structures and functions. These functions are used
 * internally by trampoline and should not be used directly.
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 * Copyright ESEO for function and data structures documentation
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 *
 */

#include "tpl_os_kernel.h"
#include "tpl_os_definitions.h"
#include "tpl_os_hooks.h"
#include "tpl_os_alarm_kernel.h"
#include "tpl_machine.h"
#include "tpl_machine_interface.h"
#include "tpl_dow.h"


#define OS_START_SEC_CODE
#include "tpl_memmap.h"

static /*@null@*/ tpl_exec_common *tpl_get_exec_object(void);

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * idle_task is the task descriptor of the kernel task
 * used when no other task is ready. The OS starts
 * by setting this task as the running task (ie the
 * init code currently being run.
 * It then calls tpl_schedule to start the
 * multitasking and falls back in an infinite loop.
 */

/**
 * @internal
 *
 * static part of the idle task descriptor
 *
 * @see #idle_task
 */
static tpl_exec_static idle_task_static = {
    /* context              */  IDLE_CONTEXT,
    /* no stack             */  IDLE_STACK,
    /* no entry point       */  NULL,
    /* internal resource    */  NULL,
    /* id is INVALID_TASK   */  INVALID_TASK,
    /* base priority is 0   */  0,
    /* max activate count   */  1,
    /* type is BASIC        */  TASK_BASIC
};

/**
 * @internal
 *
 * idle task descriptor
 */
static tpl_task idle_task = {
    /*  Common members  */
    {
    /* static descriptor    */  &idle_task_static,
    /* resources            */  NULL,
    /* activation count     */  0,
    /* priority             */  0,
    /* state                */  RUNNING
    },
    /* task members */
    /* event set            */  0,
    /* event wait           */  0
};

/*  MISRA RULE 45 VIOLATION: the original pointer points to a struct
    that has the same beginning fields as the struct it is casted to
    This allow object oriented design and polymorphism.
*/

/**
 * @internal 
 *
 * tpl_running_obj is the currently running task in the application.
 *
 * At system startup it is set to the idle task
 */
tpl_exec_common *tpl_running_obj = (tpl_exec_common *)&idle_task;

/**
 * @internal
 *
 * tpl_ready_list is a table that is automatically generated by goil from
 * the application description. Indexes of this table are the priority levels
 * Each of its elements is a pointer to the fifo used to store task/isr2
 * activation instances and the size of this fifo.
 */
extern tpl_priority_level tpl_ready_list[];

/**
 * @internal
 *
 * tpl_fifo_rw is a table that is automatically generated by goil from
 * the application description. Indexes of this table are the priority levels
 * each of its elements are a read and write index in the fifo.
 */
extern tpl_fifo_state tpl_fifo_rw[];

/**
 * @internal
 *
 * tpl_h_prio is the index of the highest priority non empty fifo
 * in the ready list. It is used to speed up scheduling
 */
s8 tpl_h_prio = -1;

/*
 * tpl_last_result store the last error
 * encountered by Trampoline.
 * It is used has a return value
 * for some OS services and for the
 * ErrorHook routine
 */
/* tpl_status tpl_last_result = E_OK; */

u8 tpl_os_state = OS_INIT; /* see doc in header file declaration */

tpl_resource_id RES_SCHEDULER = RESOURCE_COUNT; /* see doc in header declaration */

/**
 * The scheduler resource descriptor
 *
 * @see #RES_SCHEDULER
 */
tpl_resource res_sched = {
    RES_SCHEDULER_PRIORITY, /**< the ceiling priority is defined as the
                                 maximum priority of the tasks of the
                                 application                                */
    0,                      /*   owner_prev_priority                        */
    NULL,                   /*   owner                                      */
    NULL                    /*   next_res                                   */
};

/**
 * INTERNAL_RES_SCHEDULER is an internal resource with the higher task priority
 * in the application. A task is non preemptable when INTERNAL_RES_SCHEDULER
 * is set as internal resource.
 */
tpl_internal_resource INTERNAL_RES_SCHEDULER = {
    RES_SCHEDULER_PRIORITY, /**< the ceiling priority is defined as the
                                 maximum priority of the tasks of the
                                 application                                */
    0,
    NULL
};

#ifdef WITH_DOW
/*
 */
void printrl(char *msg)
{
    int i,j;
    tpl_exec_common **level;
    int size;
    
    printf("%s - Highest: %d\n",msg,tpl_h_prio);
    for (i = 0; i < PRIO_LEVEL_COUNT; i++)
    {
        printf("P%2d: ",i);
        level = tpl_ready_list[i].fifo;
        size = tpl_fifo_rw[i].size;
        for (j = 0; j < tpl_fifo_rw[i].size; j++)
        {
            int idx = tpl_fifo_rw[i].read + j;
            if (idx >= tpl_ready_list[i].size)
            {
                idx -= tpl_ready_list[i].size;
            }
            assert(level[idx]->priority == i);
            printf(" %d",level[idx]->static_desc->id);
        }
        printf("\n");
    }
}
#endif

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CODE
#include "tpl_memmap.h"

/**
 * @internal
 *
 * tpl_get_exec_object extracts the highest priority ready executable
 * object from the executable objects list and returns it.
 * tpl_get_exec_object returns NULL if no ready executable object
 * is available
 *
 * @return highest priority executable object descriptor
 */
static /*@null@*/ tpl_exec_common *tpl_get_exec_object(void)
{
    tpl_exec_common *elected = NULL ;
    tpl_exec_common **highest;
    u8              read_idx;
    
    DOW_ASSERT((tpl_h_prio >= 0) && (tpl_h_prio < PRIO_LEVEL_COUNT))
    DOW_ASSERT(tpl_fifo_rw[tpl_h_prio].size > 0)
    
    DOW_DO(printrl("tpl_get_exec_object - avant");)
    
    /*  Get the highest priority non empty fifo                         */
    highest = tpl_ready_list[tpl_h_prio].fifo;
    
    /*  Get the read index                                              */
    read_idx = tpl_fifo_rw[tpl_h_prio].read;
    
    /*  The fifo is not empty, get the descriptor                       */
    elected = highest[read_idx];
    
    /*  Adjust the read index and the size                              */
    read_idx++;
    if (read_idx >= tpl_ready_list[tpl_h_prio].size)
    {
        read_idx = 0;
    }
    tpl_fifo_rw[tpl_h_prio].read = read_idx;
    tpl_fifo_rw[tpl_h_prio].size--;
    
    /*  Adjust the highest priority non empty fifo index                */
    while ((tpl_h_prio >= 0) && (tpl_fifo_rw[tpl_h_prio].size == 0))
    {
        tpl_h_prio--;
    }

    DOW_DO(printrl("tpl_get_exec_object - apres");)
    
    return elected;
}

/**
 * @internal
 *
 * tpl_put_exec_object put an executable object
 * in the ready executable object list
 *
 * @param exec_obj address of the executable object descriptor
 * @param kind can be one of #PREEMPTED_EXEC_OBJ or #NEWLY_ACTIVATED_EXEC_OBJ
 */
void tpl_put_exec_object(
    tpl_exec_common *exec_obj,
    const u8        kind)
{
    /*  Get the current priority of the executable object                   */
    tpl_priority    prio;
    tpl_exec_common **fifo;
    u8              write_idx;
    
    DOW_ASSERT((kind == PREEMPTED_EXEC_OBJ) ||
               (kind == NEWLY_ACTIVATED_EXEC_OBJ))
    
    DOW_DO(printrl("tpl_put_exec_object - avant");)

    if (kind == NEWLY_ACTIVATED_EXEC_OBJ)
    {
        /*  the priority used as level in the ready list
            for a newly activated object is the base priority               */
        prio = exec_obj->static_desc->base_priority ;
        
        DOW_ASSERT((prio >= 0) && (prio < PRIO_LEVEL_COUNT))
        DOW_ASSERT(tpl_fifo_rw[prio].size < tpl_ready_list[prio].size)
    
        /*  a newly activated executable object
            is put at the end of the fifo                                   */
        write_idx = tpl_fifo_rw[prio].read + tpl_fifo_rw[prio].size;
        
        /*  adjust the write index                                          */
        if (write_idx >= tpl_ready_list[prio].size) {
             write_idx -= tpl_ready_list[prio].size;
        }
        
    }
    else {
        /*  the priority used as level in the ready list
            for a preempted object is the current priority                  */
        prio = exec_obj->priority;
        
        DOW_ASSERT((prio >= 0) && (prio < PRIO_LEVEL_COUNT))
        DOW_ASSERT(tpl_fifo_rw[prio].size < tpl_ready_list[prio].size)

        /*  a preempted executable object is put at the head of the fifo    */
        write_idx = tpl_fifo_rw[prio].read - 1 ;
        
        /*  since write_idx is unsigned, an overflow could occur            */
        if (write_idx >= tpl_ready_list[prio].size) {
            write_idx = tpl_ready_list[prio].size - 1;
        }
        tpl_fifo_rw[prio].read = write_idx;
    }
    
    /*  Get the corresponding fifo                                          */
    fifo = tpl_ready_list[prio].fifo;
    
    fifo[write_idx] = exec_obj ;
    
    /* adjust the size                                                      */
    tpl_fifo_rw[prio].size++;
    
    /* adjust the highest priority non empty fifo                           */
    if (prio > tpl_h_prio) {
        tpl_h_prio = prio;
    }
    
    DOW_DO(printrl("tpl_put_exec_object - apres");)
}

/**
 * @internal
 *
 * Get an internal resource
 *
 * @param task task from which internal resource is got
 */
void tpl_get_internal_resource(tpl_exec_common *a_task)
{
    tpl_internal_resource *rez = a_task->static_desc->internal_resource;
    
    if ((rez != NULL) && (rez->taken == FALSE))
    {
        rez->taken = TRUE;
        rez->owner_prev_priority = a_task->priority;
        a_task->priority = rez->ceiling_priority;
    }
}

/**
 * @internal
 *
 * Release an internal resource
 *
 * @param task task from which internal resource is released
 */
void tpl_release_internal_resource(tpl_exec_common *a_task)
{
    tpl_internal_resource *rez = a_task->static_desc->internal_resource;

    if ((rez != NULL) && (rez->taken == TRUE))
    {
        rez->taken = FALSE;
        a_task->priority = rez->owner_prev_priority;
    }
}

/**
 * @internal
 *
 * Does the scheduling
 *
 * This function is called by the OSEK/VDX Schedule service
 * and by various function when a rescheduling is needed
 * 
 * @param from can be one of #FROM_TASK_LEVEL or #FROM_IT_LEVEL
 */
void tpl_schedule(const u8 from)
{
    tpl_exec_common *old_running_obj;
    
    /*  the tpl_running_obj is never NULL and may be in 3 states
        - RUNNING:      if the running object is in the RUNNING state and
                        loses the CPU because a higher priority task is in
                        the ready list, its context must be saved.
        - WAITING:      if the running object is in the WAITING state, its
                        context must be saved and it loses the CPU.
        - RESURRECT:    if the running object is in the RESURRECT state,
                        its context is not saved, it is put as
                        READY_AND_NEW in the ready list.
        - DYING:        if the running object is in the DYING state, its
                        context is not saved and it loses the CPU.          */
    tpl_exec_state state = tpl_running_obj->state;

    tpl_bool schedule =
        (tpl_h_prio != -1) &&
        (((state & 0x3) == 0x3) ||    /*  DYING, WAITING or RESURRECT */
         (state == (tpl_exec_state)RUNNING &&
          tpl_h_prio > tpl_running_obj->priority));
    
    if (schedule == TRUE)
    {
        /*  save the old running task for context switching */
        old_running_obj = tpl_running_obj;

        /*  a task switch will occur. It is time to call the
            PostTaskHook while the soon descheduled task is running     */
        CALL_POST_TASK_HOOK()
            
        if (state == RUNNING)
        {
            /*  the current running task become READY                   */
            tpl_running_obj->state = (tpl_exec_state)READY;
            /*  put the running task in the ready task list             */
            /*  Bug fix. preempted objects are put at the head
                of the set while newly activated objects are
                put at the end of the set. So we have to
                distinguish them                                        */
            tpl_put_exec_object(tpl_running_obj, PREEMPTED_EXEC_OBJ);
        }
        else
        {
            /*  the task loses the CPU because it has been put in the
                WAITING or in the DYING state, its internal resource
                is released.                                            */
            tpl_release_internal_resource(tpl_running_obj);
            
            if (state == (tpl_exec_state)DYING)
            {
                /*  if the running object is dying, the activate count
                    is decreased                                        */
                tpl_running_obj->activate_count--;
                
                /*  and checked to compute its state.                   */
                if (tpl_running_obj->activate_count > 0)
                {
                    /*  there is at least one instance of the dying
                        running object in the ready list. So it is put
                        in the READY_AND_NEW state. This way when the
                        next instance will be prepared to run it will
                        be initialized.                                 */
                    tpl_running_obj->state = READY_AND_NEW;
                }
                else
                {
                    /*  there is no instance of the dying running
                        object in the ready list. So it is put in the
                        SUSPENDED state.                                */
                    tpl_running_obj->state = SUSPENDED;
                }
            }
            
            if (state == (tpl_exec_state)RESURRECT)
            {
                /*  This case happens when a task chains to itself
                    by calling ChainTask                                */
                tpl_running_obj->state = READY_AND_NEW;
                tpl_put_exec_object(tpl_running_obj, NEWLY_ACTIVATED_EXEC_OBJ);
            }
        }

        /*  get the ready task from the ready task list                 */
        tpl_running_obj = tpl_get_exec_object();

        if (tpl_running_obj->state == READY_AND_NEW)
        {
            /*  the object has not be preempted. So its
                descriptor must be initialized              */
            tpl_init_exec_object(tpl_running_obj);
        }
        /*  the inserted task become RUNNING                */
        tpl_running_obj->state = RUNNING;
        /*  If an internal resource is assigned to the task
            and it is not already taken by it, take it      */
        tpl_get_internal_resource(tpl_running_obj); 
        /*  A new task has been elected
            It is time to call PreTaskHook while the
            rescheduled task is running                     */
        CALL_PRE_TASK_HOOK()
    
        if (tpl_running_obj == (tpl_exec_common *)&idle_task)
        {
            tpl_os_state = OS_IDLE;
        }
        else if (tpl_running_obj->static_desc->type == IS_ROUTINE)
        {
            tpl_os_state = OS_ISR2;
        }
        else
        {
            tpl_os_state = OS_TASK;
        }
        /*  Switch the context  */
        if (from == (u8)FROM_TASK_LEVEL)
        {
            if (state < 0x4)
            {
                DOW_ASSERT(tpl_running_obj != old_running_obj)
                /*  The old running object is not in a DYING or
                    RESSURECT state */
                tpl_switch_context(
                    &(old_running_obj->static_desc->context),
                    &(tpl_running_obj->static_desc->context));
            }
            else
            {
                tpl_switch_context(
                    NULL,
                    &(tpl_running_obj->static_desc->context));
            }
        }
        else
        { /* FROM_IT_LEVEL */
            if (state < 0x4)
            {
                /*  The old running object is not in a DYING or
                    RESSURECT state */
                tpl_switch_context_from_it(
                    &(old_running_obj->static_desc->context),
                    &(tpl_running_obj->static_desc->context));
            }
            else
            {
                tpl_switch_context_from_it(
                    NULL,
                    &(tpl_running_obj->static_desc->context));
            }
        }
    }
}

/**
 * @internal
 *
 * This function is called by OSEK/VDX ActivateTask and by
 * the raise of an alarm.
 *
 * the activation count is incremented
 * if the task is in the SUSPENDED state, it is moved
 * to the task list
 *
 * @param task reference of the task's identifier
 */
tpl_status tpl_activate_task(tpl_task *a_task)
{
    tpl_status              result = E_OS_LIMIT;
    tpl_activate_counter    count = a_task->exec_desc.activate_count;

    if (count < a_task->exec_desc.static_desc->max_activate_count)
    {
        if (count == 0)
        {
            if (a_task->exec_desc.static_desc->type == TASK_EXTENDED)
            {
                /*  if the task is an extended one, it is inited now        */
                a_task->exec_desc.state = (tpl_exec_state)READY;
                tpl_init_exec_object(&a_task->exec_desc);
            }
            else
            {
                /*  if it is a basic task, its initialization
                    is postponed to the time it will get the CPU            */
                a_task->exec_desc.state = (tpl_exec_state)READY_AND_NEW;
            }
        }
        /*  put it in the list  */
        tpl_put_exec_object(
            &(a_task->exec_desc),
            (u8)NEWLY_ACTIVATED_EXEC_OBJ
        );
        /*  inc the task activation count. When the task will terminate
            it will dec this count and if not zero it will be reactivated   */
        a_task->exec_desc.activate_count++;

        result = (tpl_status)E_OK_AND_SCHEDULE;
    }

    return result;
}

/**
 * @internal
 * 
 * This function is used by SetEvent and by tpl_raise_alarm
 * 
 * @param task              Pointer to the task descriptor
 * @param incoming_event    Event mask
 */
tpl_status tpl_set_event(
    tpl_task                *a_task,
    const tpl_event_mask    incoming_event)
{
    tpl_status result = E_OK;
    
    if (a_task->exec_desc.state != (tpl_exec_state)SUSPENDED)
    {
        /*  merge the incoming event mask with the old one  */
        a_task->evt_set = (tpl_event_mask)(a_task->evt_set | incoming_event);
        /*  cross check the event the task is
            waiting for and the incoming event              */
        if ((a_task->evt_wait & incoming_event) != 0)
        {
            /*  the task was waiting for at least one of the event set
                the wait mask is reset to 0                 */
            a_task->evt_wait = (tpl_event_mask)0;
            /*  anyway check it is in the WAITING state     */
            if (a_task->exec_desc.state == (tpl_exec_state)WAITING)
            {
                /*  set the state to READY  */
                a_task->exec_desc.state = (tpl_exec_state)READY;
                /*  put the task in the READY list          */

                /*  MISRA RULE 45 VIOLATION: the original pointer points to
                    a struct that has the same beginning fields as the struct
                    it is casted to. This allow object oriented design and
                    polymorphism.
                */
                tpl_put_exec_object(
                    (tpl_exec_common *)a_task,
                    (u8)NEWLY_ACTIVATED_EXEC_OBJ);
                /*  notify a scheduling needs to be done    */
                result = (tpl_status)E_OK_AND_SCHEDULE;
            }
        }
    }
    else
    {
        result = E_OS_STATE;
    }
    
    return result;
}

/**
 * @internal
 *
 * Executable object initialization.
 *
 * This function initialize the common part of task
 * or category 2 interrupt service routine to make them ready
 * for execution. If the object is an task it initializes
 * the event masks too (this has no effect on basic tasks).
 *
 * @param exec_obj address of the executable object descriptor
 */
void tpl_init_exec_object(tpl_exec_common *exec_obj)
{
    /*  The priority is set to the base priority of the executable object    */
    exec_obj->priority = exec_obj->static_desc->base_priority;
    /*  set the resources list to NULL   */
    exec_obj->resources = NULL;
    /*  context init is machine dependant
        tpl_init_context is defined in tpl_machine.c    */
    tpl_init_context(exec_obj);
    /*  if the object is a task, init the events    */
    if ((exec_obj->static_desc->type & IS_ROUTINE) == 0)
    {
        /*  MISRA RULE 45 VIOLATION: the original pointer points to a struct
            that has the same beginning fields as the struct it is casted to
            This allow object oriented design and polymorphism.
        */
        ((tpl_task *)exec_obj)->evt_set = ((tpl_task *)exec_obj)->evt_wait = 0;
    }
}

/**
 * @internal 
 *
 * Initialization of Trampoline
 */
void tpl_init_os(const tpl_application_mode app_mode)
{
    u16         i;
#ifndef NO_ALARM
    tpl_time_obj   *auto_time_obj;
#endif

#ifndef NO_TASK
    tpl_task    *auto_task;
    
    /*  Look for autostart tasks    */
        
    for (i = 0; i < TASK_COUNT; i++)
    {
        auto_task = tpl_task_table[i];
        if (auto_task->exec_desc.state == (tpl_exec_state)AUTOSTART)
        {
            /*  each AUTOSTART task is activated   */
            tpl_activate_task(auto_task);
        }
    }  
#endif
#ifndef NO_ALARM
 
    /*  Look for autostart alarms    */
        
    for (i = 0; i < ALARM_COUNT; i++)
    {
        auto_time_obj = tpl_alarm_table[i];
        if (auto_time_obj->state == (tpl_time_obj_state)ALARM_AUTOSTART)
        {
            auto_time_obj->state = ALARM_SLEEP;
			tpl_insert_time_obj(auto_time_obj);
        }
    }  
	
#endif
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/* End of file tpl_os_kernel.c */

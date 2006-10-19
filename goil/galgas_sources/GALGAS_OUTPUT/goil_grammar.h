//---------------------------------------------------------------------------*
//                                                                           *
//                          File 'goil_grammar.h'                            *
//         Generated by version GALGAS_BETA_VERSION (LL(1) grammar)          *
//                     october 19th, 2006, at 8h53'19"                       *
//                                                                           *
//---------------------------------------------------------------------------*

//--- START OF USER ZONE 1


//--- END OF USER ZONE 1

#ifndef GRAMMAR_goil_grammar_DEFINED
#define GRAMMAR_goil_grammar_DEFINED

//---------------------------------------------------------------------------*

#include "goil_syntax.h"
#include "goil_obj_task.h"
#include "goil_obj_counter.h"
#include "goil_obj_appmode.h"
#include "goil_obj_alarm.h"
#include "goil_obj_resource.h"
#include "goil_obj_event.h"
#include "goil_obj_isr.h"
#include "goil_obj_message.h"
#include "goil_obj_networkmessage.h"
#include "goil_obj_com.h"
#include "goil_obj_ipdu.h"

//--- START OF USER ZONE 2


//--- END OF USER ZONE 2

//---------------------------------------------------------------------------*

class goil_grammar : public goil_syntax,
                                 public goil_obj_task,
                                 public goil_obj_counter,
                                 public goil_obj_appmode,
                                 public goil_obj_alarm,
                                 public goil_obj_resource,
                                 public goil_obj_event,
                                 public goil_obj_isr,
                                 public goil_obj_message,
                                 public goil_obj_networkmessage,
                                 public goil_obj_com,
                                 public goil_obj_ipdu {
  public : virtual void nt_file_ (goil_lexique &) ;
  public : void startParsing_ (goil_lexique &) ;
  public : virtual void nt_OIL_version_ (goil_lexique &,
                                GGS_lstring &,
                                GGS_lstring &) ;
  public : virtual void nt_description_ (goil_lexique &,
                                GGS_lstring &) ;
  public : virtual void nt_implementation_definition_ (goil_lexique &,
                                GGS_implementation &) ;
  public : virtual void nt_implementation_spec_list_ (goil_lexique &,
                                GGS_implementation &) ;
  public : virtual void nt_implementation_spec_ (goil_lexique &,
                                GGS_implementation &) ;
  public : virtual void nt_implementation_list_ (goil_lexique &,
                                GGS_implementationSpec &) ;
  public : virtual void nt_impl_attr_def_ (goil_lexique &,
                                GGS_implementationSpec &) ;
  public : virtual void nt_auto_specifier_ (goil_lexique &,
                                GGS_bool &) ;
  public : virtual void nt_number_range_or_nil_ (goil_lexique &,
                                GGS_bool &,
                                GGS_number_set &) ;
  public : virtual void nt_number_range_ (goil_lexique &,
                                GGS_number_set &) ;
  public : virtual void nt_attribute_name_ (goil_lexique &,
                                GGS_lstring &) ;
  public : virtual void nt_multiple_specifier_ (goil_lexique &,
                                GGS_bool &) ;
  public : virtual void nt_default_number_ (goil_lexique &,
                                GGS_bool &,
                                GGS_luint &) ;
  public : virtual void nt_default_name_ (goil_lexique &,
                                GGS_bool &,
                                GGS_lstring &) ;
  public : virtual void nt_enumeration_ (goil_lexique &) ;
  public : virtual void nt_enumerator_ (goil_lexique &) ;
  public : virtual void nt_impl_parameter_list_ (goil_lexique &) ;
  public : virtual void nt_implementation_def_ (goil_lexique &) ;
  public : virtual void nt_application_definition_ (goil_lexique &) ;
  public : virtual void nt_object_definition_list_ (goil_lexique &) ;
  public : virtual void nt_nm_ (goil_lexique &) ;
  public : virtual void nt_boolean_ (goil_lexique &) ;
  public : virtual void nt_appmode_ (goil_lexique &) ;
  public : virtual void nt_task_ (goil_lexique &) ;
  public : virtual void nt_counter_ (goil_lexique &) ;
  public : virtual void nt_alarm_ (goil_lexique &) ;
  public : virtual void nt_resource_ (goil_lexique &) ;
  public : virtual void nt_event_ (goil_lexique &) ;
  public : virtual void nt_isr_ (goil_lexique &) ;
  public : virtual void nt_mess_ (goil_lexique &) ;
  public : virtual void nt_networkmess_ (goil_lexique &) ;
  public : virtual void nt_com_ (goil_lexique &) ;
  public : virtual void nt_ipdu_ (goil_lexique &) ;
  public : virtual void nt_task_parameters_ (goil_lexique &) ;
  public : virtual void nt_counter_parameters_ (goil_lexique &) ;
  public : virtual void nt_alarm_parameters_ (goil_lexique &) ;
  public : virtual void nt_alarm_action_params_ (goil_lexique &) ;
  public : virtual void nt_alarm_autostart_params_ (goil_lexique &) ;
  public : virtual void nt_isr_attributes_ (goil_lexique &) ;
  public : virtual void nt_message_parameters_ (goil_lexique &) ;
  public : virtual void nt_message_property_ (goil_lexique &) ;
  public : virtual void nt_notification_property_ (goil_lexique &) ;
  public : virtual void nt_ssi_attributes_ (goil_lexique &) ;
  public : virtual void nt_sse_attributes_ (goil_lexique &) ;
  public : virtual void nt_sde_attributes_ (goil_lexique &) ;
  public : virtual void nt_sze_attributes_ (goil_lexique &) ;
  public : virtual void nt_rzi_attributes_ (goil_lexique &) ;
  public : virtual void nt_rze_attributes_ (goil_lexique &) ;
  public : virtual void nt_rui_attributes_ (goil_lexique &) ;
  public : virtual void nt_rqi_attributes_ (goil_lexique &) ;
  public : virtual void nt_rue_attributes_ (goil_lexique &) ;
  public : virtual void nt_rqe_attributes_ (goil_lexique &) ;
  public : virtual void nt_rde_attributes_ (goil_lexique &) ;
  public : virtual void nt_rzs_attributes_ (goil_lexique &) ;
  public : virtual void nt_cdatatype_ (goil_lexique &) ;
  public : virtual void nt_transferproperty_ (goil_lexique &) ;
  public : virtual void nt_networkmessage_ (goil_lexique &) ;
  public : virtual void nt_filter_ (goil_lexique &) ;
  public : virtual void nt_mask_x_attribute_ (goil_lexique &) ;
  public : virtual void nt_mask_attribute_ (goil_lexique &) ;
  public : virtual void nt_min_max_attribute_ (goil_lexique &) ;
  public : virtual void nt_period_offset_attribute_ (goil_lexique &) ;
  public : virtual void nt_networkordercallout_ (goil_lexique &) ;
  public : virtual void nt_cpuordercallout_ (goil_lexique &) ;
  public : virtual void nt_initialvalue_ (goil_lexique &) ;
  public : virtual void nt_sendingmessage_ (goil_lexique &) ;
  public : virtual void nt_queuesize_ (goil_lexique &) ;
  public : virtual void nt_link_ (goil_lexique &) ;
  public : virtual void nt_ipdu_ref_ (goil_lexique &) ;
  public : virtual void nt_net_messageproperty_ (goil_lexique &) ;
  public : virtual void nt_static_mp_ (goil_lexique &) ;
  public : virtual void nt_dynamic_mp_ (goil_lexique &) ;
  public : virtual void nt_sizeinbits_ (goil_lexique &) ;
  public : virtual void nt_maxsizeinbits_ (goil_lexique &) ;
  public : virtual void nt_bitordering_ (goil_lexique &) ;
  public : virtual void nt_bitposition_ (goil_lexique &) ;
  public : virtual void nt_datainterpretation_ (goil_lexique &) ;
  public : virtual void nt_direction_ (goil_lexique &) ;
  public : virtual void nt_comtimebase_ (goil_lexique &) ;
  public : virtual void nt_comerrorhook_ (goil_lexique &) ;
  public : virtual void nt_comusegetserviceid_ (goil_lexique &) ;
  public : virtual void nt_comuseparameteraccess_ (goil_lexique &) ;
  public : virtual void nt_comstartcomextension_ (goil_lexique &) ;
  public : virtual void nt_comappmode_ (goil_lexique &) ;
  public : virtual void nt_comstatus_ (goil_lexique &) ;
  public : virtual void nt_use_ (goil_lexique &) ;
  public : virtual void nt_ipduproperty_ (goil_lexique &) ;
  public : virtual void nt_transmissionmode_ (goil_lexique &) ;
  public : virtual void nt_timeperiod_ (goil_lexique &) ;
  public : virtual void nt_timeoffset_ (goil_lexique &) ;
  public : virtual void nt_minimumdelaytime_ (goil_lexique &) ;
  public : virtual void nt_timeout_ (goil_lexique &) ;
  public : virtual void nt_firsttimeout_ (goil_lexique &) ;
  public : virtual void nt_ipducallout_ (goil_lexique &) ;
  public : virtual void nt_layerused_ (goil_lexique &) ;
  public : virtual void nt_sent_attributes_ (goil_lexique &) ;
  public : virtual void nt_received_attributes_ (goil_lexique &) ;
  public : virtual void nt_direct_attributes_ (goil_lexique &) ;
  public : virtual void nt_periodic_attributes_ (goil_lexique &) ;
  public : virtual void nt_mixed_attributes_ (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_0 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_syntax_1 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_2 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_syntax_3 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_4 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_5 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_6 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_7 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_8 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_syntax_9 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_10 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_11 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_12 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_syntax_13 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_14 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_syntax_15 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_syntax_16 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_syntax_17 (goil_lexique &) ;
  public : virtual sint16 select_goil_syntax_18 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_task_0 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_task_1 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_task_2 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_task_3 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_task_4 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_counter_0 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_counter_1 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_counter_2 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_appmode_0 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_appmode_1 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_alarm_0 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_alarm_1 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_alarm_2 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_alarm_3 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_alarm_4 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_alarm_5 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_alarm_6 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_resource_0 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_resource_1 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_event_0 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_isr_0 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_0 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_1 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_2 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_3 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_4 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_5 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_6 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_7 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_8 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_9 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_10 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_11 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_12 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_13 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_14 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_15 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_16 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_17 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_18 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_19 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_20 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_21 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_22 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_23 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_message_24 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_message_25 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_networkmessage_0 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_networkmessage_1 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_networkmessage_2 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_networkmessage_3 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_networkmessage_4 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_networkmessage_5 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_networkmessage_6 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_networkmessage_7 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_com_0 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_com_1 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_ipdu_0 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_ipdu_1 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_ipdu_2 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_ipdu_3 (goil_lexique &) ;
  public : virtual sint16 select_goil_obj_ipdu_4 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_ipdu_5 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_ipdu_6 (goil_lexique &) ;
  public : virtual sint16 select_repeat_goil_obj_ipdu_7 (goil_lexique &) ;
} ;

//---------------------------------------------------------------------------*

#endif
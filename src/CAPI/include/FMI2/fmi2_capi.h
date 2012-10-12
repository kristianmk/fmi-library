/*
    Copyright (C) 2012 Modelon AB

    This program is free software: you can redistribute it and/or modify
    it under the terms of the BSD style license.

     This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    FMILIB_License.txt file for more details.

    You should have received a copy of the FMILIB_License.txt file
    along with this program. If not, contact Modelon AB <http://www.modelon.com>.
*/


#ifndef FMI2_CAPI_H_
#define FMI2_CAPI_H_

#include <FMI2/fmi2_types.h>
#include <FMI2/fmi2_functions.h>
#include <FMI2/fmi2_enums.h>
#include <JM/jm_portability.h>
#include <JM/jm_callbacks.h>

typedef struct fmi2_capi_t fmi2_capi_t;

#ifdef __cplusplus 
extern "C" {
#endif

/** \file fmi2_capi.h
	\brief Public interfaces for the FMI CAPI library. 
	*/

/** \addtogroup fmi2_capi Standard FMI 1.0 "C" API
 * \brief The "C" API loads and frees the FMI functions and it is through theise functions all the communcation with the FMU occures. The FMI import library wrappes these functions in a more convenient way.
 *  @{
 */

/**	\addtogroup fmi2_capi_const_destroy FMI 1.0 Utility functions
 *		\brief Utility functions used to load and free the FMI functions.
 *	\addtogroup fmi2_capi_me FMI 1.0 (ME) Model Exchange functions
 *		\brief List of Model Exchange wrapper functions. Common functions are not listed.
 *	\addtogroup fmi2_capi_cs FMI 1.0 (CS) Co-Simulation functions 
 *		\brief List of Co-Simulation wrapper functions. Common functions are not listed.
 *	\addtogroup fmi2_capi_common FMI 1.0 (ME & CS) Common functions
 *		\brief List of wrapper functions that are in common for both Model Exchange and Co-Simulation.
 */


/** \addtogroup fmi2_capi_const_destroy
 *  @{
 */

/**
 * \brief Free a C-API struct. All memory allocated since the struct was created is freed.
 * 
 * @param fmu A model description object returned by fmi2_import_allocate.
 */
void fmi2_capi_destroy_dllfmu(fmi2_capi_t* fmu);

/**
 * \brief Create a C-API struct. The C-API struct is a placeholder for the FMI DLL functions.
 * 
 * @param callbacks ::jm_callbacks used to construct library objects.
 * @param dllPath Full path to the FMU shared library.
 * @param modelIdentifier The model indentifier.
 * @param standard FMI standard that the function should load.
 * @param callBackFunctions callbacks passed to the FMU.
 * @return Error status. If the function returns with an error, it is not allowed to call any of the other C-API functions.
 */
fmi2_capi_t* fmi2_capi_create_dllfmu(jm_callbacks* callbacks, const char* dllPath, const char* modelIdentifier, const fmi2_callback_functions_t* callBackFunctions, fmi2_fmu_kind_enu_t standard);

/**
 * \brief Loads the FMI functions from the shared library. The shared library must be loaded before this function can be called, see fmi2_import_load_dll.
 * 
 * @param fmu A model description object returned by fmi2_import_allocate.
 * @param capabilities An array of capability flags according to fmi2_capabilities_enu_t order.
 * @return Error status. If the function returns with an error, no other C-API functions than fmi2_import_free_dll and fmi2_import_destroy_dllfmu are allowed to be called.
 */
jm_status_enu_t fmi2_capi_load_fcn(fmi2_capi_t* fmu, unsigned int capabilities[]);

/**
 * \brief Loads the FMU�s shared library. The shared library functions are not loaded in this call, see fmi2_import_load_fcn.
 * 
 * @param fmu A model description object returned by fmi2_import_allocate.
 * @return Error status. If the function returns with an error, no other C-API functions than fmi2_import_destroy_dllfmu are allowed to be called.
 */
jm_status_enu_t fmi2_capi_load_dll(fmi2_capi_t* fmu);

/**
 * \brief Frees the handle to the FMU�s shared library. After this function returnes, no other C-API functions than fmi2_import_destroy_dllfmu and fmi2_import_load_dll are allowed to be called.
 * 
 * @param fmu A model description object returned by fmi2_import_allocate that has loaded the FMU�s shared library, see fmi2_import_load_dll.
 * @return Error status.
 */
jm_status_enu_t fmi2_capi_free_dll(fmi2_capi_t* fmu);

/**
 * \brief Set CAPI debug mode flag. Setting to non-zero prevents DLL unloading in fmi1_capi_free_dll
 *  while all the memory is deallocated. This is to support valgrind debugging. 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param mode The debug mode to set.
 */
void fmi2_capi_set_debug_mode(fmi2_capi_t* fmu, int mode);

/**
 * \brief Get CAPI debug mode flag that was set with fmi1_capi_set_debug_mode()
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function. */
int fmi2_capi_get_debug_mode(fmi2_capi_t* fmu);

/**@} */

/** \addtogroup fmi2_capi_common
 *  @{
 */

/**
 * \brief Calls the FMI function fmiGetVersion() 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @return FMI version.
 */
const char* fmi2_capi_get_version(fmi2_capi_t* fmu);

/**
 * \brief Calls the FMI function fmiSetDebugLogging(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param loggingOn Enable or disable the debug logger.
 * @param nCategories Number of categories to log.
 * @param categories Which categories to log.
 *
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_set_debug_logging(fmi2_capi_t* fmu, fmi2_boolean_t loggingOn, size_t nCategories, fmi2_string_t categories[]);

/**
 * \brief Calls the FMI function fmiSetReal(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param value Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_set_real(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, const fmi2_real_t    value[]);

/**
 * \brief Calls the FMI function fmiSetInteger(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param value Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_set_integer(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, const fmi2_integer_t value[]);

/**
 * \brief Calls the FMI function fmiSetBoolean(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param value Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_set_boolean(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, const fmi2_boolean_t value[]);

/**
 * \brief Calls the FMI function fmiSetString(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param value Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_set_string(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, const fmi2_string_t  value[]);

/**
 * \brief Calls the FMI function fmiGetReal(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param value (Output)Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_real(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, fmi2_real_t    value[]);

/**
 * \brief Calls the FMI function fmiGetInteger(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param value (Output)Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_integer(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, fmi2_integer_t value[]);

/**
 * \brief Calls the FMI function fmiGetBoolean(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param value (Output)Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_boolean(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, fmi2_boolean_t value[]);

/**
 * \brief Calls the FMI function fmiGetString(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param value (Output)Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_string(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, fmi2_string_t  value[]);


/**
 * \brief Calls the FMI function fmiGetTypesPlatform(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @return The platform the FMU was compiled for.
 */
const char* fmi2_capi_get_types_platform(fmi2_capi_t* fmu);

fmi2_status_t fmi2_capi_get_fmu_state           (fmi2_capi_t* fmu, fmi2_FMU_state_t* s);
fmi2_status_t fmi2_capi_set_fmu_state           (fmi2_capi_t* fmu, fmi2_FMU_state_t s);
fmi2_status_t fmi2_capi_free_fmu_state          (fmi2_capi_t* fmu, fmi2_FMU_state_t* s);
fmi2_status_t fmi2_capi_serialized_fmu_state_size(fmi2_capi_t* fmu, fmi2_FMU_state_t s, size_t* sz);
fmi2_status_t fmi2_capi_serialize_fmu_state     (fmi2_capi_t* fmu, fmi2_FMU_state_t s , fmi2_byte_t data[], size_t sz);
fmi2_status_t fmi2_capi_de_serialize_fmu_state  (fmi2_capi_t* fmu, const fmi2_byte_t data[], size_t sz, fmi2_FMU_state_t* s);

/* Getting partial derivatives */
/*   typedef fmi2_status_t (*fmi2_get_partial_derivatives_ft)   (fmi2_component_t, fmi2_set_matrix_element_ft, void*, void*, void*, void*); */
fmi2_status_t fmi2_capi_get_directional_derivative(fmi2_capi_t* fmu, const fmi2_value_reference_t v_ref[], size_t nv,
                                                                   const fmi2_value_reference_t z_ref[], size_t nz,
                                                                   const fmi2_real_t dv[], fmi2_real_t dz[]);
/**@} */

/** \addtogroup fmi2_capi_me
 *  @{
 */


/**
 * \brief Calls the FMI function fmiInstantiateModel(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function. 
 * @param instanceName The name of the instance.
 * @param fmuGUID The GUID identifier.
 * @param fmuResourceLocation Access path to the FMU archive resources.
 * @param visible Indicates whether or not the simulator application window shoule be visible.
 * @param loggingOn Enable or disable the debug logger.
 * @return An instance of a model.
 */
fmi2_component_t fmi2_capi_instantiate_model(fmi2_capi_t* fmu, fmi2_string_t instanceName, fmi2_string_t fmuGUID, fmi2_string_t fmuResourceLocation, 
																 fmi2_boolean_t visible, fmi2_boolean_t loggingOn);
/**
 * \brief Calls the FMI function fmiFreeModelInstance(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 */
void fmi2_capi_free_model_instance(fmi2_capi_t* fmu);

/**
 * \brief Calls the FMI function fmiSetTime(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param time Set the current time.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_set_time(fmi2_capi_t* fmu, fmi2_real_t time);

/**
 * \brief Calls the FMI function fmiSetContinuousStates(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param x Array of state values.
 * @param nx Number of states.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_set_continuous_states(fmi2_capi_t* fmu, const fmi2_real_t x[], size_t nx);

/**
 * \brief Calls the FMI function fmiCompletedIntegratorStep(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param callEventUpdate (Output) Call fmiEventUpdate indicator.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_completed_integrator_step(fmi2_capi_t* fmu, fmi2_boolean_t* callEventUpdate);

/**
 * \brief Calls the FMI function fmiInitializeModel(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param toleranceControlled Enable or disable the use of relativeTolerance in the FMU.
 * @param relativeTolerance A relative tolerance used in the FMU.
 * @param eventInfo (Output) fmiEventInfo struct.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_initialize_model(fmi2_capi_t* fmu, fmi2_boolean_t toleranceControlled, fmi2_real_t relativeTolerance, fmi2_event_info_t* eventInfo);

/**
 * \brief Calls the FMI function fmiGetDerivatives(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param derivatives (Output) Array of the derivatives.
 * @param nx Number of derivatives.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_derivatives(fmi2_capi_t* fmu, fmi2_real_t derivatives[]    , size_t nx);

/**
 * \brief Calls the FMI function fmiGetEventIndicators(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param eventIndicators (Output) The event indicators.
 * @param ni Number of event indicators.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_event_indicators(fmi2_capi_t* fmu, fmi2_real_t eventIndicators[], size_t ni);

/**
 * \brief Calls the FMI function fmiEventUpdate(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param intermediateResults Indicate whether or not the fmiEventUpdate shall return after every internal event interation.
 * @param eventInfo (Output) An fmiEventInfo struct.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_eventUpdate(fmi2_capi_t* fmu, fmi2_boolean_t intermediateResults, fmi2_event_info_t* eventInfo);

/**
 * \brief Wrapper for the FMI function fmiCompletedEventIteration(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_completed_event_iteration(fmi2_capi_t* fmu);

/**
 * \brief Calls the FMI function fmiGetContinuousStates(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param states (Output) Array of state values.
 * @param nx Number of states.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_continuous_states(fmi2_capi_t* fmu, fmi2_real_t states[], size_t nx);

/**
 * \brief Calls the FMI function fmiGetNominalContinuousStates(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param x_nominal (Output) The nominal values.
 * @param nx Number of nominal values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_nominal_continuous_states(fmi2_capi_t* fmu, fmi2_real_t x_nominal[], size_t nx);

/**
 * \brief Calls the FMI function fmiGetStateValueReferences(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vrx (Output) The value-references of the states.
 * @param nx Number of value-references.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_state_value_references(fmi2_capi_t* fmu, fmi2_value_reference_t vrx[], size_t nx);

/**
 * \brief Calls the FMI function fmiTerminate(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_terminate(fmi2_capi_t* fmu);

/**@} */

/** \addtogroup fmi2_capi_cs
 *  @{
 */

/**
 * \brief Calls the FMI function fmiInstantiateSlave(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param instanceName The name of the instance.
 * @param fmuGUID The GUID identifier.
 * @param fmuResourceLocation Access path to the FMU archive resources.
 * @param visible Indicates whether or not the simulator application window shoule be visible.
 * @param loggingOn Enable or disable the debug logger.
 * @return An instance of a model.
 */
fmi2_component_t fmi2_capi_instantiate_slave(fmi2_capi_t* fmu, fmi2_string_t instanceName, fmi2_string_t fmuGUID, fmi2_string_t fmuResourceLocation, 
																 fmi2_boolean_t visible, fmi2_boolean_t loggingOn);
/**
 * \brief Calls the FMI function fmiInitializeSlave(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param relativeTolerance suggests a relative (local) tolerance in case the slave utilizes a numerical integrator with variable step size and error estimation.
 * @param tStart Start time of the simulation
 * @param StopTimeDefined Indicates whether or not the stop time is used.
 * @param tStop The stop time of the simulation.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_initialize_slave(fmi2_capi_t* fmu, fmi2_real_t  relativeTolerance, fmi2_real_t tStart, fmi2_boolean_t StopTimeDefined, fmi2_real_t tStop);

/**
 * \brief Calls the FMI function fmiTerminateSlave(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_terminate_slave(fmi2_capi_t* fmu);

/**
 * \brief Calls the FMI function fmiResetSlave(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_reset_slave(fmi2_capi_t* fmu);

/**
 * \brief Calls the FMI function fmiFreeSlaveInstance(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 */
void fmi2_capi_free_slave_instance(fmi2_capi_t* fmu);

/**
 * \brief Calls the FMI function fmiSetRealInputDerivatives(...) 
 * 
 * @param fmu C-API struct that has succesfully load the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param order	Array of derivative orders.
 * @param value Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_set_real_input_derivatives(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, const fmi2_integer_t order[], const  fmi2_real_t value[]);                                                  

/**
 * \brief Calls the FMI function fmiGetOutputDerivatives(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param vr Array of value references.
 * @param nvr Number of array elements.
 * @param order	Array of derivative orders.
 * @param value (Output) Array of variable values.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_real_output_derivatives(fmi2_capi_t* fmu, const fmi2_value_reference_t vr[], size_t nvr, const fmi2_integer_t order[], fmi2_real_t value[]);                                              

/**
 * \brief Calls the FMI function fmiCancelStep(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_cancel_step(fmi2_capi_t* fmu);

/**
 * \brief Calls the FMI function fmiDoStep(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param currentCommunicationPoint Current communication point of the master.
 * @param communicationStepSize Communication step size.
 * @param newStep Indicates whether or not the last communication step was accepted by the master.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_do_step(fmi2_capi_t* fmu, fmi2_real_t currentCommunicationPoint, fmi2_real_t communicationStepSize, fmi2_boolean_t newStep);

/**
 * \brief Calls the FMI function fmiGetStatus(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param s Kind of status to return the value for.
 * @param value (Output) FMI status value.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_status(fmi2_capi_t* fmu, const fmi2_status_kind_t s, fmi2_status_t*  value);

/**
 * \brief Calls the FMI function fmiGetRealStatus(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param s Kind of status to return the value for.
 * @param value (Output) FMI real value.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_real_status(fmi2_capi_t* fmu, const fmi2_status_kind_t s, fmi2_real_t*    value);

/**
 * \brief Calls the FMI function fmiGetIntegerStatus(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param s Kind of status to return the value for.
 * @param value (Output) FMI integer value.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_integer_status(fmi2_capi_t* fmu, const fmi2_status_kind_t s, fmi2_integer_t* value);

/**
 * \brief Calls the FMI function fmiGetBooleanStatus(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param s Kind of status to return the value for.
 * @param value (Output) FMI boolean value.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_boolean_status(fmi2_capi_t* fmu, const fmi2_status_kind_t s, fmi2_boolean_t* value);

/**
 * \brief Calls the FMI function fmiGetStringStatus(...) 
 * 
 * @param fmu C-API struct that has succesfully loaded the FMI function.
 * @param s Kind of status to return the value for.
 * @param value (Output) FMI string value.
 * @return FMI status.
 */
fmi2_status_t fmi2_capi_get_string_status(fmi2_capi_t* fmu, const fmi2_status_kind_t s, fmi2_string_t*  value);

/** @}*/
/** @}*/

#ifdef __cplusplus 
}
#endif

#endif /* End of header file FMI2_CAPI_H_ */
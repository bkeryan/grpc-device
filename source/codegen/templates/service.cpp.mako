<%
import common_helpers
import service_helpers

config = data['config']
functions = data['functions']

service_class_prefix = config["service_class_prefix"]
namespace_prefix = config["namespace_component"] + "_grpc::"
module_name = config["module_name"]
if len(config["custom_types"]) > 0:
  custom_types = config["custom_types"]
(input_custom_types, output_custom_types) = common_helpers.get_input_and_output_custom_types(functions)
has_async_functions = any(service_helpers.get_async_functions(functions))
function_names = service_helpers.filter_proto_rpc_functions_to_generate(functions)
# If there are any non-mockable functions, we need to call the library directly, which
# means we need another include file
any_non_mockable_functions = any(not common_helpers.can_mock_function(functions[name]['parameters']) for name in function_names)
# Define the constant for buffer too small if we have any of these functions.
any_ivi_dance_functions = any(
  common_helpers.has_ivi_dance_with_a_twist_param(functions[name]['parameters']) or
  common_helpers.has_ivi_dance_param(functions[name]['parameters']) for name in function_names)
%>\
<%namespace name="mako_helper" file="/service_helpers.mako"/>\

//---------------------------------------------------------------------
// This file is automatically generated. All manual edits will be lost.
//---------------------------------------------------------------------
// Service implementation for the ${config["driver_name"]} Metadata
//---------------------------------------------------------------------
#include "${module_name}_service.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <atomic>
#include <vector>
% if "additional_headers" in config:
% for additional_header in config["additional_headers"]:
#include "${additional_header}"
% endfor
% endif
% if has_async_functions:
#include <server/callback_router.h>
#include <server/server_reactor.h>
% endif
% if any_non_mockable_functions:
#include "${module_name}_library.h"
% endif

namespace ${config["namespace_component"]}_grpc {

% if any_ivi_dance_functions:
  const auto kErrorReadBufferTooSmall = -200229;
  const auto kWarningCAPIStringTruncatedToFitBuffer = 200026;

% endif
  ${service_class_prefix}Service::${service_class_prefix}Service(${service_class_prefix}LibraryInterface* library, ResourceRepositorySharedPtr session_repository)
      : library_(library), session_repository_(session_repository)
  {
  }

  ${service_class_prefix}Service::~${service_class_prefix}Service()
  {
  }

% if common_helpers.has_viboolean_array_param(functions):
  void ${service_class_prefix}Service::Copy(const std::vector<ViBoolean>& input, google::protobuf::RepeatedField<bool>* output) 
  {
    for (auto item : input) {
      output->Add(item != VI_FALSE);
    }
  }

% endif
% if common_helpers.has_enum_array_string_out_param(functions):
  template <typename TEnum>
  void ${service_class_prefix}Service::CopyBytesToEnums(const std::string& input, google::protobuf::RepeatedField<TEnum>* output)
  {
    for (auto item : input)
    {
      output->Add(item);
    }
  }

% endif
% if 'custom_types' in locals():
%   for custom_type in custom_types:
% if custom_type["name"] in output_custom_types:
  void ${service_class_prefix}Service::Copy(const ${custom_type["name"]}& input, ${namespace_prefix}${custom_type["grpc_name"]}* output) 
  {
%     for field in custom_type["fields"]: 
    output->set_${field["grpc_name"]}(input.${field["name"]});
%     endfor
  }

  void ${service_class_prefix}Service::Copy(const std::vector<${custom_type["name"]}>& input, google::protobuf::RepeatedPtrField<${namespace_prefix}${custom_type["grpc_name"]}>* output) 
  {
    for (auto item : input) {
      auto message = new ${namespace_prefix}${custom_type["grpc_name"]}();
      Copy(item, message);
      output->AddAllocated(message);
    }
  }

% endif
% if custom_type["name"] in input_custom_types:
   ${custom_type["name"]} ${service_class_prefix}Service::ConvertMessage(const ${namespace_prefix}${custom_type["grpc_name"]}& input) 
  {
    ${custom_type["name"]}* output = new ${custom_type["name"]}();  
%     for field in custom_type["fields"]: 
    output->${common_helpers.pascal_to_camel(common_helpers.snake_to_pascal(field["grpc_name"]))} = input.${common_helpers.camel_to_snake(field["name"])}();
%     endfor
    return *output;
  }

  void ${service_class_prefix}Service::Copy(const google::protobuf::RepeatedPtrField<${namespace_prefix}${custom_type["grpc_name"]}>& input, std::vector<${custom_type["name"]}>* output)
  {
    std::transform(
        input.begin(),
        input.end(),
        std::back_inserter(*output),
        [&](${namespace_prefix}${custom_type["grpc_name"]} x) { return ConvertMessage(x); });
  }

% endif
%   endfor
% endif
% for function_name in service_helpers.filter_proto_rpc_functions_to_generate(functions):
<%
    function_data = functions[function_name]
    method_name = common_helpers.snake_to_pascal(function_name)
    parameters = function_data['parameters']
    request_param = service_helpers.get_request_param(method_name)
    response_param = service_helpers.get_response_param(method_name)
    response_type = service_helpers.get_response_type(method_name)
    is_async_streaming = common_helpers.has_async_streaming_response(function_data)
%>\
  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
% if is_async_streaming:
  ::grpc::ServerWriteReactor<${response_type}>*
  ${service_class_prefix}Service::${method_name}(::grpc::CallbackServerContext* context, ${request_param})
  {
${mako_helper.define_async_callback_method_body(function_name=function_name, function_data=function_data, parameters=parameters, config=config)}\
  }
% else:
  ::grpc::Status ${service_class_prefix}Service::${method_name}(::grpc::ServerContext* context, ${request_param}, ${response_param})
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
%   if common_helpers.has_unsupported_parameter(function_data):
      return ::grpc::Status(::grpc::UNIMPLEMENTED, "TODO: This server handler has not been implemented.");
%   elif common_helpers.is_init_method(function_data):
${mako_helper.define_init_method_body(function_name=function_name, function_data=function_data, parameters=parameters)}
%   elif common_helpers.has_ivi_dance_param(parameters):
${mako_helper.define_ivi_dance_method_body(function_name=function_name, function_data=function_data, parameters=parameters)}
%   elif common_helpers.has_ivi_dance_with_a_twist_param(parameters):
${mako_helper.define_ivi_dance_with_a_twist_method_body(function_name=function_name, function_data=function_data, parameters=parameters)}
%   elif common_helpers.has_repeated_varargs_parameter(parameters):
${mako_helper.define_repeated_varargs_method_body(function_name=function_name, function_data=function_data, parameters=parameters)}
%   else:
${mako_helper.define_simple_method_body(function_name=function_name, function_data=function_data, parameters=parameters)}
%   endif
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
% if service_helpers.requires_checked_conversion(parameters):
    catch (nidevice_grpc::ValueOutOfRangeException& ex) {
      return ::grpc::Status(::grpc::OUT_OF_RANGE, ex.what());
    }
% endif
  }
% endif

% endfor
} // namespace ${config["namespace_component"]}_grpc


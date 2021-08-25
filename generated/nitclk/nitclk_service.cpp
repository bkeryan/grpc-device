
//---------------------------------------------------------------------
// This file is automatically generated. All manual edits will be lost.
//---------------------------------------------------------------------
// Service implementation for the NI-TClk Metadata
//---------------------------------------------------------------------
#include "nitclk_service.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <atomic>
#include <vector>

namespace nitclk_grpc {

  const auto kErrorReadBufferTooSmall = -200229;
  const auto kWarningCAPIStringTruncatedToFitBuffer = 200026;

  NiTClkService::NiTClkService(NiTClkLibraryInterface* library, ResourceRepositorySharedPtr session_repository)
      : library_(library), session_repository_(session_repository)
  {
  }

  NiTClkService::~NiTClkService()
  {
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::ConfigureForHomogeneousTriggers(::grpc::ServerContext* context, const ConfigureForHomogeneousTriggersRequest* request, ConfigureForHomogeneousTriggersResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      ViUInt32 session_count = static_cast<ViUInt32>(request->sessions().size());
      auto sessions_request = request->sessions();
      std::vector<ViSession> sessions;
      std::transform(
        sessions_request.begin(),
        sessions_request.end(),
        std::back_inserter(sessions),
        [&](auto session) { return session_repository_->access_session(session.id(), session.name()); }); 
      auto status = library_->ConfigureForHomogeneousTriggers(session_count, sessions.data());
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::FinishSyncPulseSenderSynchronize(::grpc::ServerContext* context, const FinishSyncPulseSenderSynchronizeRequest* request, FinishSyncPulseSenderSynchronizeResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      ViUInt32 session_count = static_cast<ViUInt32>(request->sessions().size());
      auto sessions_request = request->sessions();
      std::vector<ViSession> sessions;
      std::transform(
        sessions_request.begin(),
        sessions_request.end(),
        std::back_inserter(sessions),
        [&](auto session) { return session_repository_->access_session(session.id(), session.name()); }); 
      ViReal64 min_time = request->min_time();
      auto status = library_->FinishSyncPulseSenderSynchronize(session_count, sessions.data(), min_time);
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::GetAttributeViReal64(::grpc::ServerContext* context, const GetAttributeViReal64Request* request, GetAttributeViReal64Response* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      auto session_grpc_session = request->session();
      ViSession session = session_repository_->access_session(session_grpc_session.id(), session_grpc_session.name());
      auto channel_name = request->channel_name().c_str();
      ViAttr attribute_id = request->attribute_id();
      ViReal64 value {};
      auto status = library_->GetAttributeViReal64(session, channel_name, attribute_id, &value);
      response->set_status(status);
      if (status == 0) {
        response->set_value(value);
      }
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::GetAttributeViSession(::grpc::ServerContext* context, const GetAttributeViSessionRequest* request, GetAttributeViSessionResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      auto session_grpc_session = request->session();
      ViSession session = session_repository_->access_session(session_grpc_session.id(), session_grpc_session.name());
      auto channel_name = request->channel_name().c_str();
      ViAttr attribute_id = request->attribute_id();
      ViSession value {};
      auto status = library_->GetAttributeViSession(session, channel_name, attribute_id, &value);
      response->set_status(status);
      if (status == 0) {
        auto session_id = session_repository_->resolve_session_id(value);
        response->mutable_value()->set_id(session_id);
      }
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::GetExtendedErrorInfo(::grpc::ServerContext* context, const GetExtendedErrorInfoRequest* request, GetExtendedErrorInfoResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {

      while (true) {
        auto status = library_->GetExtendedErrorInfo(nullptr, 0);
        if (status < 0) {
          response->set_status(status);
          return ::grpc::Status::OK;
        }
        ViUInt32 error_string_size = status;
      
        std::string error_string;
        if (error_string_size > 0) {
            error_string.resize(error_string_size-1);
        }
        status = library_->GetExtendedErrorInfo((ViChar*)error_string.data(), error_string_size);
        if (status == kErrorReadBufferTooSmall || status == kWarningCAPIStringTruncatedToFitBuffer || status > static_cast<decltype(status)>(error_string_size)) {
          // buffer is now too small, try again
          continue;
        }
        response->set_status(status);
        if (status == 0) {
          response->set_error_string(error_string);
        }
        return ::grpc::Status::OK;
      }
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::Initiate(::grpc::ServerContext* context, const InitiateRequest* request, InitiateResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      ViUInt32 session_count = static_cast<ViUInt32>(request->sessions().size());
      auto sessions_request = request->sessions();
      std::vector<ViSession> sessions;
      std::transform(
        sessions_request.begin(),
        sessions_request.end(),
        std::back_inserter(sessions),
        [&](auto session) { return session_repository_->access_session(session.id(), session.name()); }); 
      auto status = library_->Initiate(session_count, sessions.data());
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::IsDone(::grpc::ServerContext* context, const IsDoneRequest* request, IsDoneResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      ViUInt32 session_count = static_cast<ViUInt32>(request->sessions().size());
      auto sessions_request = request->sessions();
      std::vector<ViSession> sessions;
      std::transform(
        sessions_request.begin(),
        sessions_request.end(),
        std::back_inserter(sessions),
        [&](auto session) { return session_repository_->access_session(session.id(), session.name()); }); 
      ViBoolean done {};
      auto status = library_->IsDone(session_count, sessions.data(), &done);
      response->set_status(status);
      if (status == 0) {
        response->set_done(done);
      }
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::SetAttributeViReal64(::grpc::ServerContext* context, const SetAttributeViReal64Request* request, SetAttributeViReal64Response* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      auto session_grpc_session = request->session();
      ViSession session = session_repository_->access_session(session_grpc_session.id(), session_grpc_session.name());
      auto channel_name = request->channel_name().c_str();
      ViAttr attribute_id = request->attribute_id();
      ViReal64 value = request->value_raw();
      auto status = library_->SetAttributeViReal64(session, channel_name, attribute_id, value);
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::SetAttributeViSession(::grpc::ServerContext* context, const SetAttributeViSessionRequest* request, SetAttributeViSessionResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      auto session_grpc_session = request->session();
      ViSession session = session_repository_->access_session(session_grpc_session.id(), session_grpc_session.name());
      auto channel_name = request->channel_name().c_str();
      ViAttr attribute_id = request->attribute_id();
      auto value_grpc_session = request->value();
      ViSession value = session_repository_->access_session(value_grpc_session.id(), value_grpc_session.name());
      auto status = library_->SetAttributeViSession(session, channel_name, attribute_id, value);
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::SetAttributeViString(::grpc::ServerContext* context, const SetAttributeViStringRequest* request, SetAttributeViStringResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      auto session_grpc_session = request->session();
      ViSession session = session_repository_->access_session(session_grpc_session.id(), session_grpc_session.name());
      auto channel_name = request->channel_name().c_str();
      ViAttr attribute_id = request->attribute_id();
      auto value = request->value_raw().c_str();
      auto status = library_->SetAttributeViString(session, channel_name, attribute_id, value);
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::SetupForSyncPulseSenderSynchronize(::grpc::ServerContext* context, const SetupForSyncPulseSenderSynchronizeRequest* request, SetupForSyncPulseSenderSynchronizeResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      ViUInt32 session_count = static_cast<ViUInt32>(request->sessions().size());
      auto sessions_request = request->sessions();
      std::vector<ViSession> sessions;
      std::transform(
        sessions_request.begin(),
        sessions_request.end(),
        std::back_inserter(sessions),
        [&](auto session) { return session_repository_->access_session(session.id(), session.name()); }); 
      ViReal64 min_time = request->min_time();
      auto status = library_->SetupForSyncPulseSenderSynchronize(session_count, sessions.data(), min_time);
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::Synchronize(::grpc::ServerContext* context, const SynchronizeRequest* request, SynchronizeResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      ViUInt32 session_count = static_cast<ViUInt32>(request->sessions().size());
      auto sessions_request = request->sessions();
      std::vector<ViSession> sessions;
      std::transform(
        sessions_request.begin(),
        sessions_request.end(),
        std::back_inserter(sessions),
        [&](auto session) { return session_repository_->access_session(session.id(), session.name()); }); 
      ViReal64 min_tclk_period = request->min_tclk_period();
      auto status = library_->Synchronize(session_count, sessions.data(), min_tclk_period);
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::SynchronizeToSyncPulseSender(::grpc::ServerContext* context, const SynchronizeToSyncPulseSenderRequest* request, SynchronizeToSyncPulseSenderResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      ViUInt32 session_count = static_cast<ViUInt32>(request->sessions().size());
      auto sessions_request = request->sessions();
      std::vector<ViSession> sessions;
      std::transform(
        sessions_request.begin(),
        sessions_request.end(),
        std::back_inserter(sessions),
        [&](auto session) { return session_repository_->access_session(session.id(), session.name()); }); 
      ViReal64 min_time = request->min_time();
      auto status = library_->SynchronizeToSyncPulseSender(session_count, sessions.data(), min_time);
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  ::grpc::Status NiTClkService::WaitUntilDone(::grpc::ServerContext* context, const WaitUntilDoneRequest* request, WaitUntilDoneResponse* response)
  {
    if (context->IsCancelled()) {
      return ::grpc::Status::CANCELLED;
    }
    try {
      ViUInt32 session_count = static_cast<ViUInt32>(request->sessions().size());
      auto sessions_request = request->sessions();
      std::vector<ViSession> sessions;
      std::transform(
        sessions_request.begin(),
        sessions_request.end(),
        std::back_inserter(sessions),
        [&](auto session) { return session_repository_->access_session(session.id(), session.name()); }); 
      ViReal64 timeout = request->timeout();
      auto status = library_->WaitUntilDone(session_count, sessions.data(), timeout);
      response->set_status(status);
      return ::grpc::Status::OK;
    }
    catch (nidevice_grpc::LibraryLoadException& ex) {
      return ::grpc::Status(::grpc::NOT_FOUND, ex.what());
    }
  }

} // namespace nitclk_grpc


// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "laserMsgs.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace laserMsgs {

namespace {

const ::google::protobuf::Descriptor* laserStatus_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  laserStatus_reflection_ = NULL;
const ::google::protobuf::Descriptor* pose_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  pose_reflection_ = NULL;
const ::google::protobuf::Descriptor* laserPose_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  laserPose_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* systemStatus_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_laserMsgs_2eproto() {
  protobuf_AddDesc_laserMsgs_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "laserMsgs.proto");
  GOOGLE_CHECK(file != NULL);
  laserStatus_descriptor_ = file->message_type(0);
  static const int laserStatus_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserStatus, pmw_az_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserStatus, pmw_el_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserStatus, status_),
  };
  laserStatus_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      laserStatus_descriptor_,
      laserStatus::default_instance_,
      laserStatus_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserStatus, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserStatus, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(laserStatus));
  pose_descriptor_ = file->message_type(1);
  static const int pose_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(pose, pulseaz_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(pose, pulseel_),
  };
  pose_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      pose_descriptor_,
      pose::default_instance_,
      pose_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(pose, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(pose, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(pose));
  laserPose_descriptor_ = file->message_type(2);
  static const int laserPose_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserPose, time_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserPose, target_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserPose, laseron_),
  };
  laserPose_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      laserPose_descriptor_,
      laserPose::default_instance_,
      laserPose_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserPose, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(laserPose, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(laserPose));
  systemStatus_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_laserMsgs_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    laserStatus_descriptor_, &laserStatus::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    pose_descriptor_, &pose::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    laserPose_descriptor_, &laserPose::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_laserMsgs_2eproto() {
  delete laserStatus::default_instance_;
  delete laserStatus_reflection_;
  delete pose::default_instance_;
  delete pose_reflection_;
  delete laserPose::default_instance_;
  delete laserPose_reflection_;
}

void protobuf_AddDesc_laserMsgs_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017laserMsgs.proto\022\tlaserMsgs\"_\n\013laserSta"
    "tus\022\016\n\006PMW_AZ\030\001 \001(\003\022\016\n\006PMW_EL\030\002 \001(\003\0220\n\006s"
    "tatus\030\003 \001(\0162\027.laserMsgs.systemStatus:\007UN"
    "KNOWN\"(\n\004pose\022\017\n\007pulseAZ\030\001 \001(\r\022\017\n\007pulseE"
    "L\030\002 \001(\r\"K\n\tlaserPose\022\014\n\004time\030\001 \001(\003\022\037\n\006ta"
    "rget\030\002 \001(\0132\017.laserMsgs.pose\022\017\n\007laserOn\030\003"
    " \001(\010*v\n\014systemStatus\022\016\n\nLASER_DOWN\020\000\022\017\n\013"
    "LASER_READY\020\001\022\025\n\021LASER_OPERATIONAL\020\002\022\017\n\013"
    "LASER_ERROR\020\003\022\020\n\014LASER_FAILED\020\004\022\013\n\007UNKNO"
    "WN\020\005B\013\n\tcom.laser", 377);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "laserMsgs.proto", &protobuf_RegisterTypes);
  laserStatus::default_instance_ = new laserStatus();
  pose::default_instance_ = new pose();
  laserPose::default_instance_ = new laserPose();
  laserStatus::default_instance_->InitAsDefaultInstance();
  pose::default_instance_->InitAsDefaultInstance();
  laserPose::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_laserMsgs_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_laserMsgs_2eproto {
  StaticDescriptorInitializer_laserMsgs_2eproto() {
    protobuf_AddDesc_laserMsgs_2eproto();
  }
} static_descriptor_initializer_laserMsgs_2eproto_;

const ::google::protobuf::EnumDescriptor* systemStatus_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return systemStatus_descriptor_;
}
bool systemStatus_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int laserStatus::kPMWAZFieldNumber;
const int laserStatus::kPMWELFieldNumber;
const int laserStatus::kStatusFieldNumber;
#endif  // !_MSC_VER

laserStatus::laserStatus()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void laserStatus::InitAsDefaultInstance() {
}

laserStatus::laserStatus(const laserStatus& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void laserStatus::SharedCtor() {
  _cached_size_ = 0;
  pmw_az_ = GOOGLE_LONGLONG(0);
  pmw_el_ = GOOGLE_LONGLONG(0);
  status_ = 5;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

laserStatus::~laserStatus() {
  SharedDtor();
}

void laserStatus::SharedDtor() {
  if (this != default_instance_) {
  }
}

void laserStatus::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* laserStatus::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return laserStatus_descriptor_;
}

const laserStatus& laserStatus::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_laserMsgs_2eproto();  return *default_instance_;
}

laserStatus* laserStatus::default_instance_ = NULL;

laserStatus* laserStatus::New() const {
  return new laserStatus;
}

void laserStatus::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    pmw_az_ = GOOGLE_LONGLONG(0);
    pmw_el_ = GOOGLE_LONGLONG(0);
    status_ = 5;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool laserStatus::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int64 PMW_AZ = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &pmw_az_)));
          set_has_pmw_az();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_PMW_EL;
        break;
      }
      
      // optional int64 PMW_EL = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_PMW_EL:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &pmw_el_)));
          set_has_pmw_el();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_status;
        break;
      }
      
      // optional .laserMsgs.systemStatus status = 3 [default = UNKNOWN];
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_status:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (laserMsgs::systemStatus_IsValid(value)) {
            set_status(static_cast< laserMsgs::systemStatus >(value));
          } else {
            mutable_unknown_fields()->AddVarint(3, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void laserStatus::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int64 PMW_AZ = 1;
  if (has_pmw_az()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->pmw_az(), output);
  }
  
  // optional int64 PMW_EL = 2;
  if (has_pmw_el()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(2, this->pmw_el(), output);
  }
  
  // optional .laserMsgs.systemStatus status = 3 [default = UNKNOWN];
  if (has_status()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      3, this->status(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* laserStatus::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int64 PMW_AZ = 1;
  if (has_pmw_az()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->pmw_az(), target);
  }
  
  // optional int64 PMW_EL = 2;
  if (has_pmw_el()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(2, this->pmw_el(), target);
  }
  
  // optional .laserMsgs.systemStatus status = 3 [default = UNKNOWN];
  if (has_status()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      3, this->status(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int laserStatus::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int64 PMW_AZ = 1;
    if (has_pmw_az()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->pmw_az());
    }
    
    // optional int64 PMW_EL = 2;
    if (has_pmw_el()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->pmw_el());
    }
    
    // optional .laserMsgs.systemStatus status = 3 [default = UNKNOWN];
    if (has_status()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->status());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void laserStatus::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const laserStatus* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const laserStatus*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void laserStatus::MergeFrom(const laserStatus& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_pmw_az()) {
      set_pmw_az(from.pmw_az());
    }
    if (from.has_pmw_el()) {
      set_pmw_el(from.pmw_el());
    }
    if (from.has_status()) {
      set_status(from.status());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void laserStatus::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void laserStatus::CopyFrom(const laserStatus& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool laserStatus::IsInitialized() const {
  
  return true;
}

void laserStatus::Swap(laserStatus* other) {
  if (other != this) {
    std::swap(pmw_az_, other->pmw_az_);
    std::swap(pmw_el_, other->pmw_el_);
    std::swap(status_, other->status_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata laserStatus::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = laserStatus_descriptor_;
  metadata.reflection = laserStatus_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int pose::kPulseAZFieldNumber;
const int pose::kPulseELFieldNumber;
#endif  // !_MSC_VER

pose::pose()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void pose::InitAsDefaultInstance() {
}

pose::pose(const pose& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void pose::SharedCtor() {
  _cached_size_ = 0;
  pulseaz_ = 0u;
  pulseel_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

pose::~pose() {
  SharedDtor();
}

void pose::SharedDtor() {
  if (this != default_instance_) {
  }
}

void pose::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* pose::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return pose_descriptor_;
}

const pose& pose::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_laserMsgs_2eproto();  return *default_instance_;
}

pose* pose::default_instance_ = NULL;

pose* pose::New() const {
  return new pose;
}

void pose::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    pulseaz_ = 0u;
    pulseel_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool pose::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint32 pulseAZ = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &pulseaz_)));
          set_has_pulseaz();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_pulseEL;
        break;
      }
      
      // optional uint32 pulseEL = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_pulseEL:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &pulseel_)));
          set_has_pulseel();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void pose::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional uint32 pulseAZ = 1;
  if (has_pulseaz()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->pulseaz(), output);
  }
  
  // optional uint32 pulseEL = 2;
  if (has_pulseel()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->pulseel(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* pose::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional uint32 pulseAZ = 1;
  if (has_pulseaz()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->pulseaz(), target);
  }
  
  // optional uint32 pulseEL = 2;
  if (has_pulseel()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->pulseel(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int pose::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional uint32 pulseAZ = 1;
    if (has_pulseaz()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->pulseaz());
    }
    
    // optional uint32 pulseEL = 2;
    if (has_pulseel()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->pulseel());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void pose::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const pose* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const pose*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void pose::MergeFrom(const pose& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_pulseaz()) {
      set_pulseaz(from.pulseaz());
    }
    if (from.has_pulseel()) {
      set_pulseel(from.pulseel());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void pose::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void pose::CopyFrom(const pose& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool pose::IsInitialized() const {
  
  return true;
}

void pose::Swap(pose* other) {
  if (other != this) {
    std::swap(pulseaz_, other->pulseaz_);
    std::swap(pulseel_, other->pulseel_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata pose::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = pose_descriptor_;
  metadata.reflection = pose_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int laserPose::kTimeFieldNumber;
const int laserPose::kTargetFieldNumber;
const int laserPose::kLaserOnFieldNumber;
#endif  // !_MSC_VER

laserPose::laserPose()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void laserPose::InitAsDefaultInstance() {
  target_ = const_cast< ::laserMsgs::pose*>(&::laserMsgs::pose::default_instance());
}

laserPose::laserPose(const laserPose& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void laserPose::SharedCtor() {
  _cached_size_ = 0;
  time_ = GOOGLE_LONGLONG(0);
  target_ = NULL;
  laseron_ = false;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

laserPose::~laserPose() {
  SharedDtor();
}

void laserPose::SharedDtor() {
  if (this != default_instance_) {
    delete target_;
  }
}

void laserPose::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* laserPose::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return laserPose_descriptor_;
}

const laserPose& laserPose::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_laserMsgs_2eproto();  return *default_instance_;
}

laserPose* laserPose::default_instance_ = NULL;

laserPose* laserPose::New() const {
  return new laserPose;
}

void laserPose::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    time_ = GOOGLE_LONGLONG(0);
    if (has_target()) {
      if (target_ != NULL) target_->::laserMsgs::pose::Clear();
    }
    laseron_ = false;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool laserPose::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int64 time = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &time_)));
          set_has_time();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_target;
        break;
      }
      
      // optional .laserMsgs.pose target = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_target:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_target()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_laserOn;
        break;
      }
      
      // optional bool laserOn = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_laserOn:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &laseron_)));
          set_has_laseron();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void laserPose::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int64 time = 1;
  if (has_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->time(), output);
  }
  
  // optional .laserMsgs.pose target = 2;
  if (has_target()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->target(), output);
  }
  
  // optional bool laserOn = 3;
  if (has_laseron()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(3, this->laseron(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* laserPose::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int64 time = 1;
  if (has_time()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->time(), target);
  }
  
  // optional .laserMsgs.pose target = 2;
  if (has_target()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->target(), target);
  }
  
  // optional bool laserOn = 3;
  if (has_laseron()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(3, this->laseron(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int laserPose::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int64 time = 1;
    if (has_time()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->time());
    }
    
    // optional .laserMsgs.pose target = 2;
    if (has_target()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->target());
    }
    
    // optional bool laserOn = 3;
    if (has_laseron()) {
      total_size += 1 + 1;
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void laserPose::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const laserPose* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const laserPose*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void laserPose::MergeFrom(const laserPose& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_time()) {
      set_time(from.time());
    }
    if (from.has_target()) {
      mutable_target()->::laserMsgs::pose::MergeFrom(from.target());
    }
    if (from.has_laseron()) {
      set_laseron(from.laseron());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void laserPose::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void laserPose::CopyFrom(const laserPose& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool laserPose::IsInitialized() const {
  
  return true;
}

void laserPose::Swap(laserPose* other) {
  if (other != this) {
    std::swap(time_, other->time_);
    std::swap(target_, other->target_);
    std::swap(laseron_, other->laseron_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata laserPose::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = laserPose_descriptor_;
  metadata.reflection = laserPose_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace laserMsgs

// @@protoc_insertion_point(global_scope)

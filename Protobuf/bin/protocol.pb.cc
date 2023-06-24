// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocol.proto

#include "protocol.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = ::PROTOBUF_NAMESPACE_ID::internal;
namespace protocol {
template <typename>
PROTOBUF_CONSTEXPR BuffData::BuffData(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.victims_)*/ {}
  ,/* _impl_._victims_cached_byte_size_ = */ { 0 }

  , /*decltype(_impl_.buffid_)*/ ::uint64_t{0u}

  , /*decltype(_impl_.remaintime_)*/ 0

  , /*decltype(_impl_._cached_size_)*/{}} {}
struct BuffDataDefaultTypeInternal {
  PROTOBUF_CONSTEXPR BuffDataDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~BuffDataDefaultTypeInternal() {}
  union {
    BuffData _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 BuffDataDefaultTypeInternal _BuffData_default_instance_;
template <typename>
PROTOBUF_CONSTEXPR S_TEST::S_TEST(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.buffs_)*/{}
  , /*decltype(_impl_.id_)*/ ::uint64_t{0u}

  , /*decltype(_impl_.hp_)*/ 0u

  , /*decltype(_impl_.attack_)*/ 0u

  , /*decltype(_impl_._cached_size_)*/{}} {}
struct S_TESTDefaultTypeInternal {
  PROTOBUF_CONSTEXPR S_TESTDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~S_TESTDefaultTypeInternal() {}
  union {
    S_TEST _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 S_TESTDefaultTypeInternal _S_TEST_default_instance_;
}  // namespace protocol
static ::_pb::Metadata file_level_metadata_protocol_2eproto[2];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_protocol_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_protocol_2eproto = nullptr;
const ::uint32_t TableStruct_protocol_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(
    protodesc_cold) = {
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::protocol::BuffData, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::protocol::BuffData, _impl_.buffid_),
    PROTOBUF_FIELD_OFFSET(::protocol::BuffData, _impl_.remaintime_),
    PROTOBUF_FIELD_OFFSET(::protocol::BuffData, _impl_.victims_),
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::protocol::S_TEST, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::protocol::S_TEST, _impl_.id_),
    PROTOBUF_FIELD_OFFSET(::protocol::S_TEST, _impl_.hp_),
    PROTOBUF_FIELD_OFFSET(::protocol::S_TEST, _impl_.attack_),
    PROTOBUF_FIELD_OFFSET(::protocol::S_TEST, _impl_.buffs_),
};

static const ::_pbi::MigrationSchema
    schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
        { 0, -1, -1, sizeof(::protocol::BuffData)},
        { 11, -1, -1, sizeof(::protocol::S_TEST)},
};

static const ::_pb::Message* const file_default_instances[] = {
    &::protocol::_BuffData_default_instance_._instance,
    &::protocol::_S_TEST_default_instance_._instance,
};
const char descriptor_table_protodef_protocol_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n\016protocol.proto\022\010protocol\"\?\n\010BuffData\022\016"
    "\n\006buffId\030\001 \001(\004\022\022\n\nremainTime\030\002 \001(\002\022\017\n\007vi"
    "ctims\030\003 \003(\004\"S\n\006S_TEST\022\n\n\002id\030\001 \001(\004\022\n\n\002hp\030"
    "\002 \001(\r\022\016\n\006attack\030\003 \001(\r\022!\n\005buffs\030\004 \003(\0132\022.p"
    "rotocol.BuffDatab\006proto3"
};
static ::absl::once_flag descriptor_table_protocol_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_protocol_2eproto = {
    false,
    false,
    184,
    descriptor_table_protodef_protocol_2eproto,
    "protocol.proto",
    &descriptor_table_protocol_2eproto_once,
    nullptr,
    0,
    2,
    schemas,
    file_default_instances,
    TableStruct_protocol_2eproto::offsets,
    file_level_metadata_protocol_2eproto,
    file_level_enum_descriptors_protocol_2eproto,
    file_level_service_descriptors_protocol_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_protocol_2eproto_getter() {
  return &descriptor_table_protocol_2eproto;
}
// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_protocol_2eproto(&descriptor_table_protocol_2eproto);
namespace protocol {
// ===================================================================

class BuffData::_Internal {
 public:
};

BuffData::BuffData(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:protocol.BuffData)
}
BuffData::BuffData(const BuffData& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  BuffData* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.victims_) { from._impl_.victims_ }
    ,/* _impl_._victims_cached_byte_size_ = */ { 0 }

    , decltype(_impl_.buffid_) {}

    , decltype(_impl_.remaintime_) {}

    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&_impl_.buffid_, &from._impl_.buffid_,
    static_cast<::size_t>(reinterpret_cast<char*>(&_impl_.remaintime_) -
    reinterpret_cast<char*>(&_impl_.buffid_)) + sizeof(_impl_.remaintime_));
  // @@protoc_insertion_point(copy_constructor:protocol.BuffData)
}

inline void BuffData::SharedCtor(::_pb::Arena* arena) {
  (void)arena;
  new (&_impl_) Impl_{
      decltype(_impl_.victims_) { arena }
    ,/* _impl_._victims_cached_byte_size_ = */ { 0 }

    , decltype(_impl_.buffid_) { ::uint64_t{0u} }

    , decltype(_impl_.remaintime_) { 0 }

    , /*decltype(_impl_._cached_size_)*/{}
  };
}

BuffData::~BuffData() {
  // @@protoc_insertion_point(destructor:protocol.BuffData)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void BuffData::SharedDtor() {
  ABSL_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.victims_.~RepeatedField();
}

void BuffData::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void BuffData::Clear() {
// @@protoc_insertion_point(message_clear_start:protocol.BuffData)
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_mutable_victims()->Clear();
  ::memset(&_impl_.buffid_, 0, static_cast<::size_t>(
      reinterpret_cast<char*>(&_impl_.remaintime_) -
      reinterpret_cast<char*>(&_impl_.buffid_)) + sizeof(_impl_.remaintime_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* BuffData::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // uint64 buffId = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 8)) {
          _impl_.buffid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else {
          goto handle_unusual;
        }
        continue;
      // float remainTime = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 21)) {
          _impl_.remaintime_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else {
          goto handle_unusual;
        }
        continue;
      // repeated uint64 victims = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 26)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedUInt64Parser(_internal_mutable_victims(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::uint8_t>(tag) == 24) {
          _internal_add_victims(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr));
          CHK_(ptr);
        } else {
          goto handle_unusual;
        }
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

::uint8_t* BuffData::_InternalSerialize(
    ::uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:protocol.BuffData)
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 buffId = 1;
  if (this->_internal_buffid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt64ToArray(
        1, this->_internal_buffid(), target);
  }

  // float remainTime = 2;
  static_assert(sizeof(::uint32_t) == sizeof(float), "Code assumes ::uint32_t and float are the same size.");
  float tmp_remaintime = this->_internal_remaintime();
  ::uint32_t raw_remaintime;
  memcpy(&raw_remaintime, &tmp_remaintime, sizeof(tmp_remaintime));
  if (raw_remaintime != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteFloatToArray(
        2, this->_internal_remaintime(), target);
  }

  // repeated uint64 victims = 3;
  {
    int byte_size = _impl_._victims_cached_byte_size_.Get();
    if (byte_size > 0) {
      target = stream->WriteUInt64Packed(3, _internal_victims(),
                                                 byte_size, target);
    }
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:protocol.BuffData)
  return target;
}

::size_t BuffData::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:protocol.BuffData)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated uint64 victims = 3;
  {
    std::size_t data_size = ::_pbi::WireFormatLite::UInt64Size(
        this->_internal_victims())
    ;
    _impl_._victims_cached_byte_size_.Set(::_pbi::ToCachedSize(data_size));
    std::size_t tag_size = data_size == 0
        ? 0
        : 1 + ::_pbi::WireFormatLite::Int32Size(
                            static_cast<int32_t>(data_size))
    ;
    total_size += tag_size + data_size;
  }

  // uint64 buffId = 1;
  if (this->_internal_buffid() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(
        this->_internal_buffid());
  }

  // float remainTime = 2;
  static_assert(sizeof(::uint32_t) == sizeof(float), "Code assumes ::uint32_t and float are the same size.");
  float tmp_remaintime = this->_internal_remaintime();
  ::uint32_t raw_remaintime;
  memcpy(&raw_remaintime, &tmp_remaintime, sizeof(tmp_remaintime));
  if (raw_remaintime != 0) {
    total_size += 5;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData BuffData::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    BuffData::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*BuffData::GetClassData() const { return &_class_data_; }


void BuffData::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<BuffData*>(&to_msg);
  auto& from = static_cast<const BuffData&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:protocol.BuffData)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_impl_.victims_.MergeFrom(from._impl_.victims_);
  if (from._internal_buffid() != 0) {
    _this->_internal_set_buffid(from._internal_buffid());
  }
  static_assert(sizeof(::uint32_t) == sizeof(float), "Code assumes ::uint32_t and float are the same size.");
  float tmp_remaintime = from._internal_remaintime();
  ::uint32_t raw_remaintime;
  memcpy(&raw_remaintime, &tmp_remaintime, sizeof(tmp_remaintime));
  if (raw_remaintime != 0) {
    _this->_internal_set_remaintime(from._internal_remaintime());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void BuffData::CopyFrom(const BuffData& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protocol.BuffData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BuffData::IsInitialized() const {
  return true;
}

void BuffData::InternalSwap(BuffData* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.victims_.InternalSwap(&other->_impl_.victims_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(BuffData, _impl_.remaintime_)
      + sizeof(BuffData::_impl_.remaintime_)
      - PROTOBUF_FIELD_OFFSET(BuffData, _impl_.buffid_)>(
          reinterpret_cast<char*>(&_impl_.buffid_),
          reinterpret_cast<char*>(&other->_impl_.buffid_));
}

::PROTOBUF_NAMESPACE_ID::Metadata BuffData::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_protocol_2eproto_getter, &descriptor_table_protocol_2eproto_once,
      file_level_metadata_protocol_2eproto[0]);
}
// ===================================================================

class S_TEST::_Internal {
 public:
};

S_TEST::S_TEST(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:protocol.S_TEST)
}
S_TEST::S_TEST(const S_TEST& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  S_TEST* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.buffs_){from._impl_.buffs_}
    , decltype(_impl_.id_) {}

    , decltype(_impl_.hp_) {}

    , decltype(_impl_.attack_) {}

    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&_impl_.id_, &from._impl_.id_,
    static_cast<::size_t>(reinterpret_cast<char*>(&_impl_.attack_) -
    reinterpret_cast<char*>(&_impl_.id_)) + sizeof(_impl_.attack_));
  // @@protoc_insertion_point(copy_constructor:protocol.S_TEST)
}

inline void S_TEST::SharedCtor(::_pb::Arena* arena) {
  (void)arena;
  new (&_impl_) Impl_{
      decltype(_impl_.buffs_){arena}
    , decltype(_impl_.id_) { ::uint64_t{0u} }

    , decltype(_impl_.hp_) { 0u }

    , decltype(_impl_.attack_) { 0u }

    , /*decltype(_impl_._cached_size_)*/{}
  };
}

S_TEST::~S_TEST() {
  // @@protoc_insertion_point(destructor:protocol.S_TEST)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void S_TEST::SharedDtor() {
  ABSL_DCHECK(GetArenaForAllocation() == nullptr);
  _internal_mutable_buffs()->~RepeatedPtrField();
}

void S_TEST::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void S_TEST::Clear() {
// @@protoc_insertion_point(message_clear_start:protocol.S_TEST)
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_mutable_buffs()->Clear();
  ::memset(&_impl_.id_, 0, static_cast<::size_t>(
      reinterpret_cast<char*>(&_impl_.attack_) -
      reinterpret_cast<char*>(&_impl_.id_)) + sizeof(_impl_.attack_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* S_TEST::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // uint64 id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 8)) {
          _impl_.id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else {
          goto handle_unusual;
        }
        continue;
      // uint32 hp = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 16)) {
          _impl_.hp_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else {
          goto handle_unusual;
        }
        continue;
      // uint32 attack = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 24)) {
          _impl_.attack_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else {
          goto handle_unusual;
        }
        continue;
      // repeated .protocol.BuffData buffs = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 34)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_buffs(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<34>(ptr));
        } else {
          goto handle_unusual;
        }
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

::uint8_t* S_TEST::_InternalSerialize(
    ::uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:protocol.S_TEST)
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 id = 1;
  if (this->_internal_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt64ToArray(
        1, this->_internal_id(), target);
  }

  // uint32 hp = 2;
  if (this->_internal_hp() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(
        2, this->_internal_hp(), target);
  }

  // uint32 attack = 3;
  if (this->_internal_attack() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(
        3, this->_internal_attack(), target);
  }

  // repeated .protocol.BuffData buffs = 4;
  for (unsigned i = 0,
      n = static_cast<unsigned>(this->_internal_buffs_size()); i < n; i++) {
    const auto& repfield = this->_internal_buffs(i);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(4, repfield, repfield.GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:protocol.S_TEST)
  return target;
}

::size_t S_TEST::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:protocol.S_TEST)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .protocol.BuffData buffs = 4;
  total_size += 1UL * this->_internal_buffs_size();
  for (const auto& msg : this->_internal_buffs()) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // uint64 id = 1;
  if (this->_internal_id() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(
        this->_internal_id());
  }

  // uint32 hp = 2;
  if (this->_internal_hp() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(
        this->_internal_hp());
  }

  // uint32 attack = 3;
  if (this->_internal_attack() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(
        this->_internal_attack());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData S_TEST::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    S_TEST::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*S_TEST::GetClassData() const { return &_class_data_; }


void S_TEST::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<S_TEST*>(&to_msg);
  auto& from = static_cast<const S_TEST&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:protocol.S_TEST)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_internal_mutable_buffs()->MergeFrom(from._internal_buffs());
  if (from._internal_id() != 0) {
    _this->_internal_set_id(from._internal_id());
  }
  if (from._internal_hp() != 0) {
    _this->_internal_set_hp(from._internal_hp());
  }
  if (from._internal_attack() != 0) {
    _this->_internal_set_attack(from._internal_attack());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void S_TEST::CopyFrom(const S_TEST& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protocol.S_TEST)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool S_TEST::IsInitialized() const {
  return true;
}

void S_TEST::InternalSwap(S_TEST* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _internal_mutable_buffs()->InternalSwap(other->_internal_mutable_buffs());
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(S_TEST, _impl_.attack_)
      + sizeof(S_TEST::_impl_.attack_)
      - PROTOBUF_FIELD_OFFSET(S_TEST, _impl_.id_)>(
          reinterpret_cast<char*>(&_impl_.id_),
          reinterpret_cast<char*>(&other->_impl_.id_));
}

::PROTOBUF_NAMESPACE_ID::Metadata S_TEST::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_protocol_2eproto_getter, &descriptor_table_protocol_2eproto_once,
      file_level_metadata_protocol_2eproto[1]);
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace protocol
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::protocol::BuffData*
Arena::CreateMaybeMessage< ::protocol::BuffData >(Arena* arena) {
  return Arena::CreateMessageInternal< ::protocol::BuffData >(arena);
}
template<> PROTOBUF_NOINLINE ::protocol::S_TEST*
Arena::CreateMaybeMessage< ::protocol::S_TEST >(Arena* arena) {
  return Arena::CreateMessageInternal< ::protocol::S_TEST >(arena);
}
PROTOBUF_NAMESPACE_CLOSE
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"

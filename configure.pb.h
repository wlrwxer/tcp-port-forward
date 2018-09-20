// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: configure.proto

#ifndef PROTOBUF_configure_2eproto__INCLUDED
#define PROTOBUF_configure_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_configure_2eproto();
void protobuf_AssignDesc_configure_2eproto();
void protobuf_ShutdownFile_configure_2eproto();

class addr_info;
class addr_map;
class configure;

// ===================================================================

class addr_info : public ::google::protobuf::Message {
 public:
  addr_info();
  virtual ~addr_info();

  addr_info(const addr_info& from);

  inline addr_info& operator=(const addr_info& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const addr_info& default_instance();

  void Swap(addr_info* other);

  // implements Message ----------------------------------------------

  addr_info* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const addr_info& from);
  void MergeFrom(const addr_info& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string addr = 100;
  inline bool has_addr() const;
  inline void clear_addr();
  static const int kAddrFieldNumber = 100;
  inline const ::std::string& addr() const;
  inline void set_addr(const ::std::string& value);
  inline void set_addr(const char* value);
  inline void set_addr(const char* value, size_t size);
  inline ::std::string* mutable_addr();
  inline ::std::string* release_addr();
  inline void set_allocated_addr(::std::string* addr);

  // required uint32 port = 200;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 200;
  inline ::google::protobuf::uint32 port() const;
  inline void set_port(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:addr_info)
 private:
  inline void set_has_addr();
  inline void clear_has_addr();
  inline void set_has_port();
  inline void clear_has_port();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* addr_;
  ::google::protobuf::uint32 port_;
  friend void  protobuf_AddDesc_configure_2eproto();
  friend void protobuf_AssignDesc_configure_2eproto();
  friend void protobuf_ShutdownFile_configure_2eproto();

  void InitAsDefaultInstance();
  static addr_info* default_instance_;
};
// -------------------------------------------------------------------

class addr_map : public ::google::protobuf::Message {
 public:
  addr_map();
  virtual ~addr_map();

  addr_map(const addr_map& from);

  inline addr_map& operator=(const addr_map& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const addr_map& default_instance();

  void Swap(addr_map* other);

  // implements Message ----------------------------------------------

  addr_map* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const addr_map& from);
  void MergeFrom(const addr_map& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .addr_info locals = 100;
  inline int locals_size() const;
  inline void clear_locals();
  static const int kLocalsFieldNumber = 100;
  inline const ::addr_info& locals(int index) const;
  inline ::addr_info* mutable_locals(int index);
  inline ::addr_info* add_locals();
  inline const ::google::protobuf::RepeatedPtrField< ::addr_info >&
      locals() const;
  inline ::google::protobuf::RepeatedPtrField< ::addr_info >*
      mutable_locals();

  // repeated .addr_info remotes = 200;
  inline int remotes_size() const;
  inline void clear_remotes();
  static const int kRemotesFieldNumber = 200;
  inline const ::addr_info& remotes(int index) const;
  inline ::addr_info* mutable_remotes(int index);
  inline ::addr_info* add_remotes();
  inline const ::google::protobuf::RepeatedPtrField< ::addr_info >&
      remotes() const;
  inline ::google::protobuf::RepeatedPtrField< ::addr_info >*
      mutable_remotes();

  // @@protoc_insertion_point(class_scope:addr_map)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::addr_info > locals_;
  ::google::protobuf::RepeatedPtrField< ::addr_info > remotes_;
  friend void  protobuf_AddDesc_configure_2eproto();
  friend void protobuf_AssignDesc_configure_2eproto();
  friend void protobuf_ShutdownFile_configure_2eproto();

  void InitAsDefaultInstance();
  static addr_map* default_instance_;
};
// -------------------------------------------------------------------

class configure : public ::google::protobuf::Message {
 public:
  configure();
  virtual ~configure();

  configure(const configure& from);

  inline configure& operator=(const configure& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const configure& default_instance();

  void Swap(configure* other);

  // implements Message ----------------------------------------------

  configure* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const configure& from);
  void MergeFrom(const configure& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .addr_map maps = 100;
  inline int maps_size() const;
  inline void clear_maps();
  static const int kMapsFieldNumber = 100;
  inline const ::addr_map& maps(int index) const;
  inline ::addr_map* mutable_maps(int index);
  inline ::addr_map* add_maps();
  inline const ::google::protobuf::RepeatedPtrField< ::addr_map >&
      maps() const;
  inline ::google::protobuf::RepeatedPtrField< ::addr_map >*
      mutable_maps();

  // required uint32 run_as_daemon = 200;
  inline bool has_run_as_daemon() const;
  inline void clear_run_as_daemon();
  static const int kRunAsDaemonFieldNumber = 200;
  inline ::google::protobuf::uint32 run_as_daemon() const;
  inline void set_run_as_daemon(::google::protobuf::uint32 value);

  // optional string server_crash_run = 320;
  inline bool has_server_crash_run() const;
  inline void clear_server_crash_run();
  static const int kServerCrashRunFieldNumber = 320;
  inline const ::std::string& server_crash_run() const;
  inline void set_server_crash_run(const ::std::string& value);
  inline void set_server_crash_run(const char* value);
  inline void set_server_crash_run(const char* value, size_t size);
  inline ::std::string* mutable_server_crash_run();
  inline ::std::string* release_server_crash_run();
  inline void set_allocated_server_crash_run(::std::string* server_crash_run);

  // @@protoc_insertion_point(class_scope:configure)
 private:
  inline void set_has_run_as_daemon();
  inline void clear_has_run_as_daemon();
  inline void set_has_server_crash_run();
  inline void clear_has_server_crash_run();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::addr_map > maps_;
  ::std::string* server_crash_run_;
  ::google::protobuf::uint32 run_as_daemon_;
  friend void  protobuf_AddDesc_configure_2eproto();
  friend void protobuf_AssignDesc_configure_2eproto();
  friend void protobuf_ShutdownFile_configure_2eproto();

  void InitAsDefaultInstance();
  static configure* default_instance_;
};
// ===================================================================


// ===================================================================

// addr_info

// required string addr = 100;
inline bool addr_info::has_addr() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void addr_info::set_has_addr() {
  _has_bits_[0] |= 0x00000001u;
}
inline void addr_info::clear_has_addr() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void addr_info::clear_addr() {
  if (addr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    addr_->clear();
  }
  clear_has_addr();
}
inline const ::std::string& addr_info::addr() const {
  // @@protoc_insertion_point(field_get:addr_info.addr)
  return *addr_;
}
inline void addr_info::set_addr(const ::std::string& value) {
  set_has_addr();
  if (addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    addr_ = new ::std::string;
  }
  addr_->assign(value);
  // @@protoc_insertion_point(field_set:addr_info.addr)
}
inline void addr_info::set_addr(const char* value) {
  set_has_addr();
  if (addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    addr_ = new ::std::string;
  }
  addr_->assign(value);
  // @@protoc_insertion_point(field_set_char:addr_info.addr)
}
inline void addr_info::set_addr(const char* value, size_t size) {
  set_has_addr();
  if (addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    addr_ = new ::std::string;
  }
  addr_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:addr_info.addr)
}
inline ::std::string* addr_info::mutable_addr() {
  set_has_addr();
  if (addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    addr_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:addr_info.addr)
  return addr_;
}
inline ::std::string* addr_info::release_addr() {
  clear_has_addr();
  if (addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = addr_;
    addr_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void addr_info::set_allocated_addr(::std::string* addr) {
  if (addr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete addr_;
  }
  if (addr) {
    set_has_addr();
    addr_ = addr;
  } else {
    clear_has_addr();
    addr_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:addr_info.addr)
}

// required uint32 port = 200;
inline bool addr_info::has_port() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void addr_info::set_has_port() {
  _has_bits_[0] |= 0x00000002u;
}
inline void addr_info::clear_has_port() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void addr_info::clear_port() {
  port_ = 0u;
  clear_has_port();
}
inline ::google::protobuf::uint32 addr_info::port() const {
  // @@protoc_insertion_point(field_get:addr_info.port)
  return port_;
}
inline void addr_info::set_port(::google::protobuf::uint32 value) {
  set_has_port();
  port_ = value;
  // @@protoc_insertion_point(field_set:addr_info.port)
}

// -------------------------------------------------------------------

// addr_map

// repeated .addr_info locals = 100;
inline int addr_map::locals_size() const {
  return locals_.size();
}
inline void addr_map::clear_locals() {
  locals_.Clear();
}
inline const ::addr_info& addr_map::locals(int index) const {
  // @@protoc_insertion_point(field_get:addr_map.locals)
  return locals_.Get(index);
}
inline ::addr_info* addr_map::mutable_locals(int index) {
  // @@protoc_insertion_point(field_mutable:addr_map.locals)
  return locals_.Mutable(index);
}
inline ::addr_info* addr_map::add_locals() {
  // @@protoc_insertion_point(field_add:addr_map.locals)
  return locals_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::addr_info >&
addr_map::locals() const {
  // @@protoc_insertion_point(field_list:addr_map.locals)
  return locals_;
}
inline ::google::protobuf::RepeatedPtrField< ::addr_info >*
addr_map::mutable_locals() {
  // @@protoc_insertion_point(field_mutable_list:addr_map.locals)
  return &locals_;
}

// repeated .addr_info remotes = 200;
inline int addr_map::remotes_size() const {
  return remotes_.size();
}
inline void addr_map::clear_remotes() {
  remotes_.Clear();
}
inline const ::addr_info& addr_map::remotes(int index) const {
  // @@protoc_insertion_point(field_get:addr_map.remotes)
  return remotes_.Get(index);
}
inline ::addr_info* addr_map::mutable_remotes(int index) {
  // @@protoc_insertion_point(field_mutable:addr_map.remotes)
  return remotes_.Mutable(index);
}
inline ::addr_info* addr_map::add_remotes() {
  // @@protoc_insertion_point(field_add:addr_map.remotes)
  return remotes_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::addr_info >&
addr_map::remotes() const {
  // @@protoc_insertion_point(field_list:addr_map.remotes)
  return remotes_;
}
inline ::google::protobuf::RepeatedPtrField< ::addr_info >*
addr_map::mutable_remotes() {
  // @@protoc_insertion_point(field_mutable_list:addr_map.remotes)
  return &remotes_;
}

// -------------------------------------------------------------------

// configure

// repeated .addr_map maps = 100;
inline int configure::maps_size() const {
  return maps_.size();
}
inline void configure::clear_maps() {
  maps_.Clear();
}
inline const ::addr_map& configure::maps(int index) const {
  // @@protoc_insertion_point(field_get:configure.maps)
  return maps_.Get(index);
}
inline ::addr_map* configure::mutable_maps(int index) {
  // @@protoc_insertion_point(field_mutable:configure.maps)
  return maps_.Mutable(index);
}
inline ::addr_map* configure::add_maps() {
  // @@protoc_insertion_point(field_add:configure.maps)
  return maps_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::addr_map >&
configure::maps() const {
  // @@protoc_insertion_point(field_list:configure.maps)
  return maps_;
}
inline ::google::protobuf::RepeatedPtrField< ::addr_map >*
configure::mutable_maps() {
  // @@protoc_insertion_point(field_mutable_list:configure.maps)
  return &maps_;
}

// required uint32 run_as_daemon = 200;
inline bool configure::has_run_as_daemon() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void configure::set_has_run_as_daemon() {
  _has_bits_[0] |= 0x00000002u;
}
inline void configure::clear_has_run_as_daemon() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void configure::clear_run_as_daemon() {
  run_as_daemon_ = 0u;
  clear_has_run_as_daemon();
}
inline ::google::protobuf::uint32 configure::run_as_daemon() const {
  // @@protoc_insertion_point(field_get:configure.run_as_daemon)
  return run_as_daemon_;
}
inline void configure::set_run_as_daemon(::google::protobuf::uint32 value) {
  set_has_run_as_daemon();
  run_as_daemon_ = value;
  // @@protoc_insertion_point(field_set:configure.run_as_daemon)
}

// optional string server_crash_run = 320;
inline bool configure::has_server_crash_run() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void configure::set_has_server_crash_run() {
  _has_bits_[0] |= 0x00000004u;
}
inline void configure::clear_has_server_crash_run() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void configure::clear_server_crash_run() {
  if (server_crash_run_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_crash_run_->clear();
  }
  clear_has_server_crash_run();
}
inline const ::std::string& configure::server_crash_run() const {
  // @@protoc_insertion_point(field_get:configure.server_crash_run)
  return *server_crash_run_;
}
inline void configure::set_server_crash_run(const ::std::string& value) {
  set_has_server_crash_run();
  if (server_crash_run_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_crash_run_ = new ::std::string;
  }
  server_crash_run_->assign(value);
  // @@protoc_insertion_point(field_set:configure.server_crash_run)
}
inline void configure::set_server_crash_run(const char* value) {
  set_has_server_crash_run();
  if (server_crash_run_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_crash_run_ = new ::std::string;
  }
  server_crash_run_->assign(value);
  // @@protoc_insertion_point(field_set_char:configure.server_crash_run)
}
inline void configure::set_server_crash_run(const char* value, size_t size) {
  set_has_server_crash_run();
  if (server_crash_run_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_crash_run_ = new ::std::string;
  }
  server_crash_run_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:configure.server_crash_run)
}
inline ::std::string* configure::mutable_server_crash_run() {
  set_has_server_crash_run();
  if (server_crash_run_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_crash_run_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:configure.server_crash_run)
  return server_crash_run_;
}
inline ::std::string* configure::release_server_crash_run() {
  clear_has_server_crash_run();
  if (server_crash_run_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = server_crash_run_;
    server_crash_run_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void configure::set_allocated_server_crash_run(::std::string* server_crash_run) {
  if (server_crash_run_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete server_crash_run_;
  }
  if (server_crash_run) {
    set_has_server_crash_run();
    server_crash_run_ = server_crash_run;
  } else {
    clear_has_server_crash_run();
    server_crash_run_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:configure.server_crash_run)
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_configure_2eproto__INCLUDED

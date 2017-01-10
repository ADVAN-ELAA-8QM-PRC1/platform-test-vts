# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: TestSchedulingPolicyMessage.proto

from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)




DESCRIPTOR = _descriptor.FileDescriptor(
  name='TestSchedulingPolicyMessage.proto',
  package='android.vts',
  serialized_pb='\n!TestSchedulingPolicyMessage.proto\x12\x0b\x61ndroid.vts\"\x98\x01\n\x1bTestSchedulingPolicyMessage\x12\x14\n\x0ctarget_plans\x18\x01 \x03(\x0c\x12\x14\n\x0ctarget_tests\x18\x02 \x03(\x0c\x12\x38\n\x0fscheduling_mode\x18\x65 \x01(\x0e\x32\x1f.android.vts.TestSchedulingMode\x12\x13\n\x0bperiod_secs\x18n \x01(\r*\x87\x01\n\x12TestSchedulingMode\x12$\n UKNOWN_TEST_SCHEDULING_MODE_TYPE\x10\x00\x12(\n$TEST_SCHEDULING_MODE_TOT_BEST_EFFORT\x10\x01\x12!\n\x1dTEST_SCHEDULING_MODE_PERIODIC\x10\x02')

_TESTSCHEDULINGMODE = _descriptor.EnumDescriptor(
  name='TestSchedulingMode',
  full_name='android.vts.TestSchedulingMode',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='UKNOWN_TEST_SCHEDULING_MODE_TYPE', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TEST_SCHEDULING_MODE_TOT_BEST_EFFORT', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TEST_SCHEDULING_MODE_PERIODIC', index=2, number=2,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=206,
  serialized_end=341,
)

TestSchedulingMode = enum_type_wrapper.EnumTypeWrapper(_TESTSCHEDULINGMODE)
UKNOWN_TEST_SCHEDULING_MODE_TYPE = 0
TEST_SCHEDULING_MODE_TOT_BEST_EFFORT = 1
TEST_SCHEDULING_MODE_PERIODIC = 2



_TESTSCHEDULINGPOLICYMESSAGE = _descriptor.Descriptor(
  name='TestSchedulingPolicyMessage',
  full_name='android.vts.TestSchedulingPolicyMessage',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='target_plans', full_name='android.vts.TestSchedulingPolicyMessage.target_plans', index=0,
      number=1, type=12, cpp_type=9, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='target_tests', full_name='android.vts.TestSchedulingPolicyMessage.target_tests', index=1,
      number=2, type=12, cpp_type=9, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='scheduling_mode', full_name='android.vts.TestSchedulingPolicyMessage.scheduling_mode', index=2,
      number=101, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='period_secs', full_name='android.vts.TestSchedulingPolicyMessage.period_secs', index=3,
      number=110, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=51,
  serialized_end=203,
)

_TESTSCHEDULINGPOLICYMESSAGE.fields_by_name['scheduling_mode'].enum_type = _TESTSCHEDULINGMODE
DESCRIPTOR.message_types_by_name['TestSchedulingPolicyMessage'] = _TESTSCHEDULINGPOLICYMESSAGE

class TestSchedulingPolicyMessage(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _TESTSCHEDULINGPOLICYMESSAGE

  # @@protoc_insertion_point(class_scope:android.vts.TestSchedulingPolicyMessage)


# @@protoc_insertion_point(module_scope)

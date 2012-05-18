# Generated by the protocol buffer compiler.  DO NOT EDIT!

from google.protobuf import descriptor
from google.protobuf import message
from google.protobuf import reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)



DESCRIPTOR = descriptor.FileDescriptor(
  name='laserMsgs.proto',
  package='laserMsgs',
  serialized_pb='\n\x0flaserMsgs.proto\x12\tlaserMsgs\"[\n\x0blaserStatus\x12\x0c\n\x04time\x18\x01 \x01(\x03\x12\x30\n\x06status\x18\x02 \x01(\x0e\x32\x17.laserMsgs.systemStatus:\x07UNKNOWN\x12\x0c\n\x04text\x18\x03 \x01(\t\"(\n\x04pose\x12\x0f\n\x07pulseAZ\x18\x01 \x01(\r\x12\x0f\n\x07pulseEL\x18\x02 \x01(\r\"K\n\tlaserPose\x12\x0c\n\x04time\x18\x01 \x01(\x03\x12\x1f\n\x06target\x18\x02 \x01(\x0b\x32\x0f.laserMsgs.pose\x12\x0f\n\x07laserOn\x18\x03 \x01(\x08*v\n\x0csystemStatus\x12\x0e\n\nLASER_DOWN\x10\x00\x12\x0f\n\x0bLASER_READY\x10\x01\x12\x15\n\x11LASER_OPERATIONAL\x10\x02\x12\x0f\n\x0bLASER_ERROR\x10\x03\x12\x10\n\x0cLASER_FAILED\x10\x04\x12\x0b\n\x07UNKNOWN\x10\x05')

_SYSTEMSTATUS = descriptor.EnumDescriptor(
  name='systemStatus',
  full_name='laserMsgs.systemStatus',
  filename=None,
  file=DESCRIPTOR,
  values=[
    descriptor.EnumValueDescriptor(
      name='LASER_DOWN', index=0, number=0,
      options=None,
      type=None),
    descriptor.EnumValueDescriptor(
      name='LASER_READY', index=1, number=1,
      options=None,
      type=None),
    descriptor.EnumValueDescriptor(
      name='LASER_OPERATIONAL', index=2, number=2,
      options=None,
      type=None),
    descriptor.EnumValueDescriptor(
      name='LASER_ERROR', index=3, number=3,
      options=None,
      type=None),
    descriptor.EnumValueDescriptor(
      name='LASER_FAILED', index=4, number=4,
      options=None,
      type=None),
    descriptor.EnumValueDescriptor(
      name='UNKNOWN', index=5, number=5,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=242,
  serialized_end=360,
)


LASER_DOWN = 0
LASER_READY = 1
LASER_OPERATIONAL = 2
LASER_ERROR = 3
LASER_FAILED = 4
UNKNOWN = 5



_LASERSTATUS = descriptor.Descriptor(
  name='laserStatus',
  full_name='laserMsgs.laserStatus',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    descriptor.FieldDescriptor(
      name='time', full_name='laserMsgs.laserStatus.time', index=0,
      number=1, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='status', full_name='laserMsgs.laserStatus.status', index=1,
      number=2, type=14, cpp_type=8, label=1,
      has_default_value=True, default_value=5,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='text', full_name='laserMsgs.laserStatus.text', index=2,
      number=3, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=unicode("", "utf-8"),
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
  serialized_start=30,
  serialized_end=121,
)


_POSE = descriptor.Descriptor(
  name='pose',
  full_name='laserMsgs.pose',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    descriptor.FieldDescriptor(
      name='pulseAZ', full_name='laserMsgs.pose.pulseAZ', index=0,
      number=1, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='pulseEL', full_name='laserMsgs.pose.pulseEL', index=1,
      number=2, type=13, cpp_type=3, label=1,
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
  serialized_start=123,
  serialized_end=163,
)


_LASERPOSE = descriptor.Descriptor(
  name='laserPose',
  full_name='laserMsgs.laserPose',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    descriptor.FieldDescriptor(
      name='time', full_name='laserMsgs.laserPose.time', index=0,
      number=1, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='target', full_name='laserMsgs.laserPose.target', index=1,
      number=2, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    descriptor.FieldDescriptor(
      name='laserOn', full_name='laserMsgs.laserPose.laserOn', index=2,
      number=3, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
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
  serialized_start=165,
  serialized_end=240,
)

_LASERSTATUS.fields_by_name['status'].enum_type = _SYSTEMSTATUS
_LASERPOSE.fields_by_name['target'].message_type = _POSE
DESCRIPTOR.message_types_by_name['laserStatus'] = _LASERSTATUS
DESCRIPTOR.message_types_by_name['pose'] = _POSE
DESCRIPTOR.message_types_by_name['laserPose'] = _LASERPOSE

class laserStatus(message.Message):
  __metaclass__ = reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _LASERSTATUS
  
  # @@protoc_insertion_point(class_scope:laserMsgs.laserStatus)

class pose(message.Message):
  __metaclass__ = reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _POSE
  
  # @@protoc_insertion_point(class_scope:laserMsgs.pose)

class laserPose(message.Message):
  __metaclass__ = reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _LASERPOSE
  
  # @@protoc_insertion_point(class_scope:laserMsgs.laserPose)

# @@protoc_insertion_point(module_scope)

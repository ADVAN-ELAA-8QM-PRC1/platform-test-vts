#!/usr/bin/env python
#
# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import copy
import logging

from vts.runners.host.proto import AndroidSystemControlMessage_pb2

from google.protobuf import text_format


class MirrorObject(object):
    """Actual mirror object.

    Args:
        _client: the TCP client instance.
        _if_spec_msg: the interface specification message of a host object to
            mirror.
    """

    def __init__(self, client, msg):
        self._client = client
        self._if_spec_msg = msg

    def GetApi(self, api_name):
        """Returns the Function Specification Message.

        Args:
            api_name: string, the name of the target function API.

        Returns:
            FunctionSpecificationMessage if found, None otherwise
        """
        for api in self._if_spec_msg.api:
            if api.name == api_name:
                return copy.copy(api)
        return None

    def __getattr__(self, api_name):
        """Calls a target component's API.

        Args:
            api_name: string, the name of an API function to call.
            *args: a list of arguments
            **kwargs: a dict for the arg name and value pairs
        """

        def RemoteCall(*args, **kwargs):
            logging.info("remote call %s", api_name)
            func_msg = self.GetApi(api_name)
            if not func_msg:
                logging.fatal("unknown api name %s", api_name)
            logging.info(func_msg)
            if args:
                for arg_msg, value_msg in zip(func_msg.arg, args):
                    logging.info("arg msg value %s %s", arg_msg, value_msg)
                    if value_msg:
                        for primitive_value in value_msg.primitive_value:
                            pv = arg_msg.primitive_value.add()
                            if primitive_value.HasField("uint32_t"):
                                pv.uint32_t = primitive_value.uint32_t
                            if primitive_value.HasField("int32_t"):
                                pv.int32_t = primitive_value.int32_t
                logging.info("final msg %s", func_msg)
            else:
                # TODO: use kwargs
                for arg in func_msg.arg:
                    # TODO: handle other
                    if arg.primitive_type == "pointer":
                        value = arg.primitive_value.add()
                        value.pointer = 0
                logging.info(func_msg)

            self._client.SendCommand(
                AndroidSystemControlMessage_pb2.CALL_FUNCTION,
                text_format.MessageToString(func_msg))
            resp = self._client.RecvResponse()
            logging.info(resp)

        return RemoteCall

/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ProfilerCodeGenBase.h"
#include "utils/InterfaceSpecUtil.h"
#include "VtsCompilerUtils.h"

namespace android {
namespace vts {

ProfilerCodeGenBase::ProfilerCodeGenBase(const std::string& input_vts_file_path,
                                         const std::string& vts_name)
    : input_vts_file_path_(input_vts_file_path),
      vts_name_(vts_name) {
}

ProfilerCodeGenBase::~ProfilerCodeGenBase() {
}

void ProfilerCodeGenBase::GenerateAll(
    Formatter& header_out, Formatter& source_out,
    const ComponentSpecificationMessage& message) {
  GenerateHeaderFile(header_out, message);
  GenerateSourceFile(source_out, message);
}

void ProfilerCodeGenBase::GenerateHeaderFile(
    Formatter& out, const ComponentSpecificationMessage& message) {
  out << "#ifndef __VTS_PROFILER_" << vts_name_ << "__\n";
  out << "#define __VTS_PROFILER_" << vts_name_ << "__\n";
  out << "\n\n";
  GenerateHeaderIncludeFiles(out, message);
  GenerateUsingDeclaration(out, message);
  GenerateOpenNameSpaces(out);

  out << "extern \"C\" {\n";
  out.indent();

  if (message.has_interface()) {
    InterfaceSpecificationMessage interface = message.interface();
    // First generate the declaration of profiler functions for all user
    // defined types within the interface.
    for (const auto attribute : interface.attribute()) {
      GenerateProfilerMethodDeclForAttribute(out, attribute);
    }
    // Generate the declaration of main profiler function.
    out << "\nvoid HIDL_INSTRUMENTATION_FUNCTION(\n";
    out.indent();
    out.indent();
    out << "InstrumentationEvent event,\n";
    out << "const char* package,\n";
    out << "const char* version,\n";
    out << "const char* interface,\n";
    out << "const char* method,\n";
    out << "std::vector<void *> *args);\n";
    out.unindent();
    out.unindent();
  } else {
    // For types.vts, just generate the declaration of profiler functions
    // for all user defined types.
    for (const auto attribute : message.attribute()) {
      GenerateProfilerMethodDeclForAttribute(out, attribute);
    }
  }

  out.unindent();
  out << "}\n\n";

  GenerateCloseNameSpaces(out);
  out << "#endif\n";
}

void ProfilerCodeGenBase::GenerateSourceFile(
    Formatter& out, const ComponentSpecificationMessage& message) {
  GenerateSourceIncludeFiles(out, message);
  GenerateUsingDeclaration(out, message);
  GenerateOpenNameSpaces(out);

  if (message.has_interface()) {
    InterfaceSpecificationMessage interface = message.interface();
    // First generate profiler functions for all user defined types within
    // the interface.
    for (const auto attribute : interface.attribute()) {
      GenerateProfilerMethodImplForAttribute(out, attribute);
    }

    // Generate the main profiler function.
    out << "\nvoid HIDL_INSTRUMENTATION_FUNCTION(\n";
    out.indent();
    out.indent();
    out << "InstrumentationEvent event,\n";
    out << "const char* package,\n";
    out << "const char* version,\n";
    out << "const char* interface,\n";
    out << "const char* method,\n";
    out << "std::vector<void *> *args) {\n";
    out.unindent();

    // Code for sanity check.
    GenerateProfierSanityCheck(out, message);

    // Generate the profiler code for each method.
    for (const FunctionSpecificationMessage api : interface.api()) {
      out << "if (strcmp(method, \"" << api.name() << "\") == 0) {\n";
      out.indent();
      GenerateProfilerForMethod(out, api);
    }

    out.unindent();
    out << "}\n\n";
  } else {
    // For types.vts, just generate profiler functions for the user defined
    // types.
    for (const auto attribute : message.attribute()) {
      GenerateProfilerMethodImplForAttribute(out, attribute);
    }
  }

  GenerateCloseNameSpaces(out);
}

void ProfilerCodeGenBase::GenerateProfilerForTypedVariable(Formatter& out,
  const VariableSpecificationMessage& val, const std::string& arg_name,
  const std::string& arg_value) {
  switch (val.type()) {
    case TYPE_SCALAR:
    {
      GenerateProfilerForScalarVariable(out, val, arg_name, arg_value);
      break;
    }
    case TYPE_STRING:
    {
      GenerateProfilerForStringVariable(out, val, arg_name, arg_value);
      break;
    }
    case TYPE_ENUM:
    {
      GenerateProfilerForEnumVariable(out, val, arg_name, arg_value);
      break;
    }
    case TYPE_VECTOR:
    {
      GenerateProfilerForVectorVariable(out, val, arg_name, arg_value);
      break;
    }
    case TYPE_ARRAY:
    {
      GenerateProfilerForArrayVariable(out, val, arg_name, arg_value);
      break;
    }
    case TYPE_STRUCT:
    {
      GenerateProfilerForStructVariable(out, val, arg_name, arg_value);
      break;
    }
    // uncomment once vts support union type.
    /*case TYPE_UNION: {
       GenerateProfilerForUnionVariable(out, val, arg_name, arg_value);
       break;
    }*/
    case TYPE_HIDL_CALLBACK:
    {
      GenerateProfilerForHidlCallbackVariable(out, val, arg_name, arg_value);
      break;
    }
    default:
    {
      cout << "not supported.\n";
    }
  }
}

void ProfilerCodeGenBase::GenerateProfilerMethodDeclForAttribute(
    Formatter& out, const VariableSpecificationMessage& attribute) {
  // TODO(zhuoyao): handle union type once vts support union type.
  if (attribute.type() == TYPE_STRUCT) {
    // Recursively generate profiler method declaration for all sub_types.
    for (const auto sub_struct : attribute.struct_value()) {
      // TODO(zhuoyao): update the condition once vts distinguish between
      // sub_types and fields.
      // TODO(zhuoyao): handle union sub_type once vts support union type.
      if (sub_struct.type() == TYPE_STRUCT
          && !sub_struct.has_predefined_type()) {
        GenerateProfilerMethodDeclForAttribute(out, sub_struct);
      }
    }

    out << "void profile__" << attribute.name()
        << "(VariableSpecificationMessage* arg_name,\n" << attribute.name()
        << " arg_val_name);\n";
  }
}

void ProfilerCodeGenBase::GenerateProfilerMethodImplForAttribute(
    Formatter& out, const VariableSpecificationMessage& attribute) {
  // TODO(zhuoyao): handle union type once vts support union type.
  if (attribute.type() == TYPE_STRUCT) {
    // Recursively generate profiler method implementation for all sub_types.
    for (const auto sub_struct : attribute.struct_value()) {
      // TODO(zhuoyao): update the condition once vts distinguish between
      // sub_types and fields.
      // TODO(zhuoyao): handle union sub_type once vts support union type.
      if (sub_struct.type() == TYPE_STRUCT
          && !sub_struct.has_predefined_type()) {
        GenerateProfilerMethodImplForAttribute(out, sub_struct);
      }
    }

    out << "void profile__" << attribute.name()
        << "(VariableSpecificationMessage* arg_name,\n" << attribute.name()
        << " arg_val_name) {\n";
    out.indent();
    GenerateProfilerForTypedVariable(out, attribute, "arg_name",
                                     "arg_val_name");
    out.unindent();
    out << "}\n\n";
  }
}

void ProfilerCodeGenBase::GenerateProfierSanityCheck(Formatter&,
  const ComponentSpecificationMessage&) {
  return;
}

void ProfilerCodeGenBase::GenerateOpenNameSpaces(Formatter& out) {
  out << "namespace android {\n";
  out << "namespace vts {\n\n";
}

void ProfilerCodeGenBase::GenerateCloseNameSpaces(Formatter& out) {
  out << "}  // namespace vts\n";
  out << "}  // namespace android\n";
}

std::string ProfilerCodeGenBase::GetPackage(
    const ComponentSpecificationMessage& message) {
  return message.package();
}
std::string ProfilerCodeGenBase::GetPackageVersion(
    const ComponentSpecificationMessage& message) {
  return GetVersionString(message.component_type_version());
}
std::string ProfilerCodeGenBase::GetComponentName(
    const ComponentSpecificationMessage& message) {
  return message.component_name();
}

}  // namespace vts
}  // namespace android
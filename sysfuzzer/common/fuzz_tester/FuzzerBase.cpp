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

#include "fuzz_tester/FuzzerBase.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "test/vts/sysfuzzer/common/proto/InterfaceSpecificationMessage.pb.h"

#include "component_loader/DllLoader.h"
#include "utils/InterfaceSpecUtil.h"

#include "gcda_parser.h"

using namespace std;
using namespace android;

#define USE_GCOV 1

#if SANCOV
extern "C" {
typedef unsigned long uptr;

#define SANITIZER_INTERFACE_ATTRIBUTE __attribute__((visibility("default")))
SANITIZER_INTERFACE_ATTRIBUTE
void __sanitizer_cov(uint32_t *guard) {
  printf("sancov\n");
  coverage_data.Add(StackTrace::GetPreviousInstructionPc(GET_CALLER_PC()),
                    guard);
}

SANITIZER_INTERFACE_ATTRIBUTE
void *__asan_memset(void *block, int c, uptr size) {
  ASAN_MEMSET_IMPL(nullptr, block, c, size);
  return block;
}


SANITIZER_INTERFACE_ATTRIBUTE
void __asan_register_globals() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_unregister_globals() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_handle_no_return() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_load1() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_load2() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_load4() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_load8() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_load16() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_store1() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_store2() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_store4() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_store8() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_report_store16() { }
SANITIZER_INTERFACE_ATTRIBUTE
void __asan_set_error_report_callback() { }

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_1(uptr size, uptr real_stack) {
  return (uptr)malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_2(uptr size, uptr real_stack) {
  return (uptr)malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_3(uptr size, uptr real_stack) {
  return (uptr)malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_4(uptr size, uptr real_stack) {
  return (uptr)malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_5(uptr size, uptr real_stack) {
  return (uptr)malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_6(uptr size, uptr real_stack) {
  return (uptr)malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_7(uptr size, uptr real_stack) {
  return (uptr)malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_8(uptr size, uptr real_stack) {
  return(uptr) malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_9(uptr size, uptr real_stack) {
  return (uptr)malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
uptr __asan_stack_malloc_10(uptr size, uptr real_stack) {
  return (uptr)malloc(size);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_1(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_2(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_3(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_4(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_5(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_6(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_7(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_8(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_9(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void  __asan_stack_free_10(uptr ptr, uptr size, uptr real_stack) {
  free((void*)ptr);
}

SANITIZER_INTERFACE_ATTRIBUTE
void __asan_version_mismatch_check_v6() {
  // Do nothing.
}

SANITIZER_INTERFACE_ATTRIBUTE void
__sanitizer_cov_module_init(int32_t *guards, uptr npcs, uint8_t *counters,
                            const char *comp_unit_name) {
}

SANITIZER_INTERFACE_ATTRIBUTE
void __asan_init() {
  static int inited = 0;
  if (inited) return;
  inited = 1;
#if __WORDSIZE == 64
  unsigned long start = 0x100000000000;
  unsigned long size  = 0x100000000000;
#else
  unsigned long start = 0x20000000;
  unsigned long size = 0x20000000;
#endif
  void *res = mmap((void*)start, size,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANON | MAP_FIXED | MAP_NORESERVE,
                   0, 0);
  if (res == (void*)start) {
    fprintf(stderr, "Fake AddressSanitizer run-time initialized ok at %p\n",
            res);
  } else {
    fprintf(stderr, "Fake AddressSanitizer run-time failed to initialize.\n"
            "You have been warned. Aborting.");
    abort();
  }
}

}
#endif

namespace android {
namespace vts {

static void RemoveDir(char *path) {
  struct dirent* entry = NULL;
  DIR* dir = opendir(path);

  while ((entry = readdir(dir)) != NULL) {
    DIR* sub_dir = NULL;
    FILE* file = NULL;
    char abs_path[4096] = {0};

    if (*(entry->d_name) != '.') {
      sprintf(abs_path, "%s/%s", path, entry->d_name);
      if ((sub_dir = opendir(abs_path)) != NULL) {
        closedir(sub_dir);
        RemoveDir(abs_path);
      } else if((file = fopen(abs_path, "r")) != NULL) {
        fclose(file);
        remove(abs_path);
      }
    }
  }
  remove(path);
}


FuzzerBase::FuzzerBase(int target_class)
    : target_class_(target_class),
      component_filename_(NULL),
      gcov_output_basepath_(NULL) {}


FuzzerBase::~FuzzerBase() {
  free(component_filename_);
}


void wfn() {
  cout << "wfn" << endl;
}

void ffn() {
  cout << "ffn" << endl;
}

bool FuzzerBase::LoadTargetComponent(
    const char* target_dll_path, const char* module_name) {
  cout << __FUNCTION__ << ":" << __LINE__ << " " << "LoadTargetCompooent entry" << endl;
  target_dll_path_ = target_dll_path;
  if (!target_loader_.Load(target_dll_path_)) return false;
  cout << __FUNCTION__ << ":" << __LINE__ << " " << "LoadTargetCompooent loaded the target" << endl;
  if (target_class_ == LEGACY_HAL) return true;
  cout << __FUNCTION__ << ": loaded a non-legacy HAL file." << endl;
  device_ = target_loader_.GetHWDevice(module_name);
  cout << __FUNCTION__ << ": device_ " << device_ << endl;
#if SANCOV
  cout << __FUNCTION__ << "sancov reset " << target_loader_.SancovResetCoverage() << endl;;
#endif

  if (target_dll_path_) {
    string target_path(target_dll_path_);

    size_t offset = target_path.rfind("/", target_path.length());
    if (offset != string::npos) {
      string filename = target_path.substr(offset + 1,
                                           target_path.length() - offset);
      filename = filename.substr(0, filename.length() - 3 /* for .so */);
      component_filename_ = (char*) malloc(filename.length() + 1);
      strcpy(component_filename_, filename.c_str());
      cout << "module file name: " << component_filename_ << endl;
    }
    cout << "module_name " << target_dll_path_ << endl;
  }

#if USE_GCOV
  cout << __FUNCTION__ << ": gcov init " << target_loader_.GcovInit(wfn, ffn) << endl;
#endif
  return (device_ != NULL);
}


bool FuzzerBase::Fuzz(const vts::InterfaceSpecificationMessage& message,
                      void** result) {
  cout << "Fuzzing target component: "
      << "class " << message.component_class()
      << " type " << message.component_type()
      << " version " << message.component_type_version() << endl;

  string function_name_prefix = GetFunctionNamePrefix(message);
  function_name_prefix_ = function_name_prefix.c_str();
  for (const vts::FunctionSpecificationMessage& func_msg : message.api()) {
    Fuzz(func_msg, result);
  }
  return true;
}


void FuzzerBase::FunctionCallBegin() {
  char product_path[4096];
  char product[128];
  char module_basepath[4096];
  char cwd[4096];

  if (getcwd(cwd, 4096)) {
    cout << cwd << endl;
    int n = snprintf(product_path, 4096, "%s/out/target/product", cwd);
    if (n <= 0 || n >= 4096) {
      cerr << "couln't get product_path" << endl;
      return;
    }
    DIR* srcdir = opendir(product_path);
    if (!srcdir) {
      cerr << "couln't open " << product_path << endl;
      return;
    }

    int dir_count = 0;
    struct dirent* dent;
    while ((dent = readdir(srcdir)) != NULL) {
      struct stat st;
      if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) {
        continue;
      }
      if (fstatat(dirfd(srcdir), dent->d_name, &st, 0) < 0) {
        cerr << "error " << dent->d_name << endl;
        continue;
      }
      if (S_ISDIR(st.st_mode)) {
        cout << dent->d_name << endl;
        strcpy(product, dent->d_name);
        dir_count++;
      }
    }
    closedir(srcdir);
    if (dir_count != 1) {
      cerr << "more than one product dir found." << endl;
      return;
    }

    n = snprintf(module_basepath, 4096, "%s/%s/obj/SHARED_LIBRARIES",
                 product_path, product);
    if (n <= 0 || n >= 4096) {
      cerr << "couln't get module_basepath" << endl;
      return;
    }
    srcdir = opendir(module_basepath);
    if (!srcdir) {
      cerr << "couln't open " << module_basepath << endl;
      return;
    }

    dir_count = 0;
    string target = string(component_filename_) + "_intermediates";
    bool hit = false;
    while ((dent = readdir(srcdir)) != NULL) {
      struct stat st;
      if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) {
        continue;
      }
      if (fstatat(dirfd(srcdir), dent->d_name, &st, 0) < 0) {
        cerr << "error " << dent->d_name << endl;
        continue;
      }
      if (S_ISDIR(st.st_mode)) {
        cout << "module_basepath " << string(dent->d_name) << endl;
        if (string(dent->d_name) == target) {
          cout << "hit" << endl;
          hit = true;
        }
        dir_count++;
      }
    }
    if (hit) {
      if (gcov_output_basepath_) {
        free(gcov_output_basepath_);
      }
      gcov_output_basepath_ = (char*) malloc(strlen(module_basepath) + target.length() + 2);
      if (!gcov_output_basepath_) {
        cerr << __FUNCTION__ << ": couldn't alloc memory" << endl;
        return;
      }
      sprintf(gcov_output_basepath_, "%s/%s", module_basepath, target.c_str());
      RemoveDir(gcov_output_basepath_);
    }
    closedir(srcdir);
  } else {
    cerr << __FUNCTION__ << ": couldn't get the pwd." << endl;
  }
}


vector<unsigned>* FuzzerBase::FunctionCallEnd() {
  cout << __FUNCTION__ << ": gcov flush " << endl;
  std::vector<unsigned>* result = NULL;
#if USE_GCOV
  target_loader_.GcovFlush();
  // find the file.
  if (!gcov_output_basepath_) {
    cerr << __FUNCTION__ << ": no gcov basepath set" << endl;
    return NULL;
  }
  DIR* srcdir = opendir(gcov_output_basepath_);
  if (!srcdir) {
    cerr << "couln't open " << gcov_output_basepath_ << endl;
    return NULL;
  }

  int dir_count = 0;
  struct dirent* dent;
  while ((dent = readdir(srcdir)) != NULL) {
    struct stat st;
    if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) {
      continue;
    }
    if (fstatat(dirfd(srcdir), dent->d_name, &st, 0) < 0) {
      cerr << "error " << dent->d_name << endl;
      continue;
    }
    if (!S_ISDIR(st.st_mode)) {
      cout << dent->d_name << endl;
      if (string(dent->d_name).rfind(".gcda") != string::npos) {
        char* buffer;
        buffer = (char*) malloc(strlen(gcov_output_basepath_) + strlen(dent->d_name) + 2);
        if (!buffer) {
          cerr << __FUNCTION__ << ": OOM" << endl;
          closedir(srcdir);
          return NULL;
        }
        sprintf(buffer, "%s/%s", gcov_output_basepath_, dent->d_name);
        result = android::vts::parse_gcda_file(buffer);
#if USE_GCOV_DEBUG
        if (result) {
          for (unsigned int index = 0; index < result->size(); index++) {
            cout << result->at(index) << endl;
          }
        }
#endif
        free(buffer);
        break;
      }
    }
  }
  closedir(srcdir);
#endif
  return result;
}

}  // namespace vts
}  // namespace android

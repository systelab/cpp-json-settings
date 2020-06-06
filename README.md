[![Build Status](https://travis-ci.org/systelab/cpp-json-settings.svg?branch=master)](https://travis-ci.org/systelab/cpp-json-settings)
[![Build status](https://ci.appveyor.com/api/projects/status/vl7vvxt33ex4l1bk?svg=true)](https://ci.appveyor.com/project/systelab/cpp-json-settings)
[![codecov](https://codecov.io/gh/systelab/cpp-json-settings/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-json-settings)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/6526d0da00cb4c90ad196782faf09523)](https://www.codacy.com/manual/systelab/cpp-json-settings?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=systelab/cpp-json-settings&amp;utm_campaign=Badge_Grade)
[![Download](https://api.bintray.com/packages/systelab/conan/JSONSettings:systelab/images/download.svg)](https://bintray.com/systelab/conan/JSONSettings:systelab/_latestVersion)

# C++ JSON Settings Library

This library provides utilities to save/load configuration settings on JSON files.

## Supported features

* Compile-time error detection
* Basic types: string, boolean, integer and double
* Sections hierarchy
* In-memory cache
* Encryption of JSON files


## Setup

### Download using Conan

  1. Create/update your `conanfile.txt` to add this library as follows:

```
[requires]
JSONSettings/1.2.0@systelab/stable

[generators]
cmake
```

  2. Integrate Conan into CMake by adding the following code into your `CMakeLists.txt`:

```cmake
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
```

  3. Link against `${CONAN_LIBS}` when configuring your executables in CMake:

```cmake
set(MY_PROJECT MyProject)
add_executable(${MY_PROJECT} main.cpp)
target_link_libraries(${MY_PROJECT} ${CONAN_LIBS})
```

### Build from sources

Prerequisites:
  - [Git](https://git-scm.com/)
  - [Conan](https://conan.io/)
  - [CMake](https://cmake.org/)
  - [Visual Studio](https://visualstudio.microsoft.com/) (only on Windows)
  - [GCC](https://gcc.gnu.org/) (only on Linux)

Build library with the following steps:
  1. Clone this repository in a local drive
  2. Make a build directory (i.e. `build/`)
  3. Register bintray repository for Systelab libraries on conan
  4. Install `conan` dependencies in the build directory
  5. Run `cmake` in the build directory to configure build targets
  6. Use `Visual Studio` (on Windows) or `make` (on Linux) to build the library

#### Windows

In order to build the application on Windows for the `Release` configuration, run the following commands ($VSINSTALLPATH is the path where Visual Studio has been installed):

``` bash
> git clone https://github.com/systelab/cpp-json-configuration
> md build && cd build
> conan remote add systelab-bintray https://api.bintray.com/conan/systelab/conan
> conan install .. -s build_type=Release -s compiler.toolset=v141 -s arch=x86_64
> cmake .. -G "Visual Studio 15 2017 Win64"
> "$VSINSTALLPATH/devenv.com" JSONSettings.sln /build "Release" /PROJECT "JSONSettings"
```

However, if you want to `Debug` the source code, you will need these commands:

``` bash
> git clone https://github.com/systelab/cpp-json-configuration
> md build && cd build
> conan remote add systelab-bintray https://api.bintray.com/conan/systelab/conan
> conan install .. -s build_type=Debug -s compiler.toolset=v141 -s arch=x86_64
> cmake .. -G "Visual Studio 15 2017 Win64"
> "$VSINSTALLPATH/devenv.com" JSONSettings.sln /build "Debug" /PROJECT "JSONSettings"
```

#### Linux
``` bash
> git clone https://github.com/systelab/cpp-json-configuration
> mkdir build && cd build
> conan remote add systelab-bintray https://api.bintray.com/conan/systelab/conan
> conan install ..
> cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
> make
```


## Usage

### Settings definition

The settings files should be defined using the `JSON_SETTINGS_FILE` macro, which specifies the associated symbol name (used to access to it) and the associated JSON filename.
This macro also defines the settings to be contained on the file by making use of the `JSON_SETTING_INT`, `JSON_SETTING_DBL`, `JSON_SETTING_STR` and `JSON_SETTING_BOOL` macros.
Thus, for each setting, the following information needs to be provided:

* *Value type*: Defined implicitly by the macro used. Current implementation supports 3 types: integer (JSON_SETTING_INT), double (JSON_SETTING_DBL) string (JSON_SETTING_STR) and boolean (JSON_SETTING_BOOL).
* *Symbol name*: Name of the symbol used when accessing to the setting. It must be unique regards the settings file.
* *Path*: Path of the setting when storing it on a JSON file. Represented as a string with the list of parent sections of the setting separated each one by a dot (`.`) character.
* *Default value*: Default value of the setting when not possible to retrieve it from the JSON file (i.e file not exists).
* *Cache enabled*: When enabled, the setting is queried from the JSON file only the first time. Further queries will return the cached in-memory value unless the cache is explicitly deleted

See the following example for a better understanding of how to define a settings file:

``` cpp
#include "JSONSettings/SettingDefinitionMacros.h"

#define CACHE_ENABLED true
#define CACHE_DISABLED false

JSON_SETTINGS_FILE(MySettingsFile, "MySettingsFile.json",

	// Root settings
	JSON_SETTING_INT (RootIntSettingCache,   "IntSettingCache",   2222,     CACHE_ENABLED)
	JSON_SETTING_DBL (RootDblSettingCache,   "DblSettingCache",   3.33,     CACHE_DISABLED)
	JSON_SETTING_STR (RootStrSettingNoCache, "StrSettingNoCache", "SECOND", CACHE_DISABLED)
	JSON_SETTING_BOOL(RootBoolSettingCache,  "BoolSettingCache",  false,    CACHE_ENABLED)
	
	// Section settings
	JSON_SETTING_INT (SectionIntSetting,  "Section.IntSettingCache",  4321, CACHE_ENABLED)
	JSON_SETTING_DBL (SectionDblSetting,  "Section.DblSettingCache",  55.5, CACHE_ENABLED)
	JSON_SETTING_STR (SectionStrSetting,  "Section.StrSettingCache",  "ba", CACHE_ENABLED)
	JSON_SETTING_BOOL(SectionBoolSetting, "Section.BoolSettingCache", true, CACHE_ENABLED)

	// Subsection settings
	JSON_SETTING_INT (SubsectionIntSetting,  "Section.Subsection.IntSettingCache",   8765, CACHE_DISABLED)
	JSON_SETTING_DBL (SubsectionDblSetting,  "Section.Subsection.DblSettingCache",  8.888, CACHE_DISABLED)
	JSON_SETTING_STR (SubsectionStrSetting,  "Section.Subsection.StrSettingCache",   "dc", CACHE_DISABLED)
	JSON_SETTING_BOOL(SubsectionBoolSetting, "Section.Subsection.BoolSettingCache", false, CACHE_DISABLED)
);
```


### Folder configuration

The folder where the JSON settings files are stored can be defined by means of the `SET_JSON_SETTINGS_FOLDER` macro:

``` cpp
#include "JSONSettings/SettingDefinitionMacros.h"

SET_JSON_SETTINGS_FOLDER("../Path/Of/My/Settings/Folder");
```


### Settings access

Query the value associated to a setting using the `GET_JSON_SETTING_INT`, `GET_JSON_SETTING_DBL`, `GET_JSON_SETTING_STR` or `GET_JSON_SETTING_BOOL` macros with the symbol names of the file and the setting as arguments:

``` cpp
#include "JSONSettings/SettingsService.h"
#include "JSONSettings/SettingsMacros.h"

int settingValue = GET_JSON_SETTING_INT(systelab::setting::SettingsService(), MySettingsFile, SectionIntSetting);
```
> Notice that the header file with the settings file definition must be included to use the symbol names

Similarly, a setting value can be changed through the `SET_JSON_SETTING_INT`, `SET_JSON_SETTING_DBL`, `SET_JSON_SETTING_STR` and `SET_JSON_SETTING_BOOL` macros:

``` cpp
SET_JSON_SETTING_INT(systelab::setting::SettingsService(), MySettingsFile, SubsectionStrSetting, "New value for subsection setting");
```


### Cache clean up

The contents of the settings cache can be removed by making use of the `clearCache()` method of the `SettingsService` class:

``` cpp
#include "JSONSettings/SettingsService.h"

systelab::setting::SettingsService().clearCache();
```


### Files encryption

This library can work with encrypted settings files when they are defined using the `JSON_SETTINGS_ENCRYPTED_FILE` macro. It works the same way than the `JSON_SETTINGS_FILE` macro, but it takes an additional argument more that represents the key to be used to encrypt/decrypt the file. Moreover, with the purpose of allowing more flexibility on how encryption key is obtained, it should be provided as a function (i.e. a lambda) that returns a string.

``` cpp
#include "JSONSettings/SettingDefinitionMacros.h"

JSON_SETTINGS_FILE(EncryptedSettingsFile, "EncryptedSettingsFile.json",
                   []() { return "MySuperSecretEncryptionKey"; },

	JSON_SETTING_INT (IntSetting, "IntSetting", 1234, true)
	...

);
```

Next step consists of defining the encryption method/algorithm to use. It is supported any implementation of the systelab's [C++ Encryption Adapter interface](https://github.com/systelab/cpp-encryption-adapter), represented by an object of type `systelab::encryption::IEncryptionAdapter`. For instance, if the simple [Caeser Cypher](https://github.com/systelab/cpp-caeser-cypher-encryption-adapter) algorithm is the selected one, then an instance of `systelab::encryption::caeser_cypher::EncryptionAdapter` will be required.

``` cpp
#include "CaeserCypherEncryptionAdapter/EncryptionAdapter.h"

std::unique_ptr<systelab::encryption::IEncryptionAdapter> encryptionAdapter =
    std::make<systelab::encryption::caeser_cypher::EncryptionAdapter>();
```

Finally, settings access works like when accessing a non-encrypted file. The only difference is on the instance of `systelab::setting::SettingsService` provided to the settings access macros. So, here this service needs to be created by providing a reference to the encryption adapter object previously built.

``` cpp
#include "JSONSettings/SettingsService.h"
#include "JSONSettings/SettingsMacros.h"

int settingValue = GET_JSON_SETTING_INT(systelab::setting::SettingsService(*encryptionAdapter), EncryptedSettingsFile, IntSetting);

SET_JSON_SETTING_INT(systelab::setting::SettingsService(*encryptionAdapter), EncryptedSettingsFile, IntSetting, 4321);
```


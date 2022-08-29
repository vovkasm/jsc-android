#!/usr/bin/python3
#
# Copyright 2021 The ANGLE Project Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# gen_extensions.py:
#   Generates files from supported extensions data.
#   NOTE: don't run this script directly. Run scripts/run_code_generation.py.

import json
import os
import re
import sys

d = os.path.dirname
THIS_DIR = d(os.path.abspath(__file__))
ANGLE_SRC_DIR = d(d(THIS_DIR))
SCRIPTS_DIR = os.path.join(ANGLE_SRC_DIR, 'scripts')
sys.path.insert(0, SCRIPTS_DIR)

import registry_xml

_GLES_EXTENSIONS_TEMPLATE = """\
// GENERATED FILE - DO NOT EDIT.
// Generated by {script_name} using data from {data_source_name}
//
// Copyright 2021 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// {filename}: GLES extension information.

#ifndef LIBANGLE_GLES_EXTENSIONS_AUTOGEN_H_
#define LIBANGLE_GLES_EXTENSIONS_AUTOGEN_H_

namespace gl
{{
class TextureCapsMap;

struct Extensions
{{
    Extensions();
    Extensions(const Extensions &other);

    Extensions &operator=(const Extensions &other);

    // Generate a vector of supported extension strings
    std::vector<std::string> getStrings() const;

    // Set all texture related extension support based on the supported textures.
    // Determines support for:
    // GL_OES_packed_depth_stencil
    // GL_OES_rgb8_rgba8
    // GL_EXT_texture_format_BGRA8888
    // GL_EXT_color_buffer_half_float,
    // GL_OES_texture_half_float, GL_OES_texture_half_float_linear
    // GL_OES_texture_float, GL_OES_texture_float_linear
    // GL_EXT_texture_rg
    // GL_EXT_texture_type_2_10_10_10_REV
    // GL_EXT_texture_compression_dxt1, GL_ANGLE_texture_compression_dxt3,
    // GL_ANGLE_texture_compression_dxt5
    // GL_KHR_texture_compression_astc_ldr, GL_OES_texture_compression_astc.
    //     NOTE: GL_KHR_texture_compression_astc_hdr must be enabled separately. Support for the
    //           HDR profile cannot be determined from the format enums alone.
    // GL_OES_compressed_ETC1_RGB8_texture
    // GL_EXT_sRGB
    // GL_ANGLE_depth_texture, GL_OES_depth32
    // GL_EXT_color_buffer_float
    // GL_EXT_texture_norm16
    // GL_EXT_texture_compression_bptc
    // GL_EXT_texture_compression_rgtc
    void setTextureExtensionSupport(const TextureCapsMap &textureCaps);

    // Helper functions
{helper_functions}

    // GLES 2.0+ extensions
    // --------------------

{gles_extensions}
    // ANGLE unofficial extensions
    // ---------------------------

{angle_extensions}
    // GLES 1.0 and 1.1 extensions
    // ---------------------------

{gles1_extensions}}};
}}  // namespace gl

#endif  // LIBANGLE_GLES_EXTENSIONS_AUTOGEN_H_
"""

_EXT_MEMBER_TEMPLATE = """\
    // {full_name}
    bool {name_camel_case}{vendor} = false;
"""

_HELPER_TEMPLATE = """    bool {ext_name}Any() const {{ return ({expression}); }}"""

_GLES_EXT_STRINGS_TEMPLATE = """\
// GENERATED FILE - DO NOT EDIT.
// Generated by {script_name} using data from {data_source_name}
//
// Copyright 2021 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// {filename}: GLES extension strings information.

#include "anglebase/no_destructor.h"
#include "libANGLE/Caps.h"

namespace gl
{{
const ExtensionInfoMap &GetExtensionInfoMap()
{{
    auto buildExtensionInfoMap = []() {{
        auto enableableExtension = [](ExtensionBool member) {{
            ExtensionInfo info;
            info.Requestable      = true;
            info.ExtensionsMember = member;
            return info;
        }};

        auto enableableDisablableExtension = [&](ExtensionBool member) {{
            ExtensionInfo info = enableableExtension(member);
            info.Disablable    = true;
            return info;
        }};

        auto esOnlyExtension = [](ExtensionBool member) {{
            ExtensionInfo info;
            info.ExtensionsMember = member;
            return info;
        }};

        // clang-format off
        ExtensionInfoMap map;

        // GLES 2.0 extension strings
        // --------------------------
{gles_strings}

        // ANGLE unofficial extension strings
        // ----------------------------------
{angle_strings}

        // GLES 1.0 and 1.1 extension strings
        // ----------------------------------
{gles1_strings}
        // clang-format on

#if defined(ANGLE_ENABLE_ASSERTS)
        // Verify all extension strings start with GL_
        for (const auto &extension : map)
        {{
            ASSERT(extension.first.rfind("GL_", 0) == 0);
        }}
#endif

        return map;
    }};

    static const angle::base::NoDestructor<ExtensionInfoMap> extensionInfo(buildExtensionInfoMap());
    return *extensionInfo;
}}
}} // namespace gl
"""

_EXT_STRING_TEMPLATE = """\
        map["{full_name}"] = {mode}Extension(&Extensions::{name_camel_case}{vendor});"""

ESONLY = 'esOnly'
REQUESTABLE = 'enableable'
TOGGLEABLE = 'enableableDisablable'

_MARKDOWN_TEMPLATE = """\
# ANGLE Supported Extensions

This is a list of all extensions currently supported by ANGLE's front-end, and
support listed for some of the tested targets for ANGLE's Vulkan back-end. To
produce a list of all supported extensions in the Vulkan back-end, run
`angle_end2end_tests` with `--gtest_filter EGLPrintEGLinfoTest.PrintGLInfo/ES*_Vulkan`.

Specifications for GLES extensions can be found in the [Khronos OpenGL ES API
Registry](http://www.khronos.org/registry/gles/)

Specifications for EGL extensions can be found in the [Khronos EGL API
Registry](http://www.khronos.org/registry/egl/)

Specifications for ANGLE-specific extensions can be found in the [ANGLE
extension registry](../extensions)

This list is automatically generated by [`{script_name}`](../src/libANGLE/gen_extensions.py)
using data from {data_source_name}.

## GLES 2.0, 3.0, 3.1 and 3.2 extension support

*Note: some data is sampled from older drivers, so might not represent the latest driver support.*

{gles_md_table_header}
{gles_md_exts}

## ANGLE unofficial extension support

*Note: some ANGLE extensions are currently missing specifications.*

{angle_md_table_header}
{angle_md_exts}

## GLES 1.0 and 1.1 extension support

{gles1_md_table_header}
{gles1_md_exts}

## EGL extension support

Currently EGL extensions are not automatically tracked by our scripting. For a
list of supported EGL extensions in ANGLE's front-end see
[`src/libANGLE/Caps.h`](../src/libANGLE/Caps.h).

## Configuration information

{md_gpu_info}
## How to update supported extension data

Supported extension data is stored in the ANGLE repo as JSON files in
[`scripts/extension_data`](../scripts/extension_data). The JSON data is
sourced from public ANGLE test runs. Look for `angle_end2end_tests` in a bot
run: [example link](https://ci.chromium.org/ui/p/angle/builders/ci/win-test/520/overview).
Search for "`angle_end2end_tests`", then click on the "cas output" and find
`GLinfo_ES3_2_Vulkan.json` or `GLinfo_ES3_1_Vulkan_SwiftShader.json` for
SwiftShader.

All data except for GLES 1 is automatically updated using
the [`update_extension_data.py`](../scripts/update_extension_data.py) script.
To use it first authenticate to the `bb` and `luci-go` tools by running `bb
auth-login` and `./tools/luci-go/swarming login`. Then run the script and
re-run [code generation][CodeGen].

The GLES 1 data is currently manually updated. Find the relevant
file from the task output (see above) and overwrite the correspoding file.
Re-run [code generation][CodeGen] and create a CL as per our normal process.

To add a new configuration, first retrieve the JSON data, modify
[`gen_extensions.py`](../src/libANGLE/gen_extensions.py) as necessary, then
run [`scripts/run_code_generation.py`][CodeGen] to refresh generated files.
Also update `update_extension_data.py` as necessary.

[CodeGen]: ../scripts/run_code_generation.py
"""

_MD_GLES_GPU_CONFIGS = [
    'NVIDIA 1660 Win10',
    'Intel 630 Win10',
    'NVIDIA 1660 Linux',
    'Intel 630 Linux',
    'SwiftShader Win10',
    'Pixel 4 Android 11',
]

_MD_GLES1_GPU_CONFIGS = [
    'SwiftShader Win10',
]

_MD_TABLE_HEADER_TEMPLATE = """\
| Extension Name | {configs} |
| -------------- | {dashes} |"""

_MD_CONFIG_INFO_TEMPLATE = """\
{config}:

 * `GL_RENDERER` is `{Renderer}`
 * `GL_VENDOR` is `{Vendor}`
 * `GL_VERSION` is `{Version}`
 * Data updated {DateRecorded}
"""

_MD_GLES_EXT_LINK_TEMPLATE = """[{full_name}](https://khronos.org/registry/OpenGL/extensions/{vendor}/{vendor}_{link}.txt)"""
_MD_ANGLE_EXT_LINK_TEMPLATE = """[{full_name}](https://chromium.googlesource.com/angle/angle/+/refs/heads/main/extensions/{vendor}_{link}.txt)"""

# Some extensions are defined in documents that have different names.
_LINK_OVERRIDES = {
    'GL_EXT_memory_object_fd': 'external_objects_fd',
    'GL_EXT_semaphore_fd': 'external_objects_fd',
}


def get_camel_case(name_with_underscores):
    """ To follow ANGLE naming for member variables, we convert the canonical extension:
    0. Delete the API and vendor prefix.
    1. Capitalize letters after underscores.
    2. Delete underscores.
    3. Add back the vendor prefix to the end. """
    words = name_with_underscores.split('_')
    words = [words[0]] + [(word[0].upper() + word[1:]) for word in words[1:]]
    return ''.join(words)


def break_down_ext(ext, expr, mode):
    """ This splits an extension name like GL_EXT_buffer_storage into string components. """
    m = expr.match(ext)
    return {
        'full_name': ext,
        'api_prefix': m.group(1),
        'vendor': m.group(2),
        'name_with_underscores': m.group(3),
        'name_camel_case': get_camel_case(m.group(3)),
        'mode': mode,
        'link': _LINK_OVERRIDES.get(ext, m.group(3)),
    }


def break_down_exts(exts, expr, mode):
    return [break_down_ext(ext, expr, mode) for ext in exts]


def format_exts(ext_infos):
    return '\n'.join([_EXT_MEMBER_TEMPLATE.format(**ext_info) for ext_info in ext_infos])


def format_helper_function(ext_name, vendors):
    return _HELPER_TEMPLATE.format(
        ext_name=ext_name,
        expression=' || '.join(['%s%s' % (ext_name, vendor) for vendor in vendors]),
    )


def format_ext_strings(ext_infos):
    return '\n'.join([_EXT_STRING_TEMPLATE.format(**ext_info) for ext_info in ext_infos])


def write_file(fname, template, format_args):
    with open(fname, 'w') as f:
        formatted = template.format(**format_args)
        f.write(formatted)
        f.close()


def sort_by_ext_name(ext_infos):
    return sorted(ext_infos, key=lambda e: e['name_camel_case'].lower())


def get_ext_support(ext_name, gpu_data):

    def s(ext, support):
        SUPPORT_SYM = '&#x2714;'
        NOSUPPORT_SYM = ''
        return SUPPORT_SYM if ext in support['Extensions'] else NOSUPPORT_SYM

    return ' | '.join([s(ext_name, support) for support in gpu_data])


def get_md_table_header(md_gpu_configs):
    configs = ' | '.join(md_gpu_configs)
    dashes = ' | '.join([(':%s:' % ('-' * (len(config) - 2))) for config in md_gpu_configs])
    return _MD_TABLE_HEADER_TEMPLATE.format(configs=configs, dashes=dashes)


def format_md_gpu_info(gpu_data):
    return _MD_CONFIG_INFO_TEMPLATE.format(**gpu_data)


def format_md_link(ext_info, link_template):
    return link_template.format(**ext_info)


def format_md_ext(ext_info, gpu_json_data, link_template):
    return '| %s | %s |' % (format_md_link(
        ext_info, link_template), get_ext_support(ext_info['full_name'], gpu_json_data))


def format_md_exts(ext_infos, gpu_json_data, link_template):
    return '\n'.join(
        [format_md_ext(ext_info, gpu_json_data, link_template) for ext_info in ext_infos])


def main():
    # auto_script parameters.
    data_source_name = 'registry_xml.py and gl.xml'
    gles_h_output_name = 'gles_extensions_autogen.h'
    gles_cpp_output_name = 'gles_extensions_autogen.cpp'
    md_output_name = '../../doc/ExtensionSupport.md'
    ext_jsons = [
        '../../scripts/extension_data/%s.json' % s.lower().replace(' ', '_')
        for s in _MD_GLES_GPU_CONFIGS
    ]
    gles1_ext_jsons = [
        '../../scripts/extension_data/%s_gles1.json' % s.lower().replace(' ', '_')
        for s in _MD_GLES1_GPU_CONFIGS
    ]
    if len(sys.argv) > 1:
        inputs = ['../../scripts/%s' % xml_input for xml_input in registry_xml.xml_inputs
                 ] + ext_jsons + gles1_ext_jsons
        outputs = [gles_h_output_name, gles_cpp_output_name, md_output_name]
        if sys.argv[1] == 'inputs':
            print(','.join(inputs))
        elif sys.argv[1] == 'outputs':
            print(','.join(outputs))
        else:
            print('Invalid script parameters.')
            return 1
        return 0

    expr = re.compile(r'^([A-Z]+)_([A-Z]+)_(\w+)$')

    angle_ext_infos = (
        break_down_exts(registry_xml.angle_requestable_extensions, expr, REQUESTABLE) +
        break_down_exts(registry_xml.angle_es_only_extensions, expr, ESONLY) +
        break_down_exts(registry_xml.angle_toggleable_extensions, expr, TOGGLEABLE))

    angle_ext_infos = sort_by_ext_name(angle_ext_infos)

    gles_ext_infos = (
        break_down_exts(registry_xml.gles_requestable_extensions, expr, REQUESTABLE) +
        break_down_exts(registry_xml.gles_es_only_extensions, expr, ESONLY))

    gles_ext_infos = sort_by_ext_name(gles_ext_infos)

    gles1_ext_infos = break_down_exts(registry_xml.gles1_extensions, expr, REQUESTABLE)

    gles1_ext_infos = sort_by_ext_name(gles1_ext_infos)

    ext_infos = angle_ext_infos + gles_ext_infos + gles1_ext_infos

    ext_name_to_vendors = {}
    for info in ext_infos:
        ext_name = info['name_camel_case']
        if ext_name in ext_name_to_vendors:
            ext_name_to_vendors[ext_name] += [info['vendor']]
        else:
            ext_name_to_vendors[ext_name] = [info['vendor']]

    helper_function_data = []
    for (ext_name, vendors) in sorted(ext_name_to_vendors.items()):
        if len(vendors) > 1:
            helper_function_data += [format_helper_function(ext_name, vendors)]

    helper_functions = '\n'.join(helper_function_data)

    gles_gpu_data = []
    for (gpu_config, ext_json) in zip(_MD_GLES_GPU_CONFIGS, ext_jsons):
        with open(ext_json) as f:
            config_support = json.loads(f.read())
            config_support['config'] = gpu_config
            gles_gpu_data.append(config_support)

    gles1_gpu_data = []
    for (gpu_config, ext_json) in zip(_MD_GLES1_GPU_CONFIGS, gles1_ext_jsons):
        with open(ext_json) as f:
            config_support = json.loads(f.read())
            config_support['config'] = gpu_config
            gles1_gpu_data.append(config_support)

    gles_md_exts = format_md_exts(gles_ext_infos, gles_gpu_data, _MD_GLES_EXT_LINK_TEMPLATE)
    angle_md_exts = format_md_exts(angle_ext_infos, gles_gpu_data, _MD_ANGLE_EXT_LINK_TEMPLATE)
    gles1_md_exts = format_md_exts(gles1_ext_infos, gles1_gpu_data, _MD_GLES_EXT_LINK_TEMPLATE)
    md_gpu_info = [format_md_gpu_info(gpu_data) for gpu_data in gles_gpu_data]

    format_args = {
        'script_name': os.path.basename(__file__),
        'data_source_name': os.path.basename(data_source_name),
        'filename': gles_h_output_name,
        'gles_extensions': format_exts(gles_ext_infos),
        'angle_extensions': format_exts(angle_ext_infos),
        'gles1_extensions': format_exts(gles1_ext_infos),
        'helper_functions': helper_functions,
        'angle_strings': format_ext_strings(angle_ext_infos),
        'gles_strings': format_ext_strings(gles_ext_infos),
        'gles1_strings': format_ext_strings(gles1_ext_infos),
        'gles_md_table_header': get_md_table_header(_MD_GLES_GPU_CONFIGS),
        'gles_md_exts': gles_md_exts,
        'angle_md_table_header': get_md_table_header(_MD_GLES_GPU_CONFIGS),
        'angle_md_exts': angle_md_exts,
        'gles1_md_table_header': get_md_table_header(_MD_GLES1_GPU_CONFIGS),
        'gles1_md_exts': gles1_md_exts,
        'md_gpu_info': '\n'.join(md_gpu_info),
    }

    write_file(gles_h_output_name, _GLES_EXTENSIONS_TEMPLATE, format_args)
    write_file(gles_cpp_output_name, _GLES_EXT_STRINGS_TEMPLATE, format_args)
    write_file(md_output_name, _MARKDOWN_TEMPLATE, format_args)

    return 0


if __name__ == '__main__':
    sys.exit(main())
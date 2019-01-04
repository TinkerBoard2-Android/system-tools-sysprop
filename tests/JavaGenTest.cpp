/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <unistd.h>
#include <string>

#include <android-base/file.h>
#include <android-base/test_utils.h>
#include <gtest/gtest.h>

#include "JavaGen.h"

namespace {

constexpr const char* kTestSyspropFile =
    R"(owner: Vendor
module: "com.somecompany.TestProperties"

prop {
    api_name: "test_double"
    type: Double
    prop_name: "vendor.test_double"
    scope: Internal
    access: ReadWrite
}
prop {
    api_name: "test_int"
    type: Integer
    prop_name: "vendor.test_int"
    scope: Public
    access: ReadWrite
}
prop {
    api_name: "test.string"
    type: String
    prop_name: "vendor.test.string"
    scope: System
    access: ReadWrite
}

prop {
    api_name: "test.enum"
    type: Enum
    prop_name: "vendor.test.enum"
    enum_values: "a|b|c|D|e|f|G"
    scope: Internal
    access: ReadWrite
}
prop {
    api_name: "test_BOOLeaN"
    type: Boolean
    prop_name: "ro.vendor.test.b"
    scope: Public
    access: Writeonce
}
prop {
    api_name: "vendor.os_test-long"
    type: Long
    scope: System
    access: ReadWrite
}

prop {
    api_name: "test_double_list"
    type: DoubleList
    scope: Internal
    access: ReadWrite
}
prop {
    api_name: "test_list_int"
    type: IntegerList
    scope: Public
    access: ReadWrite
}
prop {
    api_name: "test.strlist"
    type: StringList
    scope: System
    access: ReadWrite
}

prop {
    api_name: "el"
    type: EnumList
    enum_values: "enu|mva|lue"
    scope: Internal
    access: ReadWrite
}
)";

constexpr const char* kExpectedJavaOutput =
    R"(// Generated by the sysprop generator. DO NOT EDIT!

package com.somecompany;

import android.annotation.SystemApi;

import android.os.SystemProperties;
import java.util.ArrayList;
import java.util.function.Function;
import java.util.List;
import java.util.Optional;
import java.util.StringJoiner;

public final class TestProperties {
    private TestProperties () {}

    private static Boolean tryParseBoolean(String str) {
        switch (str.toLowerCase()) {
            case "1":
            case "true":
                return Boolean.TRUE;
            case "0":
            case "false":
                return Boolean.FALSE;
            default:
                return null;
        }
    }

    private static Integer tryParseInteger(String str) {
        try {
            return Integer.valueOf(str);
        } catch (NumberFormatException e) {
            return null;
        }
    }

    private static Long tryParseLong(String str) {
        try {
            return Long.valueOf(str);
        } catch (NumberFormatException e) {
            return null;
        }
    }

    private static Double tryParseDouble(String str) {
        try {
            return Double.valueOf(str);
        } catch (NumberFormatException e) {
            return null;
        }
    }

    private static String tryParseString(String str) {
        return "".equals(str) ? null : str;
    }

    private static <T extends Enum<T>> T tryParseEnum(Class<T> enumType, String str) {
        try {
            return Enum.valueOf(enumType, str);
        } catch (IllegalArgumentException e) {
            return null;
        }
    }

    private static <T> List<T> tryParseList(Function<String, T> elementParser, String str) {
        if ("".equals(str)) return new ArrayList<>();

        List<T> ret = new ArrayList<>();

        for (String element : str.split(",")) {
            ret.add(elementParser.apply(element));
        }

        return ret;
    }

    private static <T extends Enum<T>> List<T> tryParseEnumList(Class<T> enumType, String str) {
        if ("".equals(str)) return new ArrayList<>();

        List<T> ret = new ArrayList<>();

        for (String element : str.split(",")) {
            ret.add(tryParseEnum(enumType, element));
        }

        return ret;
    }

    private static <T> String formatList(List<T> list) {
        StringJoiner joiner = new StringJoiner(",");

        for (T element : list) {
            joiner.add(element == null ? "" : element.toString());
        }

        return joiner.toString();
    }

    private static <T extends Enum<T>> String formatEnumList(List<T> list, Function<T, String> elementFormatter) {
        StringJoiner joiner = new StringJoiner(",");

        for (T element : list) {
            joiner.add(element == null ? "" : elementFormatter.apply(element));
        }

        return joiner.toString();
    }

    /** @hide */
    public static Optional<Double> test_double() {
        String value = SystemProperties.get("vendor.test_double");
        return Optional.ofNullable(tryParseDouble(value));
    }

    /** @hide */
    public static void test_double(Double value) {
        SystemProperties.set("vendor.test_double", value == null ? "" : value.toString());
    }

    public static Optional<Integer> test_int() {
        String value = SystemProperties.get("vendor.test_int");
        return Optional.ofNullable(tryParseInteger(value));
    }

    /** @hide */
    public static void test_int(Integer value) {
        SystemProperties.set("vendor.test_int", value == null ? "" : value.toString());
    }

    /** @hide */
    @SystemApi
    public static Optional<String> test_string() {
        String value = SystemProperties.get("vendor.test.string");
        return Optional.ofNullable(tryParseString(value));
    }

    /** @hide */
    public static void test_string(String value) {
        SystemProperties.set("vendor.test.string", value == null ? "" : value.toString());
    }

    /** @hide */
    public static enum test_enum_values {
        A("a"),
        B("b"),
        C("c"),
        D("D"),
        E("e"),
        F("f"),
        G("G");
        private final String propValue;
        private test_enum_values(String propValue) {
            this.propValue = propValue;
        }
        public String getPropValue() {
            return propValue;
        }
    }

    /** @hide */
    public static Optional<test_enum_values> test_enum() {
        String value = SystemProperties.get("vendor.test.enum");
        return Optional.ofNullable(tryParseEnum(test_enum_values.class, value));
    }

    /** @hide */
    public static void test_enum(test_enum_values value) {
        SystemProperties.set("vendor.test.enum", value == null ? "" : value.getPropValue());
    }

    public static Optional<Boolean> test_BOOLeaN() {
        String value = SystemProperties.get("ro.vendor.test.b");
        return Optional.ofNullable(tryParseBoolean(value));
    }

    /** @hide */
    public static void test_BOOLeaN(Boolean value) {
        SystemProperties.set("ro.vendor.test.b", value == null ? "" : value.toString());
    }

    /** @hide */
    @SystemApi
    public static Optional<Long> vendor_os_test_long() {
        String value = SystemProperties.get("vendor.vendor.os_test-long");
        return Optional.ofNullable(tryParseLong(value));
    }

    /** @hide */
    public static void vendor_os_test_long(Long value) {
        SystemProperties.set("vendor.vendor.os_test-long", value == null ? "" : value.toString());
    }

    /** @hide */
    public static List<Double> test_double_list() {
        String value = SystemProperties.get("vendor.test_double_list");
        return tryParseList(v -> tryParseDouble(v), value);
    }

    /** @hide */
    public static void test_double_list(List<Double> value) {
        SystemProperties.set("vendor.test_double_list", value == null ? "" : formatList(value));
    }

    public static List<Integer> test_list_int() {
        String value = SystemProperties.get("vendor.test_list_int");
        return tryParseList(v -> tryParseInteger(v), value);
    }

    /** @hide */
    public static void test_list_int(List<Integer> value) {
        SystemProperties.set("vendor.test_list_int", value == null ? "" : formatList(value));
    }

    /** @hide */
    @SystemApi
    public static List<String> test_strlist() {
        String value = SystemProperties.get("vendor.test.strlist");
        return tryParseList(v -> tryParseString(v), value);
    }

    /** @hide */
    public static void test_strlist(List<String> value) {
        SystemProperties.set("vendor.test.strlist", value == null ? "" : formatList(value));
    }

    /** @hide */
    public static enum el_values {
        ENU("enu"),
        MVA("mva"),
        LUE("lue");
        private final String propValue;
        private el_values(String propValue) {
            this.propValue = propValue;
        }
        public String getPropValue() {
            return propValue;
        }
    }

    /** @hide */
    public static List<el_values> el() {
        String value = SystemProperties.get("vendor.el");
        return tryParseEnumList(el_values.class, value);
    }

    /** @hide */
    public static void el(List<el_values> value) {
        SystemProperties.set("vendor.el", value == null ? "" : formatEnumList(value, el_values::getPropValue));
    }
}
)";

}  // namespace

using namespace std::string_literals;

TEST(SyspropTest, JavaGenTest) {
  TemporaryFile temp_file;

  // strlen is optimized for constants, so don't worry about it.
  ASSERT_EQ(write(temp_file.fd, kTestSyspropFile, strlen(kTestSyspropFile)),
            strlen(kTestSyspropFile));
  close(temp_file.fd);
  temp_file.fd = -1;

  TemporaryDir temp_dir;

  std::string err;
  ASSERT_TRUE(GenerateJavaLibrary(temp_file.path, temp_dir.path, &err));
  ASSERT_TRUE(err.empty());

  std::string java_output_path =
      temp_dir.path + "/com/somecompany/TestProperties.java"s;

  std::string java_output;
  ASSERT_TRUE(
      android::base::ReadFileToString(java_output_path, &java_output, true));
  EXPECT_EQ(java_output, kExpectedJavaOutput);

  unlink(java_output_path.c_str());
  rmdir((temp_dir.path + "/com/somecompany"s).c_str());
  rmdir((temp_dir.path + "/com"s).c_str());
}

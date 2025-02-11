/*
 * Copyright (c) 2023, Simon Wanner <simon@skyrising.xyz>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Format.h>
#include <LibJS/Forward.h>

namespace JS::Bytecode {

// TitleCaseName, snake_case_name, base, property, argument_count
#define JS_ENUMERATE_BUILTINS(O) \
    O(MathAbs, math_abs, Math, abs, 1)

enum class Builtin {
#define DEFINE_BUILTIN_ENUM(name, ...) name,
    JS_ENUMERATE_BUILTINS(DEFINE_BUILTIN_ENUM)
#undef DEFINE_BUILTIN_ENUM
        __Count,
};

static StringView builtin_name(Builtin value)
{
    switch (value) {
#define DEFINE_BUILTIN_CASE(name, snake_case_name, base, property, ...) \
    case Builtin::name:                                                 \
        return #base "." #property##sv;
        JS_ENUMERATE_BUILTINS(DEFINE_BUILTIN_CASE)
#undef DEFINE_BUILTIN_CASE
    case Builtin::__Count:
        VERIFY_NOT_REACHED();
    }
    VERIFY_NOT_REACHED();
}

inline size_t builtin_argument_count(Builtin value)
{
    switch (value) {
#define DEFINE_BUILTIN_CASE(name, snake_case_name, base, property, arg_count, ...) \
    case Builtin::name:                                                            \
        return arg_count;
        JS_ENUMERATE_BUILTINS(DEFINE_BUILTIN_CASE)
#undef DEFINE_BUILTIN_CASE
    case Builtin::__Count:
        VERIFY_NOT_REACHED();
    }
    VERIFY_NOT_REACHED();
}

Optional<Builtin> get_builtin(MemberExpression const& expression);

}

namespace AK {

template<>
struct Formatter<JS::Bytecode::Builtin> : Formatter<StringView> {
    ErrorOr<void> format(FormatBuilder& builder, JS::Bytecode::Builtin value)
    {
        return Formatter<StringView>::format(builder, builtin_name(value));
    }
};

}

/*
 * Copyright (C) 2015-2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "B3PatchpointValue.h"

#if ENABLE(B3_JIT)

namespace JSC { namespace B3 {

PatchpointValue::~PatchpointValue() = default;

void PatchpointValue::dumpMeta(CommaPrinter& comma, PrintStream& out) const
{
    Base::dumpMeta(comma, out);
    out.print(comma, "resultConstraints = ");
    out.print(resultConstraints.size() > 1 ? "[" : "");

    CommaPrinter constraintComma;
    for (const auto& constraint : resultConstraints)
        out.print(constraintComma, constraint);
    out.print(resultConstraints.size() > 1 ? "]" : "");

    if (numGPScratchRegisters)
        out.print(comma, "numGPScratchRegisters = ", numGPScratchRegisters);
    if (numFPScratchRegisters)
        out.print(comma, "numFPScratchRegisters = ", numFPScratchRegisters);
}

PatchpointValue::PatchpointValue(Type type, Origin origin)
    : Base(CheckedOpcode, Patchpoint, type, origin)
    , effects(Effects::forCall())
{
    if (!type.isTuple())
        resultConstraints.append(type == Void ? ValueRep::WarmAny : ValueRep::SomeRegister);
}

} } // namespace JSC::B3

#endif // ENABLE(B3_JIT)

/*
 * Copyright (C) 2017-2020 Apple Inc. All rights reserved.
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
#include "ProxyableAccessCase.h"

#if ENABLE(JIT)

namespace JSC {

ProxyableAccessCase::ProxyableAccessCase(VM& vm, JSCell* owner, AccessType accessType, CacheableIdentifier identifier, PropertyOffset offset, Structure* structure,
    const ObjectPropertyConditionSet& conditionSet, bool viaProxy, WatchpointSet* additionalSet, RefPtr<PolyProtoAccessChain>&& prototypeAccessChain)
    : Base(vm, owner, accessType, identifier, offset, structure, conditionSet, WTFMove(prototypeAccessChain))
    , m_additionalSet(additionalSet)
{
    m_viaProxy = viaProxy;
}

Ref<AccessCase> ProxyableAccessCase::create(VM& vm, JSCell* owner, AccessType type, CacheableIdentifier identifier, PropertyOffset offset, Structure* structure, const ObjectPropertyConditionSet& conditionSet, bool viaProxy, WatchpointSet* additionalSet, RefPtr<PolyProtoAccessChain>&& prototypeAccessChain)
{
    ASSERT(type == Load || type == Miss || type == GetGetter || type == Replace);
    return adoptRef(*new ProxyableAccessCase(vm, owner, type, identifier, offset, structure, conditionSet, viaProxy, additionalSet, WTFMove(prototypeAccessChain)));
}

ProxyableAccessCase::~ProxyableAccessCase()
{
}

Ref<AccessCase> ProxyableAccessCase::clone() const
{
    auto result = adoptRef(*new ProxyableAccessCase(*this));
    result->resetState();
    return result;
}

void ProxyableAccessCase::dumpImpl(PrintStream& out, CommaPrinter& comma, Indenter& indent) const
{
    Base::dumpImpl(out, comma, indent);
    out.print(comma, "viaProxy = ", viaProxy());
    out.print(comma, "additionalSet = ", RawPointer(additionalSet()));
}

} // namespace JSC

#endif // ENABLE(JIT)
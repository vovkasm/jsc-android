/*
 * Copyright (C) 2008-2019 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "VirtualRegister.h"

#include <wtf/Assertions.h>
#include <wtf/VectorTraits.h>

namespace JSC {

    class RegisterID {
        WTF_MAKE_NONCOPYABLE(RegisterID);

        friend class VirtualRegister;
    public:
        RegisterID() = default;

        RegisterID(VirtualRegister virtualRegister)
            : m_virtualRegister(virtualRegister)
#if ASSERT_ENABLED
            , m_didSetIndex(true)
#endif
        {
        }
        
        explicit RegisterID(int index)
            : m_virtualRegister(VirtualRegister(index))
#if ASSERT_ENABLED
            , m_didSetIndex(true)
#endif
        {
        }

        void setIndex(VirtualRegister index)
        {
#if ASSERT_ENABLED
            m_didSetIndex = true;
#endif
            m_virtualRegister = index;
        }

        void setTemporary()
        {
            m_isTemporary = true;
        }

        int index() const
        {
            ASSERT(m_didSetIndex);
            return m_virtualRegister.offset();
        }

        VirtualRegister virtualRegister() const
        {
            ASSERT(m_virtualRegister.isValid());
            return m_virtualRegister;
        }

        bool isTemporary()
        {
            return m_isTemporary;
        }

        void ref()
        {
            ++m_refCount;
        }

        void deref()
        {
            --m_refCount;
            ASSERT(m_refCount >= 0);
        }

        int refCount() const
        {
            return m_refCount;
        }

    private:

        int m_refCount { 0 };
        VirtualRegister m_virtualRegister;
        bool m_isTemporary { false };
#if ASSERT_ENABLED
        bool m_didSetIndex { false };
#endif
    };
} // namespace JSC

namespace WTF {

    template<> struct VectorTraits<JSC::RegisterID> : VectorTraitsBase<true, JSC::RegisterID> {
        static constexpr bool needsInitialization = true;
        static constexpr bool canInitializeWithMemset = true; // Default initialization just sets everything to 0 or false, so this is safe.
    };

} // namespace WTF

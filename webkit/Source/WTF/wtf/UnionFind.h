/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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

#pragma once

#include <wtf/Assertions.h>

namespace WTF {

// A UnionFind class can be used to compute disjoint sets using the
// disjoint-set forest data structure. Each UnionFind instance is a
// node in the forest. Typically you use it by using UnionFind as a
// superclass:
//
// class MemberOfSet : public UnionFind<MemberOfSet> { ... }
//
// Calling x->find() gives you a MemberOfSet* that represents the
// disjoint set that x belongs to. Calling x->unify(y) unifies x's
// set with y's set, and ensures that:
//
// x->find() == y->find()
//
// and that:
//
// a->find() == b->find()
//
// for any a, b if prior to the call to x->unify(y), we would have
// had:
//
// a->find() == x
// b->find() == y
//
// This implementation is almost amortized O(1), but could be worse
// in unlikely pathological cases. It favors having a non-recursive
// single pass implementation of unify() and find() over ensuring the
// theoretical O(InverseAckermann[n]) amortized bound, which is much
// closer to amortized O(1).

template<typename T>
class UnionFind {
public:
    UnionFind() = default;
    
    bool isRoot() const
    {
        bool result = !m_parent;
        ASSERT(result == (const_cast<UnionFind<T>*>(this)->find() == this));
        return result;
    }
    
    T* find()
    {
        T* result = static_cast<T*>(this);
        T* next = result->m_parent;
        while (next) {
            result = next;
            next = result->m_parent;
        }
        ASSERT(result);
        if (result != this)
            m_parent = result;
        return result;
    }
    
    void unify(T* other)
    {
        T* a = static_cast<T*>(this)->find();
        T* b = other->find();
        
        ASSERT(!a->m_parent);
        ASSERT(!b->m_parent);
        
        if (a == b)
            return;
        
        a->m_parent = b;
    }
private:
    T* m_parent { nullptr };
};

} // namespace WTF

using WTF::UnionFind;

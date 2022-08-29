/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(GPU_PROCESS) && ENABLE(VIDEO)

#include <wtf/Ref.h>
#include <wtf/ThreadSafeRefCounted.h>

#if PLATFORM(COCOA)
#include "RemoteVideoFrameObjectHeapProxyProcessor.h"
#endif

namespace WebKit {

class GPUProcessConnection;

#if PLATFORM(COCOA)
class RemoteVideoFrameProxy;
#endif

// Wrapper around RemoteVideoFrameObjectHeapProxyProcessor that will always be destroeyd on main thread.
class RemoteVideoFrameObjectHeapProxy : public ThreadSafeRefCounted<RemoteVideoFrameObjectHeapProxy, WTF::DestructionThread::MainRunLoop> {
public:
    static Ref<RemoteVideoFrameObjectHeapProxy> create(GPUProcessConnection& connection) { return adoptRef(*new RemoteVideoFrameObjectHeapProxy(connection)); }
    ~RemoteVideoFrameObjectHeapProxy() = default;

#if PLATFORM(COCOA)
    void getVideoFrameBuffer(const RemoteVideoFrameProxy& proxy, RemoteVideoFrameObjectHeapProxyProcessor::Callback&& callback) { m_processor->getVideoFrameBuffer(proxy, WTFMove(callback)); }
#endif

private:
    explicit RemoteVideoFrameObjectHeapProxy(GPUProcessConnection& connection)
#if PLATFORM(COCOA)
        : m_processor(RemoteVideoFrameObjectHeapProxyProcessor::create(connection))
#endif
    {
    }
#if PLATFORM(COCOA)
    Ref<RemoteVideoFrameObjectHeapProxyProcessor> m_processor;
#endif
};

} // namespace WebKit

#endif
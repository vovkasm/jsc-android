/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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

#if ENABLE(IMAGE_ANALYSIS)

#include <wtf/FastMalloc.h>
#include <wtf/PriorityQueue.h>
#include <wtf/WeakHashSet.h>
#include <wtf/WeakPtr.h>

namespace WebCore {

class Document;
class HTMLImageElement;
class Page;
class Timer;

class ImageAnalysisQueue {
    WTF_MAKE_FAST_ALLOCATED;
public:
    ImageAnalysisQueue(Page&);
    ~ImageAnalysisQueue();

    WEBCORE_EXPORT void enqueueAllImages(Document&, const String& identifier);
    void clear();

    void enqueueIfNeeded(HTMLImageElement&);

private:
    void resumeProcessingSoon();
    void resumeProcessing();

    enum class Priority : bool { Low, High };
    struct Task {
        WeakPtr<HTMLImageElement> element;
        Priority priority { Priority::Low };
        unsigned taskNumber { 0 };
    };

    static bool firstIsHigherPriority(const Task&, const Task&);
    unsigned nextTaskNumber() { return ++m_currentTaskNumber; }

    String m_identifier;
    WeakPtr<Page> m_page;
    Timer m_resumeProcessingTimer;
    WeakHashSet<HTMLImageElement> m_queuedElements;
    PriorityQueue<Task, firstIsHigherPriority> m_queue;
    unsigned m_pendingRequestCount { 0 };
    unsigned m_currentTaskNumber { 0 };
};

inline bool ImageAnalysisQueue::firstIsHigherPriority(const Task& first, const Task& second)
{
    if (first.priority != second.priority)
        return first.priority == Priority::High;

    return first.taskNumber < second.taskNumber;
}

} // namespace WebCore

#endif // ENABLE(IMAGE_ANALYSIS)

/*
 * Copyright (C) 2017 Yusuke Suzuki <utatane.tea@gmail.com>.
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

// We keep this function small very carefully to encourage inlining.
@linkTimeConstant
function mapIteratorNext(bucket, kind)
{
    "use strict";
    var value;

    bucket = @mapBucketNext(bucket);
    @putMapIteratorInternalField(this, @mapIteratorFieldMapBucket, bucket);
    var done = bucket === @sentinelMapBucket;
    if (!done) {
        var key = @mapBucketKey(bucket);
        value = @mapBucketValue(bucket);
        if (kind === @iterationKindEntries)
            value = [ key, value ]
        else if (kind === @iterationKindKey)
            value = key;
    }
    return { value, done };
}

function next()
{
    "use strict";

    if (!@isMapIterator(this))
        @throwTypeError("%MapIteratorPrototype%.next requires that |this| be an Map Iterator instance");

    var bucket = @getMapIteratorInternalField(this, @mapIteratorFieldMapBucket);
    var kind = @getMapIteratorInternalField(this, @mapIteratorFieldKind);
    return @mapIteratorNext.@call(this, bucket, kind);
}

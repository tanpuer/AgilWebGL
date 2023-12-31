// <ALIOS> adapt for webengin 5.7: begin
// This file is generated by Exported_h.template.

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Schema_api_h
#define v8_inspector_protocol_Schema_api_h

#include "v8-inspector.h"

namespace v8_inspector {
namespace protocol {

#ifndef v8_inspector_protocol_exported_api_h
#define v8_inspector_protocol_exported_api_h
class V8_EXPORT Exported {
public:
    // <AliOS> change: begin
    virtual std::unique_ptr<StringBuffer> toJSONString() const = 0;
    virtual void writeBinary(std::vector<uint8_t>* out) const = 0;
    // <AliOS> change: end
    virtual ~Exported() { }
};
#endif // !defined(v8_inspector_protocol_exported_api_h)

namespace Schema {
namespace API {

// ------------- Enums.

// ------------- Types.

class V8_EXPORT Domain : public Exported {
public:
    static std::unique_ptr<protocol::Schema::API::Domain> fromJSONString(const StringView& json);
    static std::unique_ptr<protocol::Schema::API::Domain> fromBinary(const uint8_t* data, size_t length);
};

} // namespace API
} // namespace Schema
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Schema_api_h)
// <ALIOS> adapt for webengin 5.7: end

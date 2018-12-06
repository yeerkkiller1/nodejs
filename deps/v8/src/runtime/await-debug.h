#pragma once

#include "src/inspector/v8-inspector-impl.h"

class IsolateString {
public:
  bool is8Bit = false;
  uint8_t* data8 = nullptr;
  uint16_t* data16 = nullptr;
  size_t length = 0;

public:
  IsolateString(const v8_inspector::StringView& text);
  // text is always 16 bit, as v8_inspector::protocol::String is just
  IsolateString(const v8_inspector::protocol::String& text): IsolateString(v8_inspector::StringView(text.characters16(), text.length())) { }

  IsolateString(IsolateString&& other) {
    this->accept(other);
  }
  IsolateString& operator=(IsolateString&& other) {
    this->accept(other);
    return *this;
  }
  void accept(IsolateString& other) {
    this->is8Bit = other.is8Bit;
    this->data8 = other.data8;
    this->data16 = other.data16;
    this->length = other.length;

    other.data8 = nullptr;
    other.data16 = nullptr;
    other.length = 0;
  }
  ~IsolateString();

  IsolateString(const IsolateString& other) = delete;
  IsolateString* operator=(const IsolateString&) = delete;
};

class AwaitInfo {
public:
  size_t generatorId;
  IsolateString inspectorObject;
  int asyncExecutionId;

  AwaitInfo(
    size_t generatorId,
    const std::unique_ptr<v8_inspector::V8StackTraceImpl>& stack,
    int asyncExecutionId
  );

  AwaitInfo(AwaitInfo&& other): inspectorObject(std::move(other.inspectorObject)) {
    this->generatorId = other.generatorId;
    this->asyncExecutionId = other.asyncExecutionId;
  }

  AwaitInfo& operator=(AwaitInfo&& other) {
    this->generatorId = other.generatorId;
    this->inspectorObject = std::move(other.inspectorObject);
    this->asyncExecutionId = other.asyncExecutionId;
    return *this;
  }

  AwaitInfo(const AwaitInfo& other) = delete;
  AwaitInfo* operator=(const AwaitInfo&) = delete;
};
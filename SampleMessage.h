// Copyright © 2017 Matt Robinson

#include "Message.h"

class SampleMessage : public Message
{
    friend Message Message::FromFrame(Frame frame);

protected:
    explicit SampleMessage(const std::vector<uint8_t> &raw);
};

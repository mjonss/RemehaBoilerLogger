// Copyright © 2017 Matt Robinson

#include <cstring>
#include <stdexcept>

#include "Frame.h"

static const uint8_t START_BYTE = 0x02;
static const uint8_t END_BYTE   = 0x03;

static const int length_adjust = 2;

struct Header
{
    uint8_t start : 8;
    uint16_t : 16;
    uint8_t length : 8;
    uint16_t function : 16;
} __attribute__((__packed__));

struct Trailer
{
    uint8_t csum : 8;
    uint8_t end : 8;
} __attribute__((__packed__));

Frame::Frame(const std::vector<uint8_t> &raw)
{
    if(raw.size() < sizeof(Header) + sizeof(Trailer))
    {
        throw std::runtime_error("Not enough data for a valid frame");
    }

    struct Header header;
    memcpy(&header, &raw[0], sizeof(header));

    if(header.start != START_BYTE)
    {
        throw std::runtime_error("Incorrect start byte value");
    }

    if(header.length + length_adjust != raw.size())
    {
        throw std::runtime_error("Header length does not match actual length");
    }

    function = header.function;

    struct Trailer trailer;
    memcpy(&trailer, &raw[raw.size() - sizeof(trailer)], sizeof(trailer));

    uint8_t csum = 0;

    for(int i = 1; i < raw.size() - 2; i++)
    {
        csum = csum ^ raw[i];
    }

    if(trailer.csum != csum)
    {
        throw std::runtime_error("Trailer checksum does not match calculated");
    }

    if(trailer.end != END_BYTE)
    {
        throw std::runtime_error("Incorrect end byte value");
    }

    data = std::vector<uint8_t>(raw.begin() + sizeof(header), raw.begin() + (raw.size() - sizeof(trailer)));
}

uint16_t Frame::getFunction()
{
    return this->function;
}

std::vector<uint8_t> Frame::getData()
{
    return this->data;
}

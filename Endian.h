#ifndef ENDIAN_H
#define ENDIAN_H

#include <cstdint>
#include <climits>

#if 0 // not standards compliant
template <typename T>
T swapEndian(T value)
{
    union ByteMask
    {
        T value;
        uint8_t bytes[sizeof(T)];
    };
    
    const ByteMask *in = &value;
    ByteMask out;

    for (size_t i = 0; i < sizeof(T); i++)
    {
        out.bytes[i] = in->bytes[sizeof(T) - i - 1];
    }

    return out.value;
}
#endif

// Generic implementation for any size integer
template <typename T>
T swapEndian(T value)
{
    T out = 0;
    for (size_t i = 0; i < sizeof(T); i++)
    {
        out <<= CHAR_BIT;
        out |= value & 0xFF;
        value >>= CHAR_BIT;
    }
    return out;
}

// More efficient specialization for uint32_t
template <>
uint32_t swapEndian(uint32_t value)
{
    return ((value & 0x000000FF) << 24) |
           ((value & 0x0000FF00) <<  8) |
           ((value & 0x00FF0000) >>  8) |
           ((value & 0xFF000000) >> 24);
}

// More efficient specialization for uint16_t
template <>
uint16_t swapEndian(uint16_t value)
{
	return ((value & 0x00FF) <<  8) |
           ((value & 0xFF00) >>  8);
}

// More efficient specialization for uint8_t (no effect, but provided to support generic code)
template <>
uint8_t swapEndian(uint8_t value)
{
	return value;
}

bool isLittleEndian()
{
    uint16_t canary = 1u;
    return reinterpret_cast<uint8_t*>(&canary)[0] == 1u;
}

template <typename T>
T toLittleEndian(T value)
{
    if (isLittleEndian())
    {
        return value;
    }
    else
    {
        return swapEndian<T>(value);
    }
}

template <typename T>
T fromLittleEndian(T value)
{
    if (isLittleEndian())
    {
        return value;
    }
    else
    {
        return swapEndian<T>(value);
    }
}

template <typename T>
T toBigEndian(T value)
{
    if (isLittleEndian())
    {
        return swapEndian<T>(value);
    }
    else
    {
        return value;
    }
}

template <typename T>
T fromBigEndian(const T value)
{
    if (isLittleEndian())
    {
        return swapEndian<T>(value);
    }
    else
    {
        return value;
    }
}

#endif // ENDIAN_H

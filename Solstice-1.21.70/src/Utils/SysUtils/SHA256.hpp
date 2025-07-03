#pragma once
//
// Created by vastrakai on 7/21/2024.
//

#include <array>
#include <string>

class SHA256 {
public:
    SHA256();
    void update(const unsigned char* data, size_t length);
    void update(const std::string& data);
    std::string final();
    static std::string hash(const std::string& data);

private:
    void transform(const unsigned char* data);
    static constexpr std::array<uint32_t, 64> k = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    static uint32_t rotr(uint32_t x, uint32_t n);
    static uint32_t choose(uint32_t e, uint32_t f, uint32_t g);
    static uint32_t majority(uint32_t a, uint32_t b, uint32_t c);
    static uint32_t sig0(uint32_t x);
    static uint32_t sig1(uint32_t x);

    std::array<uint32_t, 8> h = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    std::array<unsigned char, 64> buffer;
    size_t buffer_length = 0;
    uint64_t bit_length = 0;
};

inline SHA256::SHA256() {}

inline void SHA256::update(const unsigned char* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        buffer[buffer_length++] = data[i];
        if (buffer_length == buffer.size()) {
            transform(buffer.data());
            bit_length += 512;
            buffer_length = 0;
        }
    }
}

inline void SHA256::update(const std::string& data) {
    update(reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
}

inline std::string SHA256::final() {
    bit_length += buffer_length * 8;
    buffer[buffer_length++] = 0x80;
    if (buffer_length > 56) {
        while (buffer_length < 64) {
            buffer[buffer_length++] = 0x00;
        }
        transform(buffer.data());
        buffer_length = 0;
    }

    while (buffer_length < 56) {
        buffer[buffer_length++] = 0x00;
    }

    for (int i = 7; i >= 0; --i) {
        buffer[buffer_length++] = (bit_length >> (i * 8)) & 0xff;
    }

    transform(buffer.data());

    std::stringstream ss;
    for (auto val : h) {
        ss << std::hex << std::setw(8) << std::setfill('0') << val;
    }
    return ss.str();
}

inline std::string SHA256::hash(const std::string& data) {
    SHA256 sha256;
    sha256.update(data);
    return sha256.final();
}

inline void SHA256::transform(const unsigned char* data) {
    std::array<uint32_t, 64> w;
    for (size_t i = 0; i < 16; ++i) {
        w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);
    }
    for (size_t i = 16; i < 64; ++i) {
        w[i] = sig1(w[i - 2]) + w[i - 7] + sig0(w[i - 15]) + w[i - 16];
    }

    uint32_t a = h[0];
    uint32_t b = h[1];
    uint32_t c = h[2];
    uint32_t d = h[3];
    uint32_t e = h[4];
    uint32_t f = h[5];
    uint32_t g = h[6];
    uint32_t h0 = h[7];

    for (size_t i = 0; i < 64; ++i) {
        uint32_t t1 = h0 + sig1(e) + choose(e, f, g) + k[i] + w[i];
        uint32_t t2 = sig0(a) + majority(a, b, c);
        h0 = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
    h[5] += f;
    h[6] += g;
    h[7] += h0;
}

inline uint32_t SHA256::rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

inline uint32_t SHA256::choose(uint32_t e, uint32_t f, uint32_t g) {
    return (e & f) ^ (~e & g);
}

inline uint32_t SHA256::majority(uint32_t a, uint32_t b, uint32_t c) {
    return (a & b) ^ (a & c) ^ (b & c);
}

inline uint32_t SHA256::sig0(uint32_t x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

inline uint32_t SHA256::sig1(uint32_t x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}
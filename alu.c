#include "main.h"

inline uint16_t add(const uint16_t a, const uint16_t b, struct Flags *flags) {
    const uint32_t result = (uint32_t) a + (uint32_t) b;

    if (result > MAX) ERR("add overflow\n");
    const uint16_t main_result = (uint16_t) result;
    set_flags(flags, main_result);

    return main_result;
}
inline uint16_t sub(const uint16_t a, const uint16_t b, struct Flags *flags) {
    uint16_t result;
    if (a >= b) {
        result = a - b;
        flags->Negative = false;
    } else {
        result = b - a;
        flags->Negative = true;
    }
    flags->Zero = (result == 0);
    return result;
}
inline uint16_t inc(const uint16_t a, struct Flags *flags) {
    return add(a, 1, flags);
}
inline uint16_t dec(const uint16_t a, struct Flags *flags) {
    return sub(a, 1, flags);
}
inline uint16_t mul(const uint16_t a, const uint16_t b, struct Flags *flags) {
    const uint32_t result = (uint32_t) a * (uint32_t) b;
    const uint16_t main_result = (uint16_t) result;

    if (result > MAX) ERR("mul overflow\n");
    set_flags(flags, main_result);

    return main_result;
}
inline uint16_t divi(const uint16_t a, const uint16_t b, struct Flags *flags) {
    if (!b) ERR("divide by zero\n");
    const uint32_t result = (uint32_t) a / (uint32_t) b;
    const uint16_t main_result = (uint16_t) result;

    set_flags(flags, main_result);

    return main_result;
}
inline uint16_t mod(const uint16_t a, const uint16_t b, struct Flags *flags) {
    if (!b) ERR("mod by zero\n");
    const uint32_t result = (uint32_t) a % (uint32_t) b;
    const uint16_t main_result = (uint16_t) result;

    set_flags(flags, main_result);

    return main_result;
}
inline uint16_t and(const uint16_t a, const uint16_t b, struct Flags *flags) {
    set_flags(flags, (uint16_t) a & b);
    return (uint16_t) a & b;
}

inline uint16_t nand(const uint16_t a, const uint16_t b, struct Flags *flags) {
    set_flags(flags, (uint16_t) ~(a & b));
    return (uint16_t) ~(a & b);
}

inline uint16_t nor(const uint16_t a, const uint16_t b, struct Flags *flags) {
    set_flags(flags, (uint16_t) ~(a | b));
    return (uint16_t) ~(a | b);
}

inline uint16_t or(const uint16_t a, const uint16_t b, struct Flags *flags) {
    set_flags(flags, (uint16_t) a | b);
    return (uint16_t) a | b;
}

inline uint16_t xor(const uint16_t a, const uint16_t b, struct Flags *flags) {
    set_flags(flags, (uint16_t) a ^ b);
    return (uint16_t) a ^ b;
}

inline uint16_t not(const uint16_t a, struct Flags *flags) {
    set_flags(flags, (uint16_t) ~a);
    return (uint16_t) ~a;
}

inline uint16_t cmp(const uint16_t a, const uint16_t b, struct Flags *flags) {
    const uint16_t r = (uint16_t)(a - b);
    set_flags(flags, r);
    return r;
}

inline uint16_t shl(const uint16_t a, const uint16_t n, struct Flags *flags) {
    if (n >= 16) ERR("shl overflow\n");
    if (n == 0) return a;
    set_flags(flags, (uint16_t) a << n);
    return a << n;
}
inline uint16_t shr(const uint16_t a, const uint16_t n, struct Flags *flags) {
    if (n >= 16) ERR("shr overflow\n");
    if (n == 0) return a;
    set_flags(flags, (uint16_t) a >> n);
    return a >> n;
}
inline uint16_t sar(const uint16_t a, const uint16_t n, struct Flags *flags) {
    if (n >= 16) ERR("sar overflow\n");
    if (n == 0) return a;
    const uint16_t main_result = (uint16_t)((int16_t)a >> n);
    set_flags(flags, main_result);
    return main_result;
}

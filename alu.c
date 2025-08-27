#include "main.h"

inline uint32_t add(const uint32_t a, const uint32_t b, struct Flags *flags) {
    const uint64_t result = (uint64_t) a + (uint64_t) b;

    if (result > MAX) ERR("add overflow\n");
    const uint32_t main_result = (uint32_t) result;
    set_flags(flags, main_result);

    return main_result;
}
inline uint32_t sub(const uint32_t a, const uint32_t b, struct Flags *flags) {
    uint32_t result;
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
inline uint32_t inc(const uint32_t a, struct Flags *flags) {
    return add(a, 1, flags);
}
inline uint32_t dec(const uint32_t a, struct Flags *flags) {
    return sub(a, 1, flags);
}
inline uint32_t mul(const uint32_t a, const uint32_t b, struct Flags *flags) {
    const uint64_t result = (uint64_t) a * (uint64_t) b;
    const uint32_t main_result = (uint32_t) result;

    if (result > MAX) ERR("mul overflow\n");
    set_flags(flags, main_result);

    return main_result;
}
inline uint32_t divi(const uint32_t a, const uint32_t b, struct Flags *flags) {
    if (!b) ERR("divide by zero\n");
    const uint64_t result = (uint64_t) a / (uint64_t) b;
    const uint32_t main_result = (uint32_t) result;

    set_flags(flags, main_result);

    return main_result;
}
inline uint32_t mod(const uint32_t a, const uint32_t b, struct Flags *flags) {
    if (!b) ERR("mod by zero\n");
    const uint64_t result = (uint64_t) a % (uint64_t) b;
    const uint32_t main_result = (uint32_t) result;

    set_flags(flags, main_result);

    return main_result;
}
inline uint32_t and(const uint32_t a, const uint32_t b, struct Flags *flags) {
    set_flags(flags, (uint32_t) a & b);
    return (uint32_t) a & b;
}

inline uint32_t nand(const uint32_t a, const uint32_t b, struct Flags *flags) {
    set_flags(flags, (uint32_t) ~(a & b));
    return (uint32_t) ~(a & b);
}

inline uint32_t nor(const uint32_t a, const uint32_t b, struct Flags *flags) {
    set_flags(flags, (uint32_t) ~(a | b));
    return (uint32_t) ~(a | b);
}

inline uint32_t or(const uint32_t a, const uint32_t b, struct Flags *flags) {
    set_flags(flags, (uint32_t) a | b);
    return (uint32_t) a | b;
}

inline uint32_t xor(const uint32_t a, const uint32_t b, struct Flags *flags) {
    set_flags(flags, (uint32_t) a ^ b);
    return (uint32_t) a ^ b;
}

inline uint32_t not(const uint32_t a, struct Flags *flags) {
    set_flags(flags, (uint32_t) ~a);
    return (uint32_t) ~a;
}

inline uint32_t cmp(const uint32_t a, const uint32_t b, struct Flags *flags) {
    const uint32_t r = (uint32_t)(a - b);
    set_flags(flags, r);
    return r;
}

inline uint32_t shl(const uint32_t a, const uint32_t n, struct Flags *flags) {
    if (n >= 32) ERR("shl overflow\n");
    if (n == 0) return a;
    set_flags(flags, (uint32_t) a << n);
    return a << n;
}
inline uint32_t shr(const uint32_t a, const uint32_t n, struct Flags *flags) {
    if (n >= 32) ERR("shr overflow\n");
    if (n == 0) return a;
    set_flags(flags, (uint32_t) a >> n);
    return a >> n;
}
inline uint32_t sar(const uint32_t a, const uint32_t n, struct Flags *flags) {
    if (n >= 32) ERR("sar overflow\n");
    if (n == 0) return a;
    const uint32_t main_result = (uint32_t)((int32_t)a >> n);
    set_flags(flags, main_result);
    return main_result;
}
inline uint32_t sltu(const uint32_t a, const uint32_t b, struct Flags *flags) {
    uint32_t r = a < b;
    set_flags(flags, r);
    return r;
}


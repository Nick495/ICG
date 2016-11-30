# ICG
Inverse Congruential Generator

Usage: ./icg <b>modulus</b> <b>multiplier</b> <b>adder</b> <b>seed</b>

The ICG appears to work. The major issue at this point is that the mod_inv()
function only works correctly for signed long longs, as the Bezout pairs can
sometimes be negative. This property means we can only have signed long long
inputs, which reduces our input space from 64 bits to 63. It'd be nice if that
weren't the case.

#include <cstdint>  // uint64_t, uint32_t
#include <bitset>  //bitset for testing
#include <iostream>

// C1
uint64_t expand3(uint64_t input) { 
    /*
    Essentially, each bit needs to be shifted to the left, by 2, from the previous shift to its right, 
    starting at b0.
    We can do this by splitting the bits into two groups, then shifting each of those groups to where they need
    to be. Then, we take each of those groups, split them in two (4 total groups) and then shift them to where
    they need to be. Repeat this until finally we are splitting each bit up individually and putting it where it
    goes.
    
    For example: bit 0 stays at position b0, bit 1 is shifted 2 to the left from there (from b1 to b3),
    bit 2 is shifted from b2 to b6, which is 4 positions OR 2^2 positions after the previous shift. We can execute
    this by shifting everything by 4 from the start, then saving only the relevant bits, then shifting everything by
    2 from the start, and saving the relevant bits.

    The highest a bit could be shifted without being truncated in 8 bits is shifted by 4 positions, hence why the 
    shifts start at 4. The shifts are by powers of 2, so the highest shift in 64 bits would be the largest power 
    of 2 that's less than 64, or 2^5 = 32.

    Each bit is shifted to the position that is its position * 3, so the highest bit that's getting shifted without
    being truncated is the largest value such that value*3 < 64, which in this case is 21, since 21 * 3 = 63
    
    Based on the 8 bit algorithm:
    - The input << x shifts the current block(s) by x to their new positions
    - The input | combines the orignal input of not-yet shifted bits with the new input that includes
        shifted bits so we have the unshifted blocks as well as the shifted ones
    - The & value saves 1. the bits that still need to be shifted into place and 2. the shifted bits that are
        where they need to be

    Each shift moves a block of bits into place, and then the next shift will split that block into 2
    new blocks and put those blocks in place working down to blocks of size 1. Each shift will move all the
    bits by 2b spots, or get the positions in a block of b + 2b spots. For a shift of 4, the bits are moved 4 places so 
    2b = 4, and b = 2. For a shift of 8, each bit moves 8 spots or 2*4, so b = 4. We can use this to generate a mask to save every b number
    of bits, separated by 2b spaces.

    - A shift of 2 moves 1 bit into place, then leaves 2 spaces, then moves another bit into place, etc. So,
        the mask for a shift of 2 is a repeating pattern of: 1001 0010 0100 1001 = 0x9249
    - A shift of 4 moves 2 bits into the right block of space, then leaves 4 spaces, then another 2 bits, etc. 
        So, the mask for a shift of 4 would be: 0000 1100 0011 0000 1100 0011 = 0x0C30C3
    - A shift of 8 moves 4 bits into place, then leaves 8 spaces, etc. So the mask can be found as:
        0000 1111 0000 0000 1111 0000 0000 1111 = 0x0F00 F00F
    - For a shift of 16: 0000 0000 0000 0000 1111 1111 = 0x00FF 0000 FF00 00FF
    - For a shift of 32: 0xFFFF 0000 0000 FFFF
    */

    // The problem with the algorithm given for 8 bits is that it should have gotten rid of any bits
    // that would not truncated out of the final value and otherwise could mess up calculations
    input = input & 0x3FFFFF;  // Save only the first 22 bits (positions 0 to 21)

    input = (input | (input << 32)) & 0xFFFF00000000FFFF;
    input = (input | (input << 16)) & 0x00FF0000FF0000FF;
    input = (input | (input << 8)) & 0xF00F00F00F00F00F;
    input = (input | (input << 4)) & 0x30C30C30C30C30C3;
    input = (input | (input << 2)) & 0x9249249249249249;
    
    return input; 
}

// C2
//uint64_t expand(uint64_t input, uint32_t scale) { return 0; }
// I did not have time to try to complete C2

// with templates...
/*
template <uint32_t Scale>
uint64_t expand(uint64_t input) {
    return 0;
}
*/

int main() {
    std::cout << std::bitset<64>(expand3(0b0)) << "\n"; // output should be 0
    std::cout << std::bitset<64>(expand3(0b1)) << "\n"; // output should be 1
    std::cout << std::bitset<64>(expand3(0b10)) << "\n";  // output should be 1000
    std::cout << std::bitset<64>(expand3(0b10101)) << "\n";  // outpu should be 0001 0000 0100 0001
    std::cout << std::bitset<64>(expand3(0b100000000000000000000000)) << "\n";  // 24th bit, output should be 0
    std::cout << std::bitset<64>(expand3(0b1111)) << "\n";  // output should be 0010 0100 1001
    std::cout << std::bitset<64>(expand3(0b1111111111111111111111)) << "\n";  
    // output should be 1001 0010 0100 1001 | 0010 0100 1001 0010 | 0100 1001 0010 0100 | 1001 0010 0100 1001

    return 0;
}
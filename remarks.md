This is a running development journal. I have put entries in as they come to
mind.

## func_mm
- the program overall doesn't seem to put newlines in properly at the end 
of the files it writes out, but oh well. that's why test.sh uses
`diff -war` and not just `diff`. mea culpa!
- gprof says the vast majority of the time is from calls to mm_redc, 
without child calls. examining the ELF using arm objdump bears out that 
all the function calls in mm_redc exept for memcpy have been inlined by 
`-O3`. 
                                                       (see `functional.stats`)
## opt 
- i was able to get each call to mm_redc from 70ms down to 50ms by making 
   the following change:
   ```
     acc <- long_mult(N', T)
     acc <- acc mod R
   ```
   into:
   ```  
     acc <- half_mult(N', T)
   ```
 - if I want to get really serious I'll replace mult() itself with Karatsuba
   multiplication.
 - gprof now says that each call to mm_exp calls mm_redc approximately 4092 
   times, i.e., ~ 2 log_2(n) for 2048-bit n. this makes sense - each pass
   through the constant-time Montgomery ladder deals with one bit of the 
   exponent, which is generally in the environment of 2047 - 2044 bits, and
   calls mm_mult() twice (once for T1 and T2).        

   Literature indicates that exponentiation a^n generally has 
   theta(log_2(n)) multiplication operations, so there's little I can do to 
   improve beside replacing the Montgomery ladder with regular 
   (non-constant-time) square and multiplying. 

   the sliding-window method isn't suitable as you'd need to compute x^0, 
   x^3, ..., x^2047 for each symbol x (i.e., each chunk of the text to be 
   encrypted or decrypted)!

   so improvements will have to be made at the level of mult() and 
   mm_redc().
 - i got this number down to ~3060 calls by doing non-ladder exponentiation.
                                                      (see `noladder.stats`)
 - loop unrolling helped with speed somewhat.
                                                    (see `multunroll.stats`)
 - i then tried inlining the parts of redc to eliminate the memcpy() upon 
   routine completion, but we start to get diminishing returns here: 80.03
   seconds vs. 80.49 seconds from `multunroll.stats`.

   I'm not really up for doing Karatsuba multiplication because either 
     a. i'd have to do variable-width bignums with malloc, and there's a 
        lot to go wrong there; or 
     b. waste a lot of space, because it's recursive and each level (except
        the base case) allocates about 10kb (when I tried implementing it);
  and furthermore each level recurses 3 times (high, low, and high+low 
  halves of the operands). 
                                                    (see `redcunroll.stats`)
 - each call to mm_exp() now takes about 201ms. we make 380 calls, which
   makes sense:
   ```  
     $ wc corpus
     255 1547 9488 corpus
   ```
   (`corpus` is the Universal Declaration of Human Rights in ASCII text.)
   the symbol size (in the cryptographic sense) is (2048/8) = 256 bytes:
   ```
     9488 / 256 = 37.0625 = 38 symbols
   ```
   and the test (in `test_gmon.sh`) is running 10 times with 10 different
   random 2048-bit RSA keys. So we're calling mm_exp() exactly once for each
   symbol.
 - the total execution time is about 80 seconds, and (380 * 0.2) = 76. so
   95% of the time is from mm_exp(), and I can't optimise it or its
   components any further (mm_redc() or mult(), for reasons above). so
   it's on to assembly!


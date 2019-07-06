The goal of the random number generator compnent is to provide you with a ready-made random number generator for you to use in your microcontroller projects. The generator algorithm is based on the linear feedback shift register approach ([LFSR](https://en.wikipedia.org/wiki/Linear-feedback_shift_register)), specifically the one presented in [application note 4400](https://www.maximintegrated.com/en/app-notes/index.mvp/id/4400) from Maxim
Integrated.

## Usage

To generate a new random number, call the function `RandomNumberGet()` and it
will return a new 32-bit random number. Example:

```c
uint32_t number;

/* Generate a new random number. */
number = RandomNumberGet();
```

## Integration

Adding MicroRNG to your software project is a simple three step process:

1. Copy the files `randomnumber.c` and `randomnumber.h` and add them to your
   project, such that the `.c` is compiled and linked during a build and that
   the `.h` file is in a directory where the compiler looks for header files.
2. Add the following hook-function to one of your project's source files, for
   example `main.c`:
   ```c
   uint32_t RandomNumberSeedHook(void)
   {
     return 123456789ul;
   }
   ```
3. Add the following line to each source-file where you intend to add code for
   generating a random number:
   ```c
   #include "randomnumber.h"
   ```

## Generator seeding

The hook-function `RandomNumberSeedHook()` is automatically called by the random
number generator the first time your program requests a new random number. This
hook-function seeds the generator. The default implementation presented above
returns a constant seed value. Each time you call function `RandomNumberGet()`
you do get a different number, but the numbers will always be the same each
time your program is restarted:

| Reset #1     | Reset #2     | Reset #3     |
| ------------:| ------------:| ------------:|
| 3196210335   | 3196210335   | 3196210335   |
| 2375684060   | 2375684060   | 2375684060   |
| 3691277220   | 3691277220   | 3691277220   |
| 3596136368   | 3596136368   | 3596136368   |

For some application, this behavior is acceptable. However, most applications
require different random numbers to be generated each time the software program
is started. This is achieved by making sure hook-function `RandomNumberSeedHook()`
returns a different seed value, each time the software program is newly started.
Here are some possible approaches to realize such a changing seed value:

1. Keep one of the analog inputs on your microcontroller unconnected. Such a
   floating analog input picks up noise from the surrounding environment and
   readings will therefore always vary slightly. Configure your ADC peripheral
   to perform an analog to digital conversion of this analog input and use its
   result value to seed the generator.
2. If your microcontroller has EEPROM or a non-volatile register, you can use it
   to store a value that you use to seed the generator. Then simply increment
   its value by one upon software program startup. This way you have a different
   seed value every time your software program is started.
   * Keep in mind though that these data storage options have a limited amount of
     write cycles. A better option might be to use external FRAM.
3. If your microcontroller has access to an external file system such as an
   SD-card, you could store a value, which you use to seed the generator, in a
   file. Then simply increment its value by one upon software program startup.
   This way you have a different seed value every time your software program is started.

## Demo

A demo program is included for the ST [Nucleo-F091RC](https://www.st.com/en/evaluation-tools/nucleo-f091rc.html)
board. You can find it in the directory `./demos/ARMCM0_STM32F0_Nucleo_F091RC_GCC/`.
Approach #1 (the floating analog input) is implemented to seed the random number
generator. The demo program generates 8 random numbers and displays them on a
serial terminal. Here are some example results:

| Reset #1     | Reset #2     | Reset #3     |
| ------------:| ------------:| ------------:|
| 1193105914   |   99986390   | 2131584566   |
| 3607630837   | 1266123007   |    2093232   |
| 2184628642   | 3493368727   | 1164981103   |
|  793839531   |  296055939   | 3318979929   |
| 3582873577   | 1523539726   | 1508456861   |
| 1593850871   |  778353671   | 3618706881   |
|   16232337   | 1629400339   | 2245936811   |
| 2814901691   | 2083475926   | 1897443686   |

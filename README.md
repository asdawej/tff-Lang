# tff Language

```Python
__author__ = 'asdawej'
```

- [tff Language](#tff-language)
  - [Basic Concepts](#basic-concepts)
  - [Dependent Files](#dependent-files)
  - [Examples](#examples)

## Basic Concepts

**Balanced Ternary System (BTS):**
We use 3 symbols - `T`, `N`, and `F`  - to represent a number. Among the three, `T` is `1`, `N` is `0`, and `F` is `-1`.

An example: `TFF` is `1 * 4 + (-1) * 2 + (-1) * 1 = 1`.

**Script Sentence (SS):**
An expression in the form of `[func][args]`. The `[func]` is a number in the FST and `>=2`. The `[args]` should be written in the form according to the FST.

`[[SS]...]` is called a _script_ and can be linked to a location of VU.

_The C Implementation Details:_ The script is actually a `FILE*` that points to the script. And its VU structure is

```C
{
    "N",   // "N" as default; or the value in the config.tffc
    "N"    // "N" as default; or the BTS file position of script
};
```

**Value Unit (VU):**
An expression in the form of `0[BTS]10[BTS]1`. Different from other languages, in tff Language all the values should be stored with two BTS values. This is because in tff Language a VU can be used for a value or for a location.

In the usage of a value, the former one BTS value is called _type value_ and the latter one called _real value_.

In the usage of a location, the former one is called _memory area_ and the latter one called _memory location_.

_The C Implementation Details:_ The VU is defined as a structure

```C
typedef struct {
    char* f_val;
    char* l_val;
} VU;
```

**Function Symbols Table (FST):**
We use numbers to represent functions:

| `[func]` | `[args]` | Descriptions |
| :-: | :- | :- |
| `0` | No arguments | The left border of expression |
| `1` | No arguments | The right border of expression |
| `2` | `0[VU]1` | Return the VU in the memory of `[VU]` |
| `3` | `00[VU1]10[VU2]11` | The memory of the location of `[VU1]` will be replaced by `[VU2]` |
| `4` | `0[VU]1` | Run the script that linked with the location of `[VU]` |
| `5` | `00[VU]10[[SS]...]11` | Link the script `[[SS]...]` to the location of `[VU]` so it can be run by `4` |
| `6` | `00[VU]10[[SS1]...]10[[SS2]...]11` | Run `[[SS1]...]` if the real value of `[VU]` is `T`, else run `[[SS2]...]` if the real value `F`, no running if `N` |

## Dependent Files

Our interpreter works with two files that are related to your codes - _config.tffc_ and _main.tffl_. To run a tff script, you should input the code below to cmd:

```PowerShell
tff -r [main.tffl(*.tffl) path] [config.tffc(*.tffc) path]
```

or

```PowerShell
tff -r [main.tffl(*.tffl) path] default
```

**_config.tffc_:**
The text file that stores the environment variables, default values, and initial values.

If you just want to write a simple tff script, than you can just use our default _config.tffc_.

But if you want to have more than one tff scripts to run in the same time or you want to import C or Python extensions <font color=grey>(future, not now yet)</font>, you should do something strictly with _config.tffc_.

**_main.tffl_:**
The text file of your code.

## Examples

An exeample of swap the memory of the locations of `0FF10T1` and `0FF10TN1`:

```PowerShell
50# [out: None. in: (ff, t)location 1, (ff, tn)location 2. memory: (fn, f)]
    0 0 FN 1 0 T 1 1
    0
        30# Intermediate variable
            0 0 FN 1 0 F 1 1
            0
                20
                    20 0 FF 1 0 T 1 1
                1
            1
        1
        30# Give value to location 1
            0
                20 0 FF 1 0 T 1 1
            1
            0
                20
                    20 0 FF 1 0 TN 1 1
                1
            1
        1
        30# Give value to location 2
            0
                20 0 FF 1 0 TN 1 1
            1
            0
                20
                    20 0 FN 1 0 F 1 1
                1
            1
        1
        30# Clear intermediate variable
            0 0 FN 1 0 F 1 1
            0 0 N 1 0 N 1 1
        1
    1
1
```
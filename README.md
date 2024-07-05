# Zurox Programming Language Suite

## Presence of both CMake and Meson

As you may have noticed, the repository contains both CMake and Meson build systems.
There are particularly two reasons -
- Zurox is still in early stages so it's hard to tell which one is more benificial/reasonable to keep.
- I keep encountering issues in either thing which I try to solve in the other. This sort of helps me find out silly mistakes earlier.

You may also consider this two -
- Future collaborators / maintainers (if any, hopefully) can use their prefered build systems.
- To potentially figure issues wth cross-compiling

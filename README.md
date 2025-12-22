
# infra-lite

A lightweight, embeddable C/C++ infrastructure library.

`infra-lite` is a collection of small, reusable infrastructure components.

The focus is not on features, but on clean abstractions, zero global state,
and long-term maintainability.

---

## Philosophy

- Infrastructure over features
- Simple over complete
- Explicit over magic
- Composable over monolithic

---

## Key Characteristics

- Header-first design
- Easy to embed into existing projects
- Suitable for long-lived codebases

---

## Build & Test

```bash
mkdir build && cd build
cmake ..
make
ctest
```

## License

MIT License.


# infra-lite

A lightweight, embeddable C/C++ infrastructure library.

`infra-lite` is a collection of small, reusable infrastructure components
designed for tools, embedded systems, EDA flows, and internal utilities.

The focus is not on features, but on clean abstractions, zero global state,
and long-term maintainability.

## Philosophy

- Infrastructure over features
- Simple over complete
- Explicit over magic
- Composable over monolithic

## Key Characteristics

- Header-first design
- No global state
- No hidden cursors
- No mandatory tokenizer
- No heap allocation by default
- Easy to embed into existing projects

## Modules

### Parsing Framework

A minimal recursive-descent parsing framework inspired by parser combinators.

## Repository Structure

See directory layout for include/, src/, examples/, tests/, docs/.

## License

MIT License.

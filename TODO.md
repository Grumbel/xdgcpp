# TODO

## Audit against XDG Base Directory Specification (v0.8)

Spec: https://specifications.freedesktop.org/basedir-spec/latest/

### Shortcomings found and fixed

1. **Relative paths were thrown as errors instead of ignored** (fixed)
   - Spec §2: "All paths set in these environment variables must be absolute.
     If an implementation encounters a relative path in any of these variables
     it should consider the path invalid and ignore it."
   - Previous code raised `std::runtime_error` via `throw_if_not_absolute`.
   - Now relative (and empty) paths are silently ignored.

2. **Multi-value vars after filtering** (fixed)
   - For `XDG_DATA_DIRS` / `XDG_CONFIG_DIRS`: keep only absolute entries.
   - If the resulting list is empty (variable was empty, unset, or contained
     only relative/empty components) fall back to the specification defaults.
   - Matches practical behaviour of other solid implementations.

3. **Empty path components** (fixed)
   - Leading/trailing/double colons produce empty strings → treated as
     relative → ignored.

4. **XDG_RUNTIME_DIR** (unchanged behaviour, documented)
   - Spec recommends a secure fallback + warning when unset.
   - Providing a correctly-permissioned, login-lifetime, local-fs directory
     is outside the scope of a pure path-query library. The library continues
     to throw when the variable is unset/empty/relative so callers can decide
     how to recover.

5. **HOME**
   - Still required to be absolute when defaults are materialised. A relative
     or missing HOME is a broken environment and produces an exception.

### Other notes (no change required)

- Defaults match the current spec (including `XDG_STATE_HOME`).
- Library only returns paths; it does not create directories or set modes.
- `$HOME/.local/bin` is an informational recommendation only.
- Path separator is hard-coded to `:` (Unix). Acceptable for this project.

### Remaining / future

- [ ] Consider returning `std::optional` for `runtime().dir()` in a future
      major version so callers can handle the missing case without exceptions.
- [ ] Windows path-separator support if the library is ever ported.

### Commits in this work

- Ignore relative paths per XDG spec (and update tests)

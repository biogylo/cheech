// Copyright 2025 Juan Navarro
const char *__asan_default_options() {
  return "detect_leaks=0:abort_on_error=1:halt_on_error=1";
}

const char *__ubsan_default_options() {
  return "abort_on_error=1:halt_on_error=1";
}

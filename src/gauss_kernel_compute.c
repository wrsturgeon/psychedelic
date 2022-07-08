#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C++ sqrt & erf aren't constexpr, so we
// compute them once and write to an .hpp

#define FILEPATH "src/gauss_kernel.hpp"

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <sigma>", *argv);
  }
  char const *const sigma_str = argv[1];
  size_t arglen = strlen(sigma_str) + 1;

  // Check we didn't already compute it
  FILE *hpp = fopen(FILEPATH, "rb");
  if (hpp) {
    char last_sigma[arglen];
    fseek(hpp, 11, SEEK_CUR);
    fread(last_sigma, sizeof(char), arglen, hpp);
    fclose(hpp);
    if ((last_sigma[arglen - 1] == '\n') && !strncmp(sigma_str, last_sigma, arglen - 1)) {
      return 0; // Already computed for the exact same sigma
    }
    char *nl = memchr(last_sigma, '\n', arglen);
    if (!nl) { nl = last_sigma + arglen - 1; }
    *nl = '\0';
    printf("%s =/= %s...; recalculating...\n", sigma_str, last_sigma);
  }
  double sigma = strtod(sigma_str, 0);

  // Recalculate
  double center = erf(0.5 / sigma);
  double periph = 0.5 * (erf(1.5 / sigma) - center);
  double summed = center + (2 * periph);
  center /= summed;
  periph /= summed;
  hpp = fopen(FILEPATH, "w");
  if (!hpp) {
    fprintf(stderr, "Couldn't open " FILEPATH);
    exit(1);
  }
  fprintf(hpp,
        "// sigma = %s\n"
        "#ifndef GAUSS_KERNEL_HPP_\n"
        "#define GAUSS_KERNEL_HPP_\n"
        "namespace gauss {\n"
        "  static constexpr double kCenter = %lf;\n"
        "  static constexpr double kPeriph = %lf;\n"
        "} // namespace gauss\n"
        "#endif // GAUSS_KERNEL_HPP_\n",
        sigma_str, center, periph);
  fclose(hpp);
  return 0;
}

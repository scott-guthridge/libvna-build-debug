class Libvna < Formula
  desc "Vector network analyzer (VNA) calibration, conversion and touchstone load/save"
  homepage "https://github.com/scott-guthridge/libvna"
  url "https://github.com/scott-guthridge/libvna/releases/download/v0.3.9/libvna-0.3.9.tar.gz"
  sha256 "b6c8cec3bb77b4d2bc1bf4ef63cdb383d1bbd7eaa260334aa77c7c4b658b86a7"
  license "GPL-3.0-or-later"
  head "https://github.com/scott-guthridge/libvna.git", branch: "master"
  depends_on "libyaml"

  def install
    system "./configure", *std_configure_args, "--disable-silent-rules"
    system "make", "-j10", "install"
  end

  test do
    (testpath/"test.c").write <<~EOS
      #include <assert.h>
      #include <complex.h>
      #include <math.h>
      #include <stdio.h>
      #include <stdlib.h>
      #include <vnaconv.h>

      static const double complex z0[] = { 75.0, 50.0 };
      static const double complex z[2][2] = {
        { 129.9038,  86.60254 },
        {  86.60254, 86.60254 },
      };

      int main(int argc, char **argv)
      {
        double complex s[2][2];

        vnaconv_ztos(z, s, z0);
        for (int i = 0; i < 2; ++i) {
          for (int j = 0; j < 2; ++j) {
            assert(fabs(cimag(s[i][j]) - 0.0000) < 0.001);
            if (i == j) {
              assert(fabs(creal(s[i][j]) - 0.0000) < 0.001);
            } else {
              assert(fabs(creal(s[i][j]) - 0.5176) < 0.001);
            }
          }
        }
        exit(0);
      }
    EOS
    system ENV.cc, "test.c", "-L#{lib}", "-lvna", "-o", "test"
    system "./test"
  end
end

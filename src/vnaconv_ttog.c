/*
 * Vector Network Analyzer Library
 * Copyright © 2020-2023 D Scott Guthridge <scott_guthridge@rompromity.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "archdep.h"

#include <complex.h>
#include <math.h>
#include "vnaconv_internal.h"


/*
 * vnaconv_ttog: convert t-parameters to g-parameters
 */
void vnaconv_ttog(const double complex (*t)[2], double complex (*g)[2],
	       const double complex *z0)
{
    const double complex t11 = t[0][0];
    const double complex t12 = t[0][1];
    const double complex t21 = t[1][0];
    const double complex t22 = t[1][1];
    const double complex z1 = z0[0];
    const double complex z2 = z0[1];
    const double complex z1c = conj(z1);
    const double complex z2c = conj(z2);
    const double k1i = sqrt(fabs(creal(z1)));
    const double k2i = sqrt(fabs(creal(z2)));
    const double z1r = creal(z1);
    const double z2r = creal(z2);
    const double complex d = (t11 + t12) * z1 + (t21 + t22) * z1c;

#define G11	g[0][0]
#define G12	g[0][1]
#define G21	g[1][0]
#define G22	g[1][1]

    G11 =         - (t11 + t12 - t21 - t22)                             / d;
    G12 = k1i / k2i * ((t12*t21 - t11*t22) * 2.0 * z2r)                 / d;
    G21 = k2i / k1i * (2.0 * z1r)                                       / d;
    G22 =         - (t11*z1*z2 - t12*z1*z2c + t21*z1c*z2 - t22*z1c*z2c) / d;
}
#undef G22
#undef G21
#undef G12
#undef G11

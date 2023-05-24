Maxwellian Rate Coefficients
============================

The rate coefficient (:math:`k`) for an electron-impact reaction can be
expressed as,

.. math:: k = \sqrt{\frac{2 e}{m_e}} \int_{0}^{\infty} E\;\sigma(E)f(E)\,dE,

where :math:`\sigma (E)` is the cross-section for the reaction which is
a function of :math:`E` the electron energy, :math:`f(E)` is the
electron energy distribution function (EEDF) in :math:`eV^{-3/2}`,
:math:`e` and :math:`m_e` are the charge and mass of the electron,
respectively. The EEDF in :math:`eV^{-3/2}` is also known as electron
energy probability function.

Generalised form of EEDF
------------------------

The generalized form of the EEDF (in :math:`eV^{-3/2})` can be expressed
as,

.. math:: f(E) = c_1 T_{eff}^{-\frac{3}{2}} \; e^{-c_2 (E/T_{eff})^x},

where

.. math::

   c_1 = x \left(\dfrac{2}{3}\right)^{\dfrac{3}{2}} \dfrac{[\Gamma(5/2x)]^{(3/2)}}{[\Gamma(3/2x)]^{(5/2)}}; \\
       c_2 = x \left(\dfrac{2}{3}\right)^{\dfrac{3}{2}} \left[\dfrac{\Gamma(5/2x)} {\Gamma(3/2x)}\right]^x.

The :math:`x` and :math:`T_{eff}` are the two adjustable parameters;
:math:`T_{eff}` is the effective (or mean) electron temperature, and
:math:`x`\  [1]_ can be varied between 1 and 2. :math:`\Gamma(\xi)` is
the usual gamma function.

Maxwellian EEDF
----------------

When :math:`x = 1`, the generalised EEDF form presented in
equation `[Eq EEDF] <#Eq EEDF>`__ reduces to Maxwellian distribution.

.. [1]
   :math:`x=1` reduces the generalised form to Maxwellian, and
   :math:`x=2` to Druyvesteyn.

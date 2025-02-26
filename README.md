# FEBioHeat
The FEBioHeat plugin implements a solver for the heat transfer equation that can be used with [febio](https://febio.org/).
It currently supports steady-state and transient heat transfer analyses and assumes Fourier's law for calculating the heat flux.

Supported boundary conditions are: 
* Zero and prescribed temperature
* Prescribed heat flux
* Convective heat flux
* Gap heat flux (enforces continuous heat flux across an interface)

Supported body loads are:
* Constant heat source
* Bio-heat

For transient problems, the initial temperature can be prescribed. 


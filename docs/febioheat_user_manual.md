## Introduction

This user manual discusses the feature of the FEBioHeat plugin in the context of the FEBio input file. 

This manual assumes the 4.0 FEBio format specification.

## Heat Module

The module type must be set to `heat` for solving a heat-transfer problem with this plugin. 

```xml
<Module type="heat"/>
```

## Heat transfer constitutive model

Currently, only one constitutive model is supported for heat-transfer analysis, namely the `isotropic Fourier`, which defines the heat flux $\mathbf{q}$ as follows,

\[\mathbf{q}=-k\,\nabla T\]

Here, $T$ is the temperature and $k$ the _thermal conductivity_.

This material has the following parameters as shown in the following table. 

| parameter | description               |units (SI)|
|-----------|---------------------------|----------|
|`density`  | The material density      | kg/m^3   |
|`k`        | The thermal conductivity	| W/m.K    |
|`c`        | The specific heat         | J/kg.K   |

The units are just given as an example of a consistent set of units for these variables. Users can use their preferred unit system instead. 

Note that density and specific heat are only used for transient analysis.

An example of material definition follows below.

```xml
<material id="1" name="myMaterial1" type="isotropic Fourier">
	<density>1.0</density>
	<k>0.4</k>
	<c>1.0</c>
</material>
```

## Heat Transfer Boundary Conditions
### Fixed and prescribed temperature

To prescribe a temperature on a boundary, the “prescribed temperature” boundary condition can be used. A load controller can be assigned to the value parameter to make the prescribed temperature a function of time. For example,

```xml
<bc node_set="PrescribedBC1" type="prescribed temperature">
	<value lc="1">1.0</value>
	<relative>0</relative>
</bc>
```

For prescribing a zero temperature on a boundary, a slightly more efficient boundary condition can be used instead, namely the `zero temperature` boundary condition.

```xml
<bc name="BC01" node_set="FixedTemperature1" type="zero temperature"/>
```

This is more efficient since the degree of freedom for nodes assigned to a `zero temperature` boundary condition, is effectively ignored, resulting in smaller linear system of equations that needs to be solved.

### Heat flux
The heat flux surface load prescribes the heat flux on a surface of the mesh. It is defined as a surface_load with the type attribute set to `heatflux`. It takes a single parameter, namely `flux`, which specifies the value of the heat flux.

```xml
<surface_load type="heatflux" surface="Surface01">
	<flux lc="1">2.5</flux>
</surface_load>
```

### Convective heat flux
A convective heat flux applies a flux boundary condition where the flux is proportional to the difference between the surface temperature $T$ and the ambient temperature $T_a$.

\[{{q}_{c}}={{h}_{c}}\left( T-{{T}_{a}} \right)\]

This boundary load is defined as a `surface_load` with the type attribute set to `convective_heatflux` and requires two parameters.

| parameter | description                  | units (SI)|
|-----------|------------------------------|-----------|
| `hc`      | The proportionality constant | W/m2.K    |
| `Ta`      | The ambient temperature      | K         |

An example is given below. 

```xml
<surface_load type="convective_heatflux" surface="Surface01">
	<hc>60.0</hc>
	<Ta lc="1">25</Ta>
</surface_load>
```

## Heat source

A heat source can be added by defining a body_load with the type set to `heat_source`. Only one parameter is required, `Q`, to define the heat source value.

| parameter | description                    | units (SI)|
|-----------|--------------------------------|-----------|
| `Q`       | The constant heat source value | W/m^3      |

```xml
<body_load type="heat_source">
	<Q lc="1">13.5</Q>
</body_load>
```

## Output variables

The heat transfer plugin adds several output variables to both the plot file and the log file.

### Plot file variables
The following table shows a list of all new variables defined by the plugin.

| variable    | description                           |
|-------------|---------------------------------------|
|`temperature`|The nodal temperature at the mesh nodes|
|`heat flux`  |The average heat flux over each element|

### Log file variables

The following table shows a list a log file variables.

|variable | description       |class    |
|---------|-------------------|---------|
| `T`     | nodal temperature |node_data|


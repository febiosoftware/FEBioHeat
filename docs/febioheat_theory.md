## Introduction
Theory Manual
The heat transfer equation is defined as follows.

\[
\begin{equation}
\label{eq:heat-transfer}
\rho C\frac{\partial u}{\partial t}+\nabla \cdot \mathbf{q}=Q
\end{equation}
\]

Here, $u$ is the _temperature_, $\rho$ the _density_, $C$ the _specific heat_, $\mathbf{q}$ the _heat flux_, and $Q$ the _heat source_. 

The following boundary conditions are assumed.

\[\begin{align}
  & u={{u}_{0}}\quad\text{on}\,{{\Gamma }_{D}} \\ 
 & \mathbf{q}\cdot \mathbf{n}={{q}_{0}}\quad\text{on }\,{{\Gamma }_{N}} \\ 
\end{align}\]


Furthermore, it is assumed that the heat flux is defined via Fourier’s law,

\[
\begin{equation}
\label{eq:heat-flux}
\mathbf{q}=-k\,\nabla u
\end{equation}
\]

where $k$ is the thermal conductivity. 

## Weak Formulation
To construct the weak form, first multiply the left- and right-hand side with an arbitrary function $w$ (that satisfies the homogenous boundary conditions, i.e. $w=0$ on ${{\Gamma }_{D}}$).

\[w\left( \rho C\frac{\partial u}{\partial t}+\nabla \cdot \mathbf{q} \right)=wQ\]

Then, integrate over the problem domain.

\[\int\limits_{\Omega }{w\left( \rho C\frac{\partial u}{\partial t}+\nabla \cdot \mathbf{q} \right)d\Omega }=\int\limits_{\Omega }{wQd\Omega }\]

Using integration by parts and the boundary conditions, we can rewrite the second term on the left-hand side.

\[\int\limits_{\Omega }{w\rho C\frac{\partial u}{\partial t}d\Omega }+\int\limits_{{{\Gamma }_{N}}}{w{{q}_{0}}d\Gamma }-\int\limits_{\Omega }{\nabla w\cdot \mathbf{q}d\Omega }=\int\limits_{\Omega }{wQd\Omega }\]

Using equation (\ref{eq:heat-flux}) results in,

\[\int\limits_{\Omega }{w\rho C\frac{\partial u}{\partial t}d\Omega }+\int\limits_{\Omega }{k\nabla w\cdot \nabla ud\Omega }=\int\limits_{\Omega }{wQd\Omega }-\int\limits_{{{\Gamma }_{N}}}{w{{q}_{0}}d\Gamma }\]

For steady-state analysis, we simply omit the first term.

\[\int\limits_{\Omega }{k\nabla w\cdot \nabla ud\Omega }=\int\limits_{\Omega }{wQd\Omega }-\int\limits_{{{\Gamma }_{N}}}{w{{q}_{0}}d\Gamma }\quad\text{(steady state analysis)}\]

## Discretization
The domain is discretized using iso-parametric elements.

\[\begin{align}
  & \mathbf{r}=\sum\limits_{a}{{{N}_{a}}{{\mathbf{r}}_{a}}} \\ 
 & u=\sum\limits_{a}{{{N}_{a}}{{u}_{a}}} \\ 
 & w=\sum\limits_{a}{{{N}_{a}}{{w}_{a}}} \\ 
\end{align}\]

Considering first the simpler case of steady state, the discretized equation becomes the following.

\[\sum\limits_{a,b}{{{w}_{a}}\left( \int\limits_{\Omega }{k\nabla {{N}_{a}}\cdot \nabla {{N}_{b}}d\Omega } \right){{u}_{b}}}=\sum\limits_{a}{{{w}_{a}}\left( \int\limits_{\Omega }{{{N}_{a}}Qd\Omega } \right)}-\sum\limits_{a}{{{w}_{a}}\left( \int\limits_{{{\Gamma }_{N}}}{N_{a}^{'}{{q}_{0}}d\Gamma } \right)}\]

Note that in the second term on the right-hand side, the shape functions are restricted to the boundary surface.

This can be written more concisely.

\[\mathbf{Ku}=\mathbf{F}\]

Here, $\mathbf{K}$ is the stiffness matrix and is calculated from,

\[{{K}_{ab}}=\int\limits_{\Omega }{k\nabla {{N}_{a}}\cdot \nabla {{N}_{b}}d\Omega }\]

Note that the stiffness matrix is symmetric.

The vector $\mathbf{u}$ collects all the unknown temperature values, and $\mathbf{F}$ collects all the known force loads,

\[{{F}_{a}}=\int\limits_{\Omega }{{{N}_{a}}Qd\Omega }-\int\limits_{{{\Gamma }_{N}}}{N_{a}^{'}{{q}_{0}}d\Gamma }\]

Note again the slight abuse of notation, since the second term is evaluated over a surface, using different shape functions than the first term.

Returning now to the transient case, we still need to discretize the term,

\[\int\limits_{\Omega }{w\rho C\dot{u}d\Omega }\]

Here, we use $\dot{u}$ to denote the time derivative of the temperature. Introducing shape functions, similar as before,

\[\dot{u}=\sum\limits_{a}{{{N}_{a}}{{{\dot{u}}}_{a}}}\]

The discretized integral then becomes,

\[\sum\limits_{a,b}{{{w}_{a}}\left( \int\limits_{\Omega }{\rho C{{N}_{a}}{{N}_{b}}d\Omega } \right)\,{{{\dot{u}}}_{b}}}=\sum\limits_{a,b}{{{w}_{a}}{{M}_{ab}}{{{\dot{u}}}_{b}}}\]

where,

\[{{M}_{ab}}=\int\limits_{\Omega }{\rho C{{N}_{a}}{{N}_{b}}d\Omega }\,\]

is the _capacitance matrix_.

Bringing all the terms together results in the following equation,

\[\mathbf{M\dot{u}}+\mathbf{Ku}=\mathbf{F}\]

This is the _semi-discrete equation_. Solving it will be discussed below.


## Time Discretization

We now turn our attention to solving the semi-discrete equation, which is repeated here.

\[\mathbf{M\dot{u}}+\mathbf{Ku}=\mathbf{F}\]

We’ll solve this equation using the implicit-Euler method (also known as backward Euler method). This method is unconditionally stable and first-order accurate.

First, we replace $\dot{\mathbf{u}}$ with a finite difference approximation.

\[\mathbf{\dot{u}}=\frac{1}{\Delta t}\left( {{\mathbf{u}}^{n+1}}-{{\mathbf{u}}^{n}} \right)\]

Then, we solve the semi-discrete equation at time point n+1.

\[\left( \frac{1}{\Delta t}\mathbf{M}+\mathbf{K} \right){{\mathbf{u}}^{n+1}}=\mathbf{F}+\frac{1}{\Delta t}\mathbf{M}{{\mathbf{u}}^{n}}\]

Once again, we recover a linear system of equations that can be solved.
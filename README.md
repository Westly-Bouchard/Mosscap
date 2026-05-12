# Mosscap

Mosscap is a simulation platform for Arduino based robots. The goal of this project is to provide
a tool that can be used for robotics education from high school through college. Essentially,
Mosscap addresses the problem statement:

> Robotics hardware is often prohibitively expensive. Even simple wheeled mobile robot kits that
> can be purchased on Amazon can exceed $100 in cost, which puts them out of reach for many students
> or hobbyists.
> 
> Additionally, many software based educational tools and simulators provide their own barriers to
> entry in the form of complicated installation and usage patterns, along with large amounts of
> assumed prerequisite knowledge for their use.
> 
> Naturally, the question arises: How might one create an educational robotics platform that is
> simple to install and use, and that can leverage users' existing knowledge to provide educational
> opportunities without such barriers to entry?

Mosscap addresses this problem by packaging a lightweight physics simulator and Arduino interface
into a VS Code extension to allow for simple installation. It leverages WebAssembly through
emscripten to compile a user's Arduino sketch and open the simulation interface in browser.

This circumvents the hassle of c++ build systems and dependency management and simplifies the usage
requirements to two simple items:
1. Do you have a computer?
2. Can that computer run VS Code?

Currently, Mosscap cannot run in the web version of VS Code. Although this is certainly a goal for
the future.

# Usage
More info to come soon!
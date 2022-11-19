
# Simple Command Handler

This is Simple Command Handler library. It is frameworkless library for parsing C-string and executing found commands. 
It written in C++ and requires [sSTD](https://github.com/silvio3105/sSTD) library.

Library can parse and execute multiple commands with next format: `cmd1:param1,...,paramN;...;cmdN:param1,...,paramN;` where:

- `;` is delimiter between commands.
- `:` is delimiter between command and arguments.
- `,` is delimiter between arguments.

Command to arguments and argument to argument delimiters can be same, only command to command delimiter have to be different. Input C-string have to be NULL terminated. List of commands does not need to end with command to command delimiter.

To create object for handling commands: `CMD<N> cmd = CMD<N>(...);` where `N` is maximum number of arguments command hanlder can handle.
Example: `CMD<4> cmd = CMD<4>(list, 2, fallback, ';', ':', ',');` where:

- `4` in angled bracked means handler `cmd` can handle maximum 4 arguments.
- `list` is pointer to list of commands.
- `2` is length of `list`.
- `fallback` is pointer to fallback function.
- `;` is command to command delimiter.
- `:` is command to arguments delimiter.
- `,` is argument to argument delimiter.

Check library documentation and example codes for more information.
Library documentation is available at `.docs/html/index.html`.
Example applications are available at `examples` folder. All examples are made with STM HAL framework.

# License

Copyright (c) 2022, silvio3105 (www.github.com/silvio3105)

Access and use of this Project and its contents are granted free of charge to any Person.
The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
Commercial use of this Project and its contents is prohibited.
Modifying this License and/or sublicensing is prohibited.

THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

This License shall be included in all functional textual files.

---

Copyright (c) 2022, silvio3105
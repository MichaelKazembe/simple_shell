
# Simple Shell Project

This project implements a simple shell in C, which is a command-line interface that allows users to interact with the operating system by running commands. The simple shell is a basic version of the UNIX shell, which is the default command-line interface for most Linux and UNIX-based operating systems.

## Features

The simple shell supports the following features:

- Executing external commands
- Redirecting input and output
- Pipelining commands
- Running commands in the background
- Environment variable expansion
- Signal handling
- Builtin commands such as `cd`, `echo`, `exit`, and more.

## Usage

To use the simple shell, simply compile the code and run the resulting executable file. Once the shell is running, you can enter commands at the prompt, just like you would in a regular terminal.

## Installation

To install the simple shell, follow these steps:

1. Clone the repository using `git clone https://github.com/your-username/simple-shell.git`
2. Navigate to the project directory using `cd simple-shell`
3. Compile the code using `gcc -Wall -Werror -Wextra -pedantic *.c -o shell`
4. Run the shell using `./shell`

## Builtin commands

The simple shell includes a number of builtin commands, which are executed directly by the shell itself rather than by a separate process. These commands include:

- `cd`: Change the current working directory.
- `echo`: Display a message on the screen.
- `exit`: Terminate the shell.
- `help`: Display help information for builtin commands.
- `history`: Display the history of commands entered in the shell.
- `setenv`: Set the value of an environment variable.
- `unsetenv`: Remove an environment variable.
- `alias`: Create an alias for a command.

## Limitations

The simple shell has a number of limitations, including:

- Lack of support for advanced features like scripting and job control
- Limited error handling
- No support for command-line editing or tab completion

## Credits

This project was created by [Samuel Chigozie] and [Michael Kazembe] as part of the curriculum for [ALX SIMPLE_SHELL TASK]. It was inspired by the original UNIX shell, which was developed by [KEN THOMPSON] in [1971].

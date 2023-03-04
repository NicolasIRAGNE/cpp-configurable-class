# cpp-configurable-class

Experimenting with configurable classes in C++. The goal was to allow self-registration of classes and their parameters, and to allow the user to configure the classes from a configuration file or from the command line.

This kinda works but is far from being as flexible as I would like it to be. Just leaving it here for posterity.

Maybe I'll come back to it one day, but I think it need to be rethought from the ground up.

I wish there was a more standard way to do this in C++. Boost has some stuff but it's not really well documented and I don't think it's actively maintained.

## Basic principles

The idea is to provide an interface for classes to give a comprehensive description of their parameters, and to provide a way to configure them from a configuration file or from the command line, all from the class itself. This way, classes can be added easily to the system without having to modify the configuration system itself.

A basic test is included. It declares a simple "Printer" class with configurable text and padding. The registry is now populated with the Printer class, and the configuration system can be used to configure it, along with providing a "usage" message.

Example with the following configuration file:

```ini
[printer]
text = hello world
padding = 10
```

```sh
$ ./configurable-test
Registered options:
Printer options:
  --printer.text arg (=UNINITIALIZED) text to print
  --printer.padding arg (=0)          padding to add before printing


Loading config file "../test.cfg"...
PRINTER:           hello world
```

Parameters can be overridden from the command line:

```sh
$ ./configurable_test --printer.text="yeahhhhhh uhhhhh can i get uhhhhhhh     mcburger" --printer.padding=0
Registered options:
Printer options:
  --printer.text arg (=UNINITIALIZED) text to print
  --printer.padding arg (=0)          padding to add before printing


Loading config file "../test.cfg"...
PRINTER: yeahhhhhh uhhhhh can i get uhhhhhhh     mcburger
```

Anyways, not in a usable state, but it's a start.
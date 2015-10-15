# Rathaxes: Language approach for safe and multi-operating systems driver generation #

# What is a driver? #

A driver is a software which allows communication between hardware and operating system. Tasks like using a printer or a mouse could not be achieved without driver. Those softwares are very important and take the most part of code in an operating system:

  * An operating system is composed by 70% of drivers;
  * Some system like Windows provide generic drivers for common hardware;
  * Often those softwares are made by hardware vendors.

# Why create a driver generator? #

## Dual skills ##

Writing this kind of software is very tricky. Dual skills are necessary to write a driver. Good knowledge of hardware specifications and skills in system development are required.

## Driver are critical ##

Because of their complexity, driver are seven times more subject to errors than other software. Those software are linked to operating system, one critical error could disturb the entire system. This kind of error generally causes a system shutdown (Blue Screen or Kernel Panic).

## Redundancy ##

To finish, the code in those software is very redundant. One driver written for one operating system must be totally rewritten for another system, even if the algorithms stay the same.

## Conclusion ##

Rathaxes aims to resolve those problems by:

  * Providing a new safe description device language;
  * Checking the developers code to eliminate the majority of bugs;
  * Generating driver for the supported operating systems (currently, Windows 7, Linux and OpenBSD).

# Getting started #

Even though Rathaxes is still in an early conception stage you download it and
install it on your computer.
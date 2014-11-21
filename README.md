turbo-sense
===========

Sensor package code for Firefighting Drone Challenge: Fall 2014

Motivation and Goal
===================

The goal of the sensor package module is to provide obstacle detection. Ideally, obstacles around 360 degrees in three dimensions should be accurately detected and reported to the control module. There are at least two closely related tasks that have to be completed in this module:

    selection, characterization and modelling of the individual sensors (potentially only one, but likely multiple ones are required)
    fusion of the information from individual sensors in three dimensions

The fused information has to be then sent via some interface to the control module. The interface can be as simple as providing a hardware design with a software "driver" code, or as complex as defining a data format to be sent over some standardized bus.

The primary challenge in this module is designing a low cost, yet accurate sensor that is able to avoid as many common obstacles as possible.


Other information
=================

The HC-SR04 sensor is a low-cost ultrasonic sensor.  This code implements ten of these sensors and measures the distances they report.

All sensors are triggered at the same time and thus run concurrently.  This code was designed to run on the Raspberry Pi Model B+.

The distances reported over the FIFO correspond to sides of a rectagular prism representing free space around the sensor package.
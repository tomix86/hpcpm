# What is it?
A centralized power management system for a distributed environment consisting of NVIDIA GPUs and Intel Xeon Phi coprocessors. It was developed as a part of engineer's degree thesis, for which an abstract is given down below.

## Authors
* [Tomasz Gajger](https://github.com/tomix86)
* [Andrzej Podgórski](https://github.com/andi9310)
* [Bartosz Pollok](https://github.com/bartekmp)

## Abstract
The purpose of this study is to design and then implement a system which would allow to manage the energy consumption of a heterogeneous (consisting of many devices of various types) and distributed computation environment in a centralized and uniform manner. The main problem that had to be solved were the differences in design and APIs of power management libraries supplied by the manufacturers of computational devices and thus the need to interact with each of them in that library’s specific way. The system created as a part of this thesis provides a device independent and centralized way to manage the power consumption and furthermore, its architecture was designed with the goal to be as much extensible as possible. Based on the experiences from the implementation phase, comparison of following libraries: NVML, MPSS and NMPRK was made, in which similarities and differences as well as pros and cons of each of them were pointed out. The conclusion of this analysis is that from aforementioned libraries, NVML deserves the highest grade and the NMPRK has proven to be the worst one. Finally, several test scenarios in an environment consisting of Intel Xeon Phi accelerators and Nvidia Tesla GPUs were executed, every single of them completed successfully and thus the correct functioning of implemented system was confirmed.

**Keywords**: energy consumption, accelerators, heterogeneous computational environments, high performance computing, NVML, MPSS, NMPRK.


For a detailed description see the [engineer's degree thesis (in polish)](Podsystem-do-zarzadzania-zuzyciem-energii-dla-heterogenicznego-srodowiska-obliczen-wysokiej-wydajnosci.pdf).

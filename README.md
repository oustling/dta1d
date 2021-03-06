# dta1d
Program can be used in radiotherapy departments by physicists to perform certain QA procedures. Program compares two dose profiles.

One profile is taken from two-dimensional dose plane. That can be dose plane exported from Monaco TPS or from OmniPro I'mRT. OmniPro I'mRT plane must be exported as csv separated by comma (choose File->Export Data->Generic ASCII File). The desired profile can be selected from the plane.

The other dose profile can be measured using a water phantom and application IBA OmniPro Accept. It generates one-dimensional array of values. Unfortunately these files must be csv as well. 
You can also import 1D dose from csv file. Some example files are in py_scripts folder.
   
The program is simple, the input files must meet some criteria, ie, the spatial resolution must be 0.1cm in all files, additionally in the middle always must be 0cm (the profiles from -5cm to 10cm are not allowed so far). So remember to prepare the input files first. Sorry for the inconvenience.

It is written in c, compiled by gnu gcc under linux. It uses gtk3 library to draw widgets.

To compile it you must have some development tools installed, these tools are: autoconf, automake, pkg-config, gcc and make.
You will need as weel <b>gtk3</b> and <b>glib</b> in development mode installed (gtk+3.2.0-devel and glib-2.6.0-devel or something similar). After downloading the source (or after pulling the source in git), in main directory run: autoconf, automake, ./configure, make. This should produce binary file named dta1d.

The program probably still have some errors or bugs. The coding style is not perfect as well. If someone has any suggestions please write an email to 0.akowalski@gmail.com or maybe try to contribute.

Some screenshots are in wiki section.

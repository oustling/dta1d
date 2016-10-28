# dta1d
Program can be used in radiotherapy departments by physicists to perform certain QA procedures. Program compares two dose profiles.

One profile is taken from two-dimensional dose plane. That can be dose plane exported from Monaco TPS or from OmniPro I'mRT. The desired profile can be selected from the plane.

The other dose profile can be measured using a water phantom and application IBA OmniPro Accept. It generates one-dimensional array of values.

The program is simple, the input files must meet some criteria, ie, the spatial resolution must be 0.1cm in all files, additionally in the middle always must be 0cm (the profiles from -5cm to 10cm are not allowed so far). So remember to prepare the input files first. Sorry for the inconvenience.

It is written in c. Probably still have some errors or bugs. The coding style is not perfect as well. If someone has any suggestions please write an email to 0.akowalski@gmail.com or maybe try to contribute.

Some screenshots are in wiki section.

# headsetpresenter
Legacy project for controlling presentations etc using a bluetooth headset. The code is not cleaned up but in theory it should compile fine using Visual Studio 6.0 (or a newer version given that the solution files converts ok).

This is a very old project I did between 2001 and 2008 which I added to github in 2017 so the code would not get lost.

HeadsetPresenter started out as an idea to control PowerPoint presentations using the buttons on Bluetooth headsets.

The project proved quite challenging given the massive fragmentation of Bluetooth drivers. The solution finally supported the following stacks:
* Microsoft Windows Bluetooth stack introduced in Vista
* Bluesolei
* Widcomm
* Toshiba
* and a custom solution adding a USB sniffer filter on the Bluetooth driver intercepting the ACL commands.

In the last release voice control was also added. The application was very well received but I stopped updating it due to lack of time.

In the reposition is also the code for a custom software based License Management system based on ATL/COM and C++ that I developed for the HeadsetPresenter project. It was capable of managing encrypted license keys controlled by a web backend as well as performing custom CRC checks to avoid tampering. It was used in other projects as well and since it was COM based it was pretty much language independent. At least one project that I am aware of used LISP for licensing an Autocad plugin..

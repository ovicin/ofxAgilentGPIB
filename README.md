ofxAgilentGPIB
==============

first draft implementation

OF addon to communicate with lab instruments (power supplyes, osciloscopes, electronic loads etc) using the GPIB protocol.

There are 2 connection modes available:
- using the IEE-488 connector https://en.wikipedia.org/wiki/IEEE-488 -> in this case is using the IO library suite from agillent http://www.home.agilent.com/en/pc-2267246/instrument-control?nid=-33002.0&cc=ES&lc=eng
- using RS-232 -> in this case the Agilent library is not used and the communication is implemented just using OfSerial

Tested with a Chroma DC Electronic Load

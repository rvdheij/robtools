/* PDWFMTEV REXX                                                     */
/*                   Author: Rob van der Heij, 31 Jul 2017           */

signal on error
signal on novalue

'select input 1'
'peekto rec'
parse var rec '=' n1 ',' n2 .
tag = 'x'n1''n2

'addpipe (end \ name PDWFMTEV.REXX:11)',
   '\ *.input.0: ',
   '| p: pick 1+ ==' tag ,
   '| *.input.0:',
   '\ p: ',
   '| w: pdwfmtev ',
   '| i: faninany ',
   '| *.output:',
   '\ *.output: ',
   '| i:',
   '\ *.input.1:',
   '| drop',
   '| t: strtolabel /=/',
   '| *.input.1:',
   '\ t: ',
   '| w:'

'callpipe (end \ name PDWFMTEV.REXX:28)',
   '\ *.input.1:',
   '| locate w1',
   '| o: fanout ',
   '| t: take ',
   '| substr substr 2-* of w1',
   '| var struct ',
   '\ t: ',
   '| xlate w2',
   '| stem flds.',
   '\ o: ',
   '| structure add caller'

'callpipe (end \ name PDWFMTEV.REXX:44)',
   '\ stem flds. ',
   '| pdwmkfld',
   '| join * , ,',
   '| var pip'

'select input 0'

'callpipe (end \ name PDWFMTEV.REXX:53)',
   '\ *: ',
   '| o: fanout',
   '| pdwmkrun qual' struct '1 nowrite' pip,
   '| j: join * /, /',
   '| locate w1',
   '| spec /{ "evt": "'struct'",/ 1 1-* nw /}/ nw',
   '| *:',
   '\ o: ',
   '| j:'

error: return rc * ( rc <> 12 )

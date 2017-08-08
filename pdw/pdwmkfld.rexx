/* PDWMKFLD REXX                                                     */
/*                   Author: Rob van der Heij, 31 Jul 2017           */

'callpipe (end \ name PDWMKFLD.REXX:4)',
   '\ *: ',
   '| substr w1.2 ',
   '| o: fanout ',
   '| copy ',
   '| s: spec ,outstream 0 /, 1 w1 n ,/ 1 write, n',
          'select 1 1-* nw',
   '| *:',
   '\ o: ',
   '| pdwmkfmt ',
   '| s:'


error: return rc * ( rc <> 12 )

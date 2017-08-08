/* PDWSPAWN REXX     Spawn a pipeline for each connection request    */
/*                   Author: Rob van der Heij, 31 Jul 2017           */

signal on novalue
signal on error
'addstrm output spwn'

do forever
  'peekto req'

  'addpipe (end \ name PDWSPAWN.REXX:10)',
     '\ *.output.spwn:',
     '| take',
     '| copy',                        /* Make sure to consume        */
     '| o: fanout',
     '| spec 65.16 1',                /* Take the client's address   */
     '| socka2ip ',                   /* make it readable            */
     '| spec w3 1 ,:, n w2 n , , 23',
     '| out: juxtapose',              /* Prefix with IP address      */
     '| *.output:',                   /* with output of other        */
     '\ *.output:',
     '| out:',
     '\ o: ',
     '| i: fanin',
     '| tcpdata',
     '| h: httpsplit',
     '| xlate *-* from 850 to 1047',
     '| w: pdwserve',                 /* Process single request      */
     '| z: fanout',
     '| xlate *-* from 1047 to 850',
     '| y: faninany ',
     '| elastic',
     '| i:',
     '\ h:',                          /* Request body                */
     '| w:',
     '| y:',
     '\ z: ',
     '| buffer ',
     '| take ',
     '| out:'

  'select output spwn'
  'output' req
  'sever output'
  'select output 0'

end

error: return rc * ( rc <> 12 )
